#include <ntifs.h>
#include <intrin.h>

#include "main.h"
#include "shellcode.h"
#include "vm.h"

static UNICODE_STRING device_name = RTL_CONSTANT_STRING( L"\\Device\\Injector" );
static UNICODE_STRING symlink_name = RTL_CONSTANT_STRING( L"\\??\\Injector" );

struct scanpattern_t
{
    const char *pattern;
    const char *mask;
};

scanpattern_t miallocatevad_pattern{
    "\x48\x8B\x8E\x00\x00\x00\x00\x48\x8D\x57\xFF\x45\x33\xC0\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x08\x48\x89\x18\x48\x8B\xD8\xEB\xCC",
    "xxx????xxxxxxxx????xxxxxxxxxxxxx"
};

scanpattern_t mmallocateindependentpages_pattern{
    "\x48\x8B\xC4\x48\x89\x58\x10\x44\x89\x48\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00",
    "xxxxxxxxxxxxxxxxxxxxxxxxx????"
};

scanpattern_t mi_reserve_ptes_pattern{
    "\xBA\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x4C\x8B\xE0",
    "x????xxx????x????xxx"
};

#define B2_S(b) ( b ) ? "yes" : "no"
#define log(...) DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_MASK | DPFLTR_INFO_LEVEL, "[" __FUNCTION__ "] " ##__VA_ARGS__)

extern "C" NTSTATUS ZwQuerySystemInformation( int,
                                              PVOID,
                                              ULONG,
                                              PULONG );

extern "C" NTKERNELAPI
PVOID
PsGetProcessSectionBaseAddress(
    __in PEPROCESS Process
);

using MmAllocateIndependentPagesEx_t = PVOID( * )( SIZE_T,
                                                   ULONG,
                                                   SIZE_T,
                                                   SIZE_T );

MmAllocateIndependentPagesEx_t MmAllocateIndependentPagesEx{ nullptr };

static uintptr_t get_eprocess_for_process( const char *name )
{
    const auto system_process = IoGetCurrentProcess ( );
    const auto proc_list_head = reinterpret_cast< LIST_ENTRY* >(
        reinterpret_cast< uintptr_t >( system_process ) + 1096 );

    for ( auto next = proc_list_head->Flink; next != proc_list_head; next = next->Flink )
    {
        const auto eprocess_base = reinterpret_cast< PEPROCESS >( reinterpret_cast< uintptr_t >( next ) - 1096 );
        const auto exit_time = *reinterpret_cast< PLARGE_INTEGER >( reinterpret_cast< uintptr_t >( eprocess_base ) +
            2112 );
        const char *img_name = reinterpret_cast< const char* >( eprocess_base ) + 1448;

        if ( !img_name )
            continue;

        if ( !exit_time.QuadPart && _strnicmp( name,
                                               img_name,
                                               strlen( name ) ) == 0 )
        {
            return reinterpret_cast< uintptr_t >( eprocess_base );
        }
    }
    return 0;
}

void read_phys( void *dst,
                const uintptr_t *src,
                const size_t read_size = sizeof( uintptr_t ) )
{
    const MM_COPY_ADDRESS addr{ .PhysicalAddress = { .QuadPart = reinterpret_cast< long long >( src ) } };
    size_t read_count{ 0 };

    const auto status{
        MmCopyMemory( dst,
                      addr,
                      read_size,
                      MM_COPY_MEMORY_PHYSICAL,
                      &read_count )
    };

    if ( !NT_SUCCESS( status ) )
        log( "failed to read physical memory, status: 0x%08x\n",
         status );
}


