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

Thread *kernelThread = nullptr;

int testThread(uintptr_t arg)
{
    for(int i = 0; i < 10; ++i)
    {
        DEBUG("testThread %d\n", arg);
        Time::Sleep(510, false);
        if(arg == 125)
            kernelThread->Resume(false);
    }
    if(arg == 123) asm("int3");
    return 0xAA55AA55 + arg;
}

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

    kernelThread = Thread::GetCurrent();

    int r1 = 0;
    Thread *tt1 = new Thread("test 1", nullptr, (void *)testThread, 123, 0, 0, &r1, nullptr);
    tt1->Enable();
    tt1->Resume(false);

    int r2 = 0;
    Thread *tt2 = new Thread("test 2", nullptr, (void *)testThread, 125, 0, 0, &r2, nullptr);
    tt2->Enable();
    tt2->Resume(false);

    Time::Sleep(100, false);
    for(;;)
    {
        DEBUG("L: %d %x %x\n", Time::Sleep(1000, true), r1, r2);
    }

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
