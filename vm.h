#pragma once

union va_t
{
    unsigned long long addr;

    struct
    {
        unsigned long long offset_4kb : 12;
        unsigned long long pt_index : 9;
        unsigned long long pd_index : 9;
        unsigned long long pdpt_index : 9;
        unsigned long long pml4_index : 9;
        unsigned long long reserved : 16;
    };

    struct
    {
        unsigned long long offset_2mb : 21;
        unsigned long long pd_index : 9;
        unsigned long long pdpt_index : 9;
        unsigned long long pml4_index : 9;
        unsigned long long reserved : 16;
    };

    struct
    {
        unsigned long long offset_1gb : 30;
        unsigned long long pdpt_index : 9;
        unsigned long long pml4_index : 9;
        unsigned long long reserved : 16;
    };
};

using pa_t = va_t;

typedef union
{
    struct
    {
        uintptr_t reserved1 : 3;
        uintptr_t page_level_write_through : 1;
        uintptr_t page_level_cache_disable : 1;
        uintptr_t reserved2 : 7;
        uintptr_t address_of_page_directory : 36;
        uintptr_t reserved3 : 16;
    };

    uintptr_t flags;
} cr3_t;

typedef union
{
    struct
    {
        uintptr_t present : 1;
        uintptr_t write : 1;
        uintptr_t supervisor : 1;
        uintptr_t page_level_write_through : 1;
        uintptr_t page_level_cache_disable : 1;
        uintptr_t accessed : 1;
        uintptr_t reserved1 : 1;
        uintptr_t must_be_zero : 1;
        uintptr_t ignored_1 : 3;
        uintptr_t restart : 1;
        uintptr_t pfn : 36;
        uintptr_t reserved2 : 4;
        uintptr_t ignored_2 : 11;
        uintptr_t execute_disable : 1;
    };

    uintptr_t addr;
} pml4e_t;

union pdpte_1gb_t
{
    struct
    {
        unsigned long long present : 1;
        unsigned long long write : 1;
        unsigned long long supervisor : 1;
        unsigned long long page_level_write_through : 1;
        unsigned long long page_level_cache_disable : 1;
        unsigned long long accessed : 1;
        unsigned long long dirty : 1;
        unsigned long long large_page : 1;
        unsigned long long global : 1;
        unsigned long long ignored_1 : 2;
        unsigned long long restart : 1;
        unsigned long long pat : 1;
        unsigned long long reserved1 : 17;
        unsigned long long pfn : 18;
        unsigned long long reserved2 : 4;
        unsigned long long ignored_2 : 7;
        unsigned long long protection_key : 4;
        unsigned long long execute_disable : 1;
    };

    unsigned long long addr;
};

union pdpte_t
{
    struct
    {
        unsigned long long present : 1;
        unsigned long long write : 1;
        unsigned long long supervisor : 1;
        unsigned long long page_level_write_through : 1;
        unsigned long long page_level_cache_disable : 1;
        unsigned long long accessed : 1;
        unsigned long long reserved1 : 1;
        unsigned long long large_page : 1;
        unsigned long long ignored_1 : 3;
        unsigned long long restart : 1;
        unsigned long long pfn : 36;
        unsigned long long reserved2 : 4;
        unsigned long long ignored_2 : 11;
        unsigned long long execute_disable : 1;
    };

    unsigned long long addr;
};

union pde_t
{
    struct
    {
        unsigned long long present : 1;
        unsigned long long write : 1;
        unsigned long long supervisor : 1;
        unsigned long long page_level_write_through : 1;
        unsigned long long page_level_cache_disable : 1;
        unsigned long long accessed : 1;
        unsigned long long reserved1 : 1;
        unsigned long long large_page : 1;
        unsigned long long ignored_1 : 3;
        unsigned long long restart : 1;
        unsigned long long pfn : 36;
        unsigned long long reserved2 : 4;
        unsigned long long ignored_2 : 11;
        unsigned long long execute_disable : 1;
    };

    unsigned long long addr;
};

union pde_2mb_t
{
    struct
    {
        unsigned long long present : 1;
        unsigned long long write : 1;
        unsigned long long supervisor : 1;
        unsigned long long page_level_write_through : 1;
        unsigned long long page_level_cache_disable : 1;
        unsigned long long accessed : 1;
        unsigned long long dirty : 1;
        unsigned long long large_page : 1;
        unsigned long long global : 1;
        unsigned long long ignored_1 : 2;
        unsigned long long restart : 1;
        unsigned long long pat : 1;
        unsigned long long reserved1 : 8;
        unsigned long long pfn : 27;
        unsigned long long reserved2 : 4;
        unsigned long long ignored_2 : 7;
        unsigned long long protection_key : 4;
        unsigned long long execute_disable : 1;
    };

    unsigned long long addr;
};

union pte_t
{
    struct
    {
        unsigned long long present : 1;
        unsigned long long write : 1;
        unsigned long long supervisor : 1;
        unsigned long long page_level_write_through : 1;
        unsigned long long page_level_cache_disable : 1;
        unsigned long long accessed : 1;
        unsigned long long dirty : 1;
        unsigned long long pat : 1;
        unsigned long long global : 1;
        unsigned long long ignored_1 : 2;
        unsigned long long restart : 1;
        unsigned long long pfn : 36;
        unsigned long long reserved1 : 4;
        unsigned long long ignored_2 : 7;
        unsigned long long protection_key : 4;
        unsigned long long execute_disable : 1;
    };

    unsigned long long addr;
};