bool inject_pml4_and_shellcode( PEPROCESS process,
                                const uintptr_t cr3 )
{
    const auto pdpt{
        MmAllocateContiguousMemory(
            PAGE_SIZE,
            { .QuadPart = static_cast< LONGLONG >( MAXUINT64 ) }
        )
    };

    const auto pd{
        MmAllocateContiguousMemory(
            PAGE_SIZE,
            { .QuadPart = static_cast< LONGLONG >( MAXUINT64 ) }
        )
    };

    void *pg = nullptr;

    constexpr auto pg_align{ 0x200000 };

    pg = MmAllocateContiguousMemory(
        0x200000 + PAGE_SIZE,
        { .QuadPart = static_cast< LONGLONG >( MAXUINT64 ) }
    );

    if ( ( reinterpret_cast< uintptr_t >( pg ) & ( pg_align - 1 ) ) == 0 )
        log( "aligned address: %p (%p)\n",
         pg,
         MmGetPhysicalAddress( pg ).QuadPart );
    else
        log( "unaligned address: %p (%p)\n",
         pg,
         MmGetPhysicalAddress( pg ).QuadPart );


    if ( !pdpt || !pd || !pg )
    {
        log( "unable to allocate physical pages, returning\n" );
        return false;
    }

    const auto pdpt_phys{
        MmGetPhysicalAddress( pdpt ).QuadPart
    };

    const auto pd_phys{
        MmGetPhysicalAddress( pd ).QuadPart
    };

    const auto pg_phys{
        MmGetPhysicalAddress( pg ).QuadPart
    };

    memset( pg,
            0,
            0x200000 );

    memset( pd,
            0,
            PAGE_SIZE );

    memset( pdpt,
            0,
            PAGE_SIZE );

    memset( pg,
            'A',
            0x200000 );

    KAPC_STATE apc_state{ };

    KeStackAttachProcess( process,
                          &apc_state );

    const auto pml4{
        static_cast< pml4e_t* >(
            MmGetVirtualForPhysical( { .QuadPart = static_cast< long long >( cr3 ) } )
        )
    };

    log( "pdpt: 0x%llx pd: 0x%llx pg: 0x%llx\n",
         pdpt_phys,
         pd_phys,
         pg_phys );

    constexpr auto user_addr_space{ 256llu };

    for ( auto pml4_idx{ 0ull }; pml4_idx < user_addr_space; pml4_idx++ )
    {
        const auto pml4e{
            &pml4[ pml4_idx ]
        };

        if ( !pml4e->addr )
        {
            pml4e_t new_entry{ };

            new_entry.pfn = pdpt_phys >> 12;
            new_entry.present = 1;
            new_entry.write = 1;
            new_entry.supervisor = 0;
            new_entry.accessed = 1;
            new_entry.execute_disable = 0;

            *pml4e = new_entry;

            pdpte_t pdpte{ };

            pdpte.pfn = pd_phys >> 12;
            pdpte.present = 1;
            pdpte.write = 1;
            pdpte.supervisor = 0;
            pdpte.accessed = 1;
            pdpte.execute_disable = 0;

            *reinterpret_cast< pdpte_t* >( static_cast< uintptr_t* >( pdpt ) + 1 ) = pdpte;

            pde_2mb_t pde{ };

            pde.pfn = pg_phys >> 21;
            pde.present = 1;
            pde.write = 1;
            pde.supervisor = 1;
            pde.execute_disable = 0;
            pde.large_page = 1;

            *reinterpret_cast< pde_2mb_t* >( static_cast< uintptr_t* >( pd ) + 2 ) = pde;

            va_t va{ };

            va.pml4_index = pml4_idx;
            va.pdpt_index = 1;
            va.pd_index = 2;
            va.offset_2mb = pg_phys & 0x1fffff;

            log( "new virtual address: 0x%llx cr3: 0x%llx\n",
                 va.addr,
                 cr3 );

            const auto cr4{
                __readcr4 ( )
            };

            __writecr4( cr4 ^ 0b10000000 );
            __writecr4( cr4 );

            void *addr{ reinterpret_cast< void* >( va.addr + ( 0x200000ull * 2 ) ) };
            size_t reg_sz{ 0x200000 };

            const auto status{
                ZwAllocateVirtualMemory( NtCurrentProcess ( ),
                                         &addr,
                                         0,
                                         &reg_sz,
                                         MEM_COMMIT,
                                         PAGE_READWRITE )
            };

            if ( !NT_SUCCESS( status ) )
            {
                log( "status: 0x%08x\n",
                     status );
            }
            else
            {
                log( "virtual memory allocated at %p (size: %llu bytes)\n",
                     addr,
                     reg_sz );
            }

            break;
        }

        log( "pte: 0x%llx present: %s write: %s u/s: %s accessed: %s executable: %s pfn: 0x%llx\n",
             pml4e,
             B2_S( pml4e->present ),
             B2_S( pml4e->write ),
             pml4e->supervisor ? "u" : "s",
             B2_S( pml4e->accessed ),
             pml4e->execute_disable ? "n" : "y",
             pml4e->pfn << 12
        );
    }

    KeUnstackDetachProcess( &apc_state );

    return true;
}

RTL_PROCESS_MODULE_INFORMATION *get_module_base_addr( const char *name )
{
    if ( !name )
        return nullptr;

    unsigned long required_alloc_sz = 0UL;

    ZwQuerySystemInformation( 11,
                              nullptr,
                              0,
                              &required_alloc_sz );

    if ( !required_alloc_sz )
    {
        return nullptr;
    }

    const auto buffer = ExAllocatePool2( POOL_FLAG_NON_PAGED,
                                         static_cast< SIZE_T >( required_alloc_sz ) * 2,
                                         'iSCS' );

    if ( buffer )
    {
        const auto _buffer = static_cast< RTL_PROCESS_MODULES* >( buffer );

        const auto status = ZwQuerySystemInformation( 11,
                                                      _buffer,
                                                      required_alloc_sz * 2,
                                                      nullptr );

        if ( !NT_SUCCESS( status ) )
        {
            ExFreePool( buffer );
            return nullptr;
        }

        for ( auto idx = 0UL; idx < _buffer->NumberOfModules; idx++ )
        {
            const auto entry = &_buffer->Modules[ idx ];
            const auto filename = entry->FullPathName + entry->OffsetToFileName;
            if ( _strnicmp( filename,
                            name,
                            strlen( filename ) ) == 0 )
            {
                return entry;
            }
        }
    }

    return nullptr;
}

