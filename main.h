#pragma once

struct RTL_PROCESS_MODULE_INFORMATION
{
    void *Section;
    void *MappedBase;
    void *ImageBase;
    unsigned long  ImageSize;
    unsigned long  Flags;
    unsigned short LoadOrderIndex;
    unsigned short InitOrderIndex;
    unsigned short LoadCount;
    unsigned short OffsetToFileName;
    char           FullPathName[ 0x0100 ];
};

struct RTL_PROCESS_MODULES
{
    unsigned long                  NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[ 1 ];
};

typedef struct _MMVAD_FLAGS2
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned long FileOffset : 24; /* bit position: 0 */
        /* 0x0000 */ unsigned long Large : 1; /* bit position: 24 */
        /* 0x0000 */ unsigned long TrimBehind : 1; /* bit position: 25 */
        /* 0x0000 */ unsigned long Inherit : 1; /* bit position: 26 */
        /* 0x0000 */ unsigned long NoValidationNeeded : 1; /* bit position: 27 */
        /* 0x0000 */ unsigned long PrivateDemandZero : 1; /* bit position: 28 */
        /* 0x0000 */ unsigned long Spare : 3; /* bit position: 29 */
    }; /* bitfield */
} MMVAD_FLAGS2, *PMMVAD_FLAGS2; /* size: 0x0004 */

typedef struct _MI_VAD_SEQUENTIAL_INFO
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned __int64 Length : 12; /* bit position: 0 */
        /* 0x0000 */ unsigned __int64 Vpn : 52; /* bit position: 12 */
    }; /* bitfield */
} MI_VAD_SEQUENTIAL_INFO, *PMI_VAD_SEQUENTIAL_INFO; /* size: 0x0008 */

typedef struct _MMVAD_FLAGS
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned long Lock : 1; /* bit position: 0 */
        /* 0x0000 */ unsigned long LockContended : 1; /* bit position: 1 */
        /* 0x0000 */ unsigned long DeleteInProgress : 1; /* bit position: 2 */
        /* 0x0000 */ unsigned long NoChange : 1; /* bit position: 3 */
        /* 0x0000 */ unsigned long VadType : 3; /* bit position: 4 */
        /* 0x0000 */ unsigned long Protection : 5; /* bit position: 7 */
        /* 0x0000 */ unsigned long PreferredNode : 6; /* bit position: 12 */
        /* 0x0000 */ unsigned long PageSize : 2; /* bit position: 18 */
        /* 0x0000 */ unsigned long PrivateMemory : 1; /* bit position: 20 */
    }; /* bitfield */
} MMVAD_FLAGS, *PMMVAD_FLAGS; /* size: 0x0004 */

typedef struct _MM_PRIVATE_VAD_FLAGS
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned long Lock : 1; /* bit position: 0 */
        /* 0x0000 */ unsigned long LockContended : 1; /* bit position: 1 */
        /* 0x0000 */ unsigned long DeleteInProgress : 1; /* bit position: 2 */
        /* 0x0000 */ unsigned long NoChange : 1; /* bit position: 3 */
        /* 0x0000 */ unsigned long VadType : 3; /* bit position: 4 */
        /* 0x0000 */ unsigned long Protection : 5; /* bit position: 7 */
        /* 0x0000 */ unsigned long PreferredNode : 6; /* bit position: 12 */
        /* 0x0000 */ unsigned long PageSize : 2; /* bit position: 18 */
        /* 0x0000 */ unsigned long PrivateMemoryAlwaysSet : 1; /* bit position: 20 */
        /* 0x0000 */ unsigned long WriteWatch : 1; /* bit position: 21 */
        /* 0x0000 */ unsigned long FixedLargePageSize : 1; /* bit position: 22 */
        /* 0x0000 */ unsigned long ZeroFillPagesOptional : 1; /* bit position: 23 */
        /* 0x0000 */ unsigned long Graphics : 1; /* bit position: 24 */
        /* 0x0000 */ unsigned long Enclave : 1; /* bit position: 25 */
        /* 0x0000 */ unsigned long ShadowStack : 1; /* bit position: 26 */
        /* 0x0000 */ unsigned long PhysicalMemoryPfnsReferenced : 1; /* bit position: 27 */
    }; /* bitfield */
} MM_PRIVATE_VAD_FLAGS, *PMM_PRIVATE_VAD_FLAGS; /* size: 0x0004 */

typedef struct _MM_GRAPHICS_VAD_FLAGS
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned long Lock : 1; /* bit position: 0 */
        /* 0x0000 */ unsigned long LockContended : 1; /* bit position: 1 */
        /* 0x0000 */ unsigned long DeleteInProgress : 1; /* bit position: 2 */
        /* 0x0000 */ unsigned long NoChange : 1; /* bit position: 3 */
        /* 0x0000 */ unsigned long VadType : 3; /* bit position: 4 */
        /* 0x0000 */ unsigned long Protection : 5; /* bit position: 7 */
        /* 0x0000 */ unsigned long PreferredNode : 6; /* bit position: 12 */
        /* 0x0000 */ unsigned long PageSize : 2; /* bit position: 18 */
        /* 0x0000 */ unsigned long PrivateMemoryAlwaysSet : 1; /* bit position: 20 */
        /* 0x0000 */ unsigned long WriteWatch : 1; /* bit position: 21 */
        /* 0x0000 */ unsigned long FixedLargePageSize : 1; /* bit position: 22 */
        /* 0x0000 */ unsigned long ZeroFillPagesOptional : 1; /* bit position: 23 */
        /* 0x0000 */ unsigned long GraphicsAlwaysSet : 1; /* bit position: 24 */
        /* 0x0000 */ unsigned long GraphicsUseCoherentBus : 1; /* bit position: 25 */
        /* 0x0000 */ unsigned long GraphicsNoCache : 1; /* bit position: 26 */
        /* 0x0000 */ unsigned long GraphicsPageProtection : 3; /* bit position: 27 */
    }; /* bitfield */
} MM_GRAPHICS_VAD_FLAGS, *PMM_GRAPHICS_VAD_FLAGS; /* size: 0x0004 */

