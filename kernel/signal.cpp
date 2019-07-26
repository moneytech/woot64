#include <cpu.hpp>
#include <debug.hpp>
#include <memory.hpp>
#include <signal.hpp>
#include <syscalls.hpp>
#include <sysdefs.h>
#include <thread.hpp>

#undef MAKE_STR
#undef STRINGIFY
#define MAKE_STR(s) #s
#define STRINGIFY(s) MAKE_STR(s)

extern "C" void __sigHandlerAsm();
/*extern "C" __attribute__((section(".text.user"))) void __sigHandlerC(int no, ...)
{
    int signum = __doSyscall(SYS_SIGNAL_GET_CURRENT);
    void (*handler)(int) = (void (*)(int))__doSyscall(SYS_SIGNAL_GET_HANDLER, signum);
    handler(signum);
    __doSyscall(SYS_SIGNAL_RETURN);
}*/

asm(
INLINE_ASM_SYNTAX
".section .text.user\n"
"__sigHandlerAsm:\n"

"mov rax, " STRINGIFY(SYS_SIGNAL_GET_CURRENT)
"\nsyscall\n"

"push rax\n"
"mov rdi, rax\n"
"mov rax, " STRINGIFY(SYS_SIGNAL_GET_HANDLER)
"\nsyscall\n"
"pop rdi\n"
"call rax\n"

"mov rax, " STRINGIFY(SYS_SIGNAL_RETURN)
"\nsyscall\n"
"1: int3\n"    // shouldn't execute
"hlt\n"  // generate GPF exception
"jmp 1b\n"  // if this fails try again

".section .text\n"
NORMAL_ASM_SYNTAX);

void Signal::ReturnFromSignal(Thread *thread, Ints::State *state)
{
    bool ints = cpuDisableInterrupts();
    // restore saved registers
    Memory::Move(state, &thread->SavedMachineState, sizeof(Ints::State));
    thread->CurrentSignal = -1;
    thread->Suspend();
    cpuRestoreInterrupts(ints);
}

void Signal::HandleSignals(Thread *thread, Ints::State *state)
{
    bool ints = cpuDisableInterrupts();
    bool ok = false;
    uint8_t signum = thread->SignalQueue.Read(&ok);
    if(ok && signum < SIGNAL_COUNT && thread->SignalHandlers[signum])
    {   // we have queued signal to handle
        // save registers
        //DEBUG("[signal] interrupt handler\n");
        Memory::Move(&thread->SavedMachineState, state, sizeof(Ints::State));
        thread->CurrentSignal = signum;

        // inject signal handler
        *reinterpret_cast<uintptr_t *>(state->RSP -= sizeof(uintptr_t)) = 0x90909080CDC03148ULL;    // xor rax, rax / int 0x80
        uintptr_t retAddr = state->RSP;
        state->RDI = signum;    // signal argument
        *reinterpret_cast<uintptr_t *>(state->RSP -= sizeof(uintptr_t)) = retAddr; // return address

        // and modify instruction pointer
        state->RIP = reinterpret_cast<uintptr_t>(thread->SignalHandlers[signum]);
    }
    cpuRestoreInterrupts(ints);
}

void Signal::HandleSignals(Thread *thread, uintptr_t *retAddrAddr)
{
    bool ints = cpuDisableInterrupts();
    bool ok = false;
    uint8_t signum = thread->SignalQueue.Read(&ok);
    if(ok && signum < SIGNAL_COUNT && thread->SignalHandlers[signum])
    {   // we have queued signal to handle
        //DEBUG("[signal] syscall handler\n");
        thread->CurrentSignal = signum;
        thread->SignalRetAddr = *retAddrAddr;
        *retAddrAddr = reinterpret_cast<uintptr_t>(__sigHandlerAsm);
    }
    cpuRestoreInterrupts(ints);
}

void Signal::Raise(Thread *thread, unsigned signum)
{
    bool ints = cpuDisableInterrupts();
    if((1ull << signum) & thread->SignalMask)
    {
        thread->SignalQueue.Write(static_cast<uint8_t>(signum));
        thread->Resume(false);
    }
    cpuRestoreInterrupts(ints);
}
