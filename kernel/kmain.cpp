#include <cpu.hpp>
#include <debug.hpp>
#include <heap.hpp>
#include <irqs.hpp>
#include <misc.hpp>
#include <multiboot.h>
#include <paging.hpp>
#include <sysdefs.h>
#include <thread.hpp>
#include <time.hpp>
#include <types.h>

extern "C" int kmain(multiboot_info_t *mbootInfo)
{
    DEBUG("Starting WOOT v%d.%d (%s)\n",
          KERNEL_VERSION_MAJOR,
          KERNEL_VERSION_MINOR,
          KERNEL_VERSION_DESCR);
    IRQs::Initialize();
    Time::Initialize();
    Time::DateTime bootDateTime;
    Time::GetDateTime(&bootDateTime);
    DEBUG("[kmain] Boot started on %.4d-%.2d-%.2d at %.2d:%.2d:%.2d\n",
          bootDateTime.Year, bootDateTime.Month, bootDateTime.Day,
          bootDateTime.Hour, bootDateTime.Minute, bootDateTime.Second);

    cpuEnableInterrupts();
    Time::StartSystemTimer();

    //for(;;) cpuWaitForInterrupt(0);

    DEBUG("[kmain] Exiting kmain()\n");
    return 0xABCD;
}

typedef void (*InitFiniFunc)(void);

extern "C" InitFiniFunc __init_array_start[0], __init_array_end[0];
extern "C" InitFiniFunc __fini_array_start[0], __fini_array_end[0];

extern "C" void _init(multiboot_info_t *mbootInfo)
{
    Paging::Initialize(mbootInfo);
    Heap::Initialize(KERNEL_HEAP_BASE, KERNEL_HEAP_END, 16);
    Misc::InitializeDebugStream();

    for(InitFiniFunc *func = __init_array_start; func != __init_array_end; ++func)
        (*func)();

    Thread::Initialize();
}

extern "C" void _fini(void)
{
    for(InitFiniFunc *func = __fini_array_start; func != __fini_array_end; ++func)
        (*func)();
}
