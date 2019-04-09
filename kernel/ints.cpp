#include <cpu.hpp>
#include <debug.hpp>
#include <ints.hpp>
#include <irqs.hpp>
#include <process.hpp>
#include <thread.hpp>

#define VECTOR_COUNT 256

static const char *excNames[] =
{
    "Division by zero",
    "Debug exception",
    "Non-maskable interrupt",
    "Breakpoint",
    "Overflow exception",
    "Bound range exceeded",
    "Invalid opcode",
    "Device not available",
    "Double fault",
    "Coprocessor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack segment fault",
    "General protection fault",
    "Page fault",
    "Exception 15",
    "x87 FPU exception",
    "Alignment check",
    "Machine check",
    "SIMD FPU exception",
    "Virtualization exception",
    "Exception 21",
    "Exception 22",
    "Exception 23",
    "Exception 24",
    "Exception 25",
    "Exception 26",
    "Exception 27",
    "Exception 28",
    "Exception 29",
    "Security exception",
    "Exception 31"
};

Ints::Handler *Ints::Handlers[VECTOR_COUNT];

extern "C" void intsCommonHandler(Ints::State *state)
{
    Ints::CommonHandler(state);
}

void Ints::CommonHandler(Ints::State *state)
{
    int irq = state->InterruptNumber - IRQs::Base;
    bool isIRQ = irq >= 0 && irq < IRQs::Count;
    Process *cp = Process::GetCurrent();
    Thread *ct = Thread::GetCurrent();

    // handle spurious irqs
    if(isIRQ)
    {
        if(IRQs::IsSpurious(irq))
        {
            IRQs::HandleSpurious(irq);
            return;
        }
    }

    bool handled = false;

    /*if(ct)
    {
        //if(state->CS & 3 && ct->CurrentSignal < 0)
        //    Signal::HandleSignals(ct, state);
        if(state->InterruptNumber == 0x80 && !state->RAX)
        {
            //Signal::ReturnFromSignal(ct, state);
            handled = true;
        }
    }*/

    Handler *handler = Handlers[state->InterruptNumber];
    for(; !handled && handler && handler->Callback; handler = handler->Next)
        handled = handler->Callback(state, handler->Context);
    if(!handled && isIRQ)
        IRQs::HandleSpurious(irq); // unhandled IRQs are treated as spurious
    if(!handled && state->InterruptNumber != 0x80)
    {
        // print some info about what happened
        if(isIRQ)
            DEBUG("Unhandled IRQ %d (interrupt %d)\n", irq, state->InterruptNumber);
        else
        {
            DEBUG("Unhandled %s %d (%s)\n",
                  state->InterruptNumber < IRQs::Base ? "exception" : "interrupt",
                  state->InterruptNumber,
                  state->InterruptNumber < IRQs::Base ? excNames[state->InterruptNumber] : "hardware interrupt");

            if(cp) DEBUG("Process: %d (%s)\n", cp->ID, cp->Name);
            if(ct)
            {
                ++ct->ExcCount;
                DEBUG("Thread: %d (%s)\n", ct->ID, ct->Name);
            }
        }

        // print extra info for PF
        if(state->InterruptNumber == 14)
        {
            DEBUG("%s when %s address %p\n", state->ErrorCode & 1 ? "Page protection violation" : "Page not present",
                  state->ErrorCode & 16 ? "executing code at" : (state->ErrorCode & 2 ? "writing to" : "reading from"),
                  cpuGetCR2());
        }
        if(ct->ExcCount <= 1)
            DumpState(state);

        if(ct && ct->ExcCount > 1)
        {
            DEBUG("Something went wrong when building stack trace. Killing thread.\n");
            Thread::Finalize(ct, 127);
        }

        DEBUG("Stack trace:\n");
        uintptr_t *rbp = (uintptr_t *)state->RBP;
        for(int i = 0; i < 5; ++i)
        {
            uintptr_t rip = rbp[1];
            if(!rip) break;
            rbp = (uintptr_t *)(*rbp);
            DEBUG("%p\n", rip);
        }

        if(ct && ct->ID != 1) Thread::Finalize(ct, 127);
        else cpuSystemHalt(state->InterruptNumber);
    }

    if(isIRQ) IRQs::SendEOI(irq);
}

void Ints::RegisterHandler(uint intNo, Ints::Handler *handler)
{
    if(intNo >= VECTOR_COUNT || !handler)
        return;
    bool ints = cpuDisableInterrupts();
    handler->Next = Handlers[intNo];
    Handlers[intNo] = handler;
    cpuRestoreInterrupts(ints);
}

void Ints::UnRegisterHandler(uint intNo, Ints::Handler *handler)
{
    if(intNo >= VECTOR_COUNT || !handler)
        return;
    bool ints = cpuDisableInterrupts();
    for(Handler *prev = 0, *h = Handlers[intNo]; h; prev = h, h = h->Next)
    {
        if(handler == h)
        {
            if(prev) prev->Next = h->Next;
            else Handlers[intNo] = h->Next;
        }
    }
    cpuRestoreInterrupts(ints);
}

uint Ints::HandlerCount(uint intNo)
{
    if(intNo >= VECTOR_COUNT)
        return 0;
    bool ints = cpuDisableInterrupts();
    uint res = 0;
    for(Handler *h = Handlers[intNo]; h; h = h->Next, ++res);
    cpuRestoreInterrupts(ints);
    return res;
}

void Ints::DumpState(Ints::State *state)
{
    DEBUG("RAX: %.16X RBX: %.16X\n", state->RAX, state->RBX);
    DEBUG("RCX: %.16X RDX: %.16X\n", state->RCX, state->RDX);
    DEBUG("RSI: %.16X RDI: %.16X\n", state->RSI, state->RDI);
    DEBUG("RSP: %.16X RBP: %.16X\n", state->RSP, state->RBP);
    DEBUG(" R8: %.16X  R9: %.16X\n", state->R8, state->R9);
    DEBUG("R10: %.16X R11: %.16X\n", state->R10, state->R11);
    DEBUG("R12: %.16X R13: %.16X\n", state->R12, state->R13);
    DEBUG("R14: %.16X R15: %.16X\n", state->R14, state->R15);
    DEBUG("RFL: %.16X ERR: %.16x\n", state->RFLAGS, state->ErrorCode);
    DEBUG("CR0: %.16X CR2: %.16X\n", cpuGetCR0(), cpuGetCR2());
    DEBUG("CR3: %.16X CR4: %.16X\n", cpuGetCR3(), cpuGetCR4());
    DEBUG(" CS: %.4x    DS: %.4x  ES: %.4x    FS: %.4x\n", state->CS, state->DS, state->ES, state->FS);
    DEBUG(" GS: %.4x    SS: %.4x RIP: %.16X\n", state->GS, state->SS, state->RIP);
}