bool check_mask( unsigned char *base,
                 unsigned char *pattern,
                 unsigned char *mask )
{
    for ( ; *mask; ++base, ++pattern, ++mask )
    {
        if ( *mask == 'x' && *base != *pattern )
        {
            return false;
        }
    }

    return true;
}

void *find_pattern( char *base,
                    size_t size,
                    const char *pattern,
                    const char *mask )
{
    size -= strlen( mask );

    for ( size_t i = 0; i <= size; ++i )
    {
        const auto addr = &base[ i ];
        if ( check_mask(
                reinterpret_cast< unsigned char* >( addr ),
                reinterpret_cast< unsigned char* >( const_cast< char* >( pattern ) ),
                reinterpret_cast< unsigned char* >( const_cast< char* >( mask ) ) )
        )
        {
            return addr;
        }
    }

    return nullptr;
}

void test_mi_reserve_ptes( )
{
    return;
}

void DrvUnload( PDRIVER_OBJECT drv_obj )
{
    IoDeleteDevice( drv_obj->DeviceObject );
    IoDeleteSymbolicLink( &symlink_name );
}

NTSTATUS DrvCreateClose( PDEVICE_OBJECT device_obj,
                         PIRP irp )
{
    ( void )device_obj;

    irp->IoStatus.Information = 0;
    irp->IoStatus.Status = STATUS_SUCCESS;

    IoCompleteRequest( irp,
                       IO_NO_INCREMENT );

    return STATUS_SUCCESS;
}

extern "C" NTSTATUS DriverEntry( PDRIVER_OBJECT drv_object,
                                 UNICODE_STRING reg_path )
{
    ( void )drv_object;
    ( void )reg_path;

    PDEVICE_OBJECT device_obj{ nullptr };
    auto status{
        IoCreateDevice( drv_object,
                        0,
                        &device_name,
                        0,
                        0,
                        FALSE,
                        &device_obj )
    };

    if ( !NT_SUCCESS( status ) )
        return status;

    status = IoCreateSymbolicLink( &symlink_name,
                                   &device_name );

    if ( !NT_SUCCESS( status ) )
        return status;

    drv_object->MajorFunction[ IRP_MJ_CREATE ] = drv_object->MajorFunction[ IRP_MJ_CLOSE ] = DrvCreateClose;
    drv_object->DriverUnload = DrvUnload;

    log( "created finished driver initialization, device object: 0x%p\n",
         device_obj );

    log( "shellcode buffer: 0x%p\n",
         shellcode );

    const auto eprocess{
        get_eprocess_for_process( "dwm.exe" )
    };

    const auto dirbase{
        *reinterpret_cast< uintptr_t* >( eprocess + 0x28 )
    };

    const auto ntoskrnl{ get_module_base_addr( "ntoskrnl.exe" ) };

    const auto mi_reserve_ptes = ( char* )find_pattern(
        ( char* )ntoskrnl->ImageBase,
        ntoskrnl->ImageSize,
        mi_reserve_ptes_pattern.pattern,
        mi_reserve_ptes_pattern.mask
    );

    using mi_reserve_ptes_t = void *( * )( void *,
                                           int );

    const auto vs_offset = *( long* )( mi_reserve_ptes + 0x8 );

    const auto offset = *( long* )( mi_reserve_ptes + 0xd );
    const auto mi_reserve_ptes_fn = ( mi_reserve_ptes_t )( mi_reserve_ptes + 0x11 + offset );
    const auto vs = mi_reserve_ptes + 0xc + vs_offset;

    log( "mi reserve ptes: %p offset: %04x fn addr: %p vs offset: %04x vs: %p\n",
         mi_reserve_ptes,
         offset,
         mi_reserve_ptes_fn,
         vs_offset,
         vs
    );

    using u64 = unsigned long long;
    using u32 = unsigned long;

    using mi_make_valid_pte_t = void *( * )(
        pte_t *pte,
        u64 pfn,
        u32 prot
    );

    const auto pivot = ( u64 )MmAllocateNonCachedMemory + 308;
    const auto mi_make_valid_pte_offset = *( u32* )( pivot );
    const auto mi_make_valid_pte = ( mi_make_valid_pte_t )( pivot + 4 + mi_make_valid_pte_offset );

    log( "mi_make_valid_pte: %p\n",
         mi_make_valid_pte );

    const auto ptes = mi_reserve_ptes_fn(
        vs,
        1
    );

    const auto kernel_phys = MmGetPhysicalAddress( ntoskrnl->ImageBase ).QuadPart;

    auto pte = mi_make_valid_pte(
        ( pte_t* )ptes,
        kernel_phys >> 9,
        0xa000004
    );

    *( pte_t** )ptes = ( pte_t* )pte;

    log(
        "ptes: %p pte: %p kernel: %p\n",
        ptes,
        pte,
        kernel_phys >> 9
    );

    __debugbreak ( );

    const auto image_base{
        reinterpret_cast< uintptr_t >(
            PsGetProcessSectionBaseAddress( reinterpret_cast< PEPROCESS >( eprocess ) )
        )
    };

    log( "eprocess: %p cr3: 0x%llx image base: 0x%llx\n",
         eprocess,
         dirbase,
         image_base );

    return STATUS_SUCCESS;
}
