#include <cpu.hpp>
#include <heap.hpp>
#include <multiboot.h>
#include <paging.hpp>
#include <sysdefs.h>
#include <types.h>

extern "C" int kmain(multiboot_info_t *mbootInfo)
{
    return 0xABCD;
}

typedef void (*InitFiniFunc)(void);

extern "C" InitFiniFunc __init_array_start[0], __init_array_end[0];
extern "C" InitFiniFunc __fini_array_start[0], __fini_array_end[0];

extern "C" void _init(multiboot_info_t *mbootInfo)
{
    Paging::Initialize(mbootInfo);
    Heap::Initialize(KERNEL_HEAP_BASE, KERNEL_HEAP_END, 16);

    for(InitFiniFunc *func = __init_array_start; func != __init_array_end; ++func)
        (*func)();
}

extern "C" void _fini(void)
{
    for(InitFiniFunc *func = __fini_array_start; func != __fini_array_end; ++func)
        (*func)();
}
