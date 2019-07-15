#include <../ahci/ahcidrive.hpp>
#include <../ext2/ext2.hpp>
#include <../ide/idedrive.hpp>
#include <cpu.hpp>
#include <debug.hpp>
#include <file.hpp>
#include <heap.hpp>
#include <irqs.hpp>
#include <misc.hpp>
#include <module.hpp>
#include <multiboot.h>
#include <paging.hpp>
#include <partvolume.hpp>
#include <pci.hpp>
#include <process.hpp>
#include <syscalls.hpp>
#include <sysdefs.h>
#include <thread.hpp>
#include <time.hpp>
#include <types.h>

#include <elf.hpp>
#include <semaphore.hpp>

extern "C" int kmain(multiboot_info_t *mbootInfo)
{
    (void)mbootInfo;
    DEBUG("Starting WOOT v%d.%d (%s)\n", KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR, KERNEL_VERSION_DESCR);
    Misc::InitializeBreakpointInt();
    IRQs::Initialize();
    Time::Initialize();
    Time::DateTime bootDateTime;
    Time::GetDateTime(&bootDateTime);
    DEBUG("[kmain] Boot started on %.4d-%.2d-%.2d at %.2d:%.2d:%.2d\n",
          bootDateTime.Year, bootDateTime.Month, bootDateTime.Day,
          bootDateTime.Hour, bootDateTime.Minute, bootDateTime.Second);
    cpuEnableInterrupts();
    Time::StartSystemTimer();
    PCI::Initialize();
    IDEDrive::Initialize();
    AHCIDrive::Initialize();
    PartVolume::Initialize();
    EXT2::Initialize();
    Volume::DetectAll();
    FileSystem::DetectAll();
    SysCalls::Initialize();

    // get main kernel process
    Process *kernelProc = Process::GetCurrent();

    // initialize current directory for kernel process
    File *rootDir = File::Open("WOOT_OS~/", O_RDONLY, 0);
    if(rootDir)
    {
        kernelProc->CurrentDirectory = FileSystem::GetDEntry(rootDir->DEntry);
        delete rootDir;
    }

    ELF *kernelImage = ELF::Load("/system/kernel", false, true, false);
    if(!kernelImage) DEBUG("[kmain] Couldn't load kernel image. Modules won't load properly.\n");

    Module::LoadBootModules();
    Module::ProbeAll();

    const char *procExec = "/bin/init";
    //const char *procExec = "/bin/usertest";
    //const char *procExec = "/bin/usertest_static";
    //const char *procExec = "/lib/libc.so /bin/usertest";
    //const char *procExec = "/lib/libc.so";
    //const char *procExec = "/lib/libc.so /bin/usertest_static";
    Semaphore done(0);
    int res;
    Process *proc = Process::Create(procExec, &done, false, &res);
    if(proc)
    {
        proc->Start();
        done.Wait(0, false, false);
        DEBUG("'%s' returned %d\n", proc->Name, res);
    }
    else DEBUG("Couldn't start '%s'\n", procExec);

    FileSystem::SynchronizeAll();
    Volume::SynchronizeAll();
    Module::CleanupAll();

    EXT2::Cleanup();
    PartVolume::Cleanup();
    AHCIDrive::Cleanup();
    IDEDrive::Cleanup();
    PCI::Cleanup();
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
    Debug::Initialize();

    for(InitFiniFunc *func = __init_array_start; func != __init_array_end; ++func)
        (*func)();

    Thread::Initialize();
    Process::Initialize();
}

extern "C" void _fini(void)
{
    for(InitFiniFunc *func = __fini_array_start; func != __fini_array_end; ++func)
        (*func)();
}
