#include <cpu.hpp>
#include <debug.hpp>
#include <errno.h>
#include <memory.hpp>
#include <process.hpp>
#include <syscalls.hpp>
#include <sysdefs.h>
#include <thread.hpp>

extern "C" void syscallHandler();

asm(
INLINE_ASM_SYNTAX
".extern mainTSS\n"
"syscallHandler:\n"
"push rbp\n"                                // save user frame pointer
"mov rbp, rsp\n"                            // save user stack pointer
"mov rsp, [rip + mainTSS + 4]\n"            // load kernel stack
"and rsp, ~0xF\n"                           // align stack for SSE

"push rbx\n"
"push rdx\n"
"push rsi\n"
"push rdi\n"
"push r8\n"
"push r9\n"
"push r10\n"
"push r12\n"
"push r13\n"
"push r14\n"
"push r15\n"
"push r15\n"                                // second push r15 to keep stack alignment

"push rcx\n"                                // save return address
"push r11\n"                                // save flags
"mov rcx, r10\n"                            // move argument 4 from r10 to rcx
"lea r11, [rip + _ZN8SysCalls8HandlersE]\n" // get SysCalls::Handler address
"mov r11, [r11 + rax * 8]\n"                // calculate handler address
"or r11, r11\n"                             // validate handler address
"jnz 1f\n"                                  //   and skip InvalidHandler call if handler is valid
"call _ZN8SysCalls14InvalidHandlerEPm\n"    // call InvalidHandler
"jmp 2f\n"                                  //   and return
"1: call r11\n"                             // call actual handler
"2: pop r11\n"                              // restore flags
"pop rcx\n"                                 // restore return address

"pop r15\n"
"pop r15\n"
"pop r14\n"
"pop r13\n"
"pop r12\n"
"pop r10\n"
"pop r9\n"
"pop r8\n"
"pop rdi\n"
"pop rsi\n"
"pop rdx\n"
"pop rbx\n"

"mov rsp, rbp\n"                            // restore user stack pointer
"pop rbp\n"                                 // restore user frame pointer
"sysret\n"                                  // return to usermode code
NORMAL_ASM_SYNTAX
);

SysCalls::SysCallHandler SysCalls::Handlers[1024];

intn SysCalls::InvalidHandler(uintn args[6])
{
    // not sure if this is stable
    uintn number; asm volatile("": "=a"(number));
    DEBUG("[syscalls] Unknown syscall %d\n", number);
    return -ENOSYS;
}

intn SysCalls::sysExitThread(intn retVal)
{
    Thread::Finalize(nullptr, retVal);
    return ESUCCESS;
}

intn SysCalls::sysExitProcess(intn retVal)
{
    Process::Finalize(0, retVal);
    return ESUCCESS;
}

void SysCalls::Initialize()
{
    Memory::Zero(Handlers, sizeof(Handlers));
    Handlers[SYS_EXIT_THREAD] = (SysCallHandler)sysExitThread;
    Handlers[SYS_EXIT_PROCESS] = (SysCallHandler)sysExitProcess;

    cpuWriteMSR(0xC0000081, (uintptr_t)(SEG_KERNEL_DATA) << 48 | (uintptr_t)(SEG_KERNEL_CODE) << 32);
    cpuWriteMSR(0xC0000082, (uintptr_t)syscallHandler);
    cpuWriteMSR(0xC0000084, 0);
}