typedef struct _MM_SHARED_VAD_FLAGS
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned long Lock : 1; /* bit position: 0 */
        /* 0x0000 */ unsigned long LockContended : 1; /* bit position: 1 */
        /* 0x0000 */ unsigned long DeleteInProgress : 1; /* bit position: 2 */
        /* 0x0000 */ unsigned long NoChange : 1; /* bit position: 3 */
        /* 0x0000 */ unsigned long VadType : 3; /* bit position: 4 */
        /* 0x0000 */ unsigned long Protection : 5; /* bit position: 7 */
        /* 0x0000 */ unsigned long PreferredNode : 6; /* bit position: 12 */
        /* 0x0000 */ unsigned long PageSize : 2; /* bit position: 18 */
        /* 0x0000 */ unsigned long PrivateMemoryAlwaysClear : 1; /* bit position: 20 */
        /* 0x0000 */ unsigned long PrivateFixup : 1; /* bit position: 21 */
        /* 0x0000 */ unsigned long HotPatchAllowed : 1; /* bit position: 22 */
    }; /* bitfield */
} MM_SHARED_VAD_FLAGS, *PMM_SHARED_VAD_FLAGS; /* size: 0x0004 */

typedef struct _MMVAD_FLAGS1
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned long CommitCharge : 31; /* bit position: 0 */
        /* 0x0000 */ unsigned long MemCommit : 1; /* bit position: 31 */
    }; /* bitfield */
} MMVAD_FLAGS1, *PMMVAD_FLAGS1; /* size: 0x0004 */

typedef struct _MMVAD_SHORT
{
    union
    {
        struct
        {
            /* 0x0000 */ struct _MMVAD_SHORT *NextVad;
            /* 0x0008 */ void *ExtraCreateInfo;
        }; /* size: 0x0010 */
        /* 0x0000 */ struct _RTL_BALANCED_NODE VadNode;
    }; /* size: 0x0018 */
    /* 0x0018 */ unsigned long StartingVpn;
    /* 0x001c */ unsigned long EndingVpn;
    /* 0x0020 */ unsigned char StartingVpnHigh;
    /* 0x0021 */ unsigned char EndingVpnHigh;
    /* 0x0022 */ unsigned char CommitChargeHigh;
    /* 0x0023 */ unsigned char SpareNT64VadUChar;
    /* 0x0024 */ long ReferenceCount;
    /* 0x0028 */ EX_PUSH_LOCK PushLock;
    union
    {
        union
        {
            /* 0x0030 */ unsigned long LongFlags;
            /* 0x0030 */ struct _MMVAD_FLAGS VadFlags;
            /* 0x0030 */ struct _MM_PRIVATE_VAD_FLAGS PrivateVadFlags;
            /* 0x0030 */ struct _MM_GRAPHICS_VAD_FLAGS GraphicsVadFlags;
            /* 0x0030 */ struct _MM_SHARED_VAD_FLAGS SharedVadFlags;
            /* 0x0030 */ volatile unsigned long VolatileVadLong;
        }; /* size: 0x0004 */
    } /* size: 0x0004 */ u;
    union
    {
        union
        {
            /* 0x0034 */ unsigned long LongFlags1;
            /* 0x0034 */ struct _MMVAD_FLAGS1 VadFlags1;
        }; /* size: 0x0004 */
    } /* size: 0x0004 */ u1;
    /* 0x0038 */ struct _MI_VAD_EVENT_BLOCK *EventList;
} MMVAD_SHORT, *PMMVAD_SHORT; /* size: 0x0040 */

typedef struct _MMVAD
{
    /* 0x0000 */ struct _MMVAD_SHORT Core;
    union
    {
        union
        {
            /* 0x0040 */ unsigned long LongFlags2;
            /* 0x0040 */ volatile struct _MMVAD_FLAGS2 VadFlags2;
        }; /* size: 0x0004 */
    } /* size: 0x0004 */ u2;
    /* 0x0044 */ long Padding_770;
    /* 0x0048 */ struct _SUBSECTION *Subsection;
    /* 0x0050 */ struct _MMPTE *FirstPrototypePte;
    /* 0x0058 */ struct _MMPTE *LastContiguousPte;
    /* 0x0060 */ struct _LIST_ENTRY ViewLinks;
    /* 0x0070 */ struct _EPROCESS *VadsProcess;
    union
    {
        union
        {
            /* 0x0078 */ struct _MI_VAD_SEQUENTIAL_INFO SequentialVa;
            /* 0x0078 */ struct _MMEXTEND_INFO *ExtendedInfo;
        }; /* size: 0x0008 */
    } /* size: 0x0008 */ u4;
    /* 0x0080 */ struct _FILE_OBJECT *FileObject;
} MMVAD, *PMMVAD; /* size: 0x0088 */

