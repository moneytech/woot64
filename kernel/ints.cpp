#include <cpu.hpp>
#include <debug.hpp>
#include <ints.hpp>

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

class Process;
class Thread;

void Ints::CommonHandler(Ints::State *state)
{
    //int irq = state->InterruptNumber - IRQs::Base;
    //bool isIrq = irq >= 0 && irq < IRQs::Count;
    Process *cp = nullptr;//Process::GetCurrent();
    Thread *ct = nullptr;//Thread::GetCurrent();

    // handle spurious irqs
    /*if(isIrq)
    {
        if(IRQs::IsSpurious(irq))
        {
            IRQs::HandleSpurious(irq);
            return;
        }
    }*/

    bool isIrq = false;
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
    //if(!handled && isIrq)
    //    IRQs::HandleSpurious(irq);
    /*else */if(!handled && state->InterruptNumber != 0x80)
    {
        // print some info about what happened
        /*if(isIrq)
            DEBUG("Unhandled IRQ %d (interrupt %d)\n", irq, state->InterruptNumber);
        else
        {
            DEBUG("Unhandled %s %d (%s)\n",
                  state->InterruptNumber < IRQS_BASE ? "exception" : "interrupt",
                  state->InterruptNumber,
                  state->InterruptNumber < IRQS_BASE ? excNames[state->InterruptNumber] : "hardware interrupt");

            if(cp) DEBUG("Process: %d (%s)\n", cp->ID, cp->Name);
            if(ct)
            {
                ++ct->ExcCount;
                DEBUG("Thread: %d (%s)\n", ct->ID, ct->Name);
            }
        }*/

        // print extra info for PF
        if(state->InterruptNumber == 14)
        {
            DEBUG("%s when %s address %#.8x\n", state->ErrorCode & 1 ? "Page protection violation" : "Page not present",
                  state->ErrorCode & 16 ? "executing code at" : (state->ErrorCode & 2 ? "writing to" : "reading from"),
                  cpuGetCR2());
        }
        DumpState(state);

        /*if(ct && ct->ExcCount > 1)
        {
            DEBUG("Something went wrong when building stack trace. Killing thread.\n");
            Thread::Finalize(ct, 127);
        }*/

        DEBUG("Stack trace:\n");
        uintptr_t *rbp = (uintptr_t *)state->RBP;
        for(int i = 0; i < 5; ++i)
        {
            uintptr_t rip = rbp[1];
            if(!rip) break;
            rbp = (uintptr_t *)(*rbp);
            DEBUG("%p\n", rip);
        }

        _outsb("except", 0xE9, 6);
        /*if(ct && ct->ID != 1)
            Thread::Finalize(ct, 127);
        else cpuSystemHalt(state->InterruptNumber);*/
    }

    //if(isIrq) IRQs::SendEOI(irq);
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
    DEBUG("RAX: %.16X RBX: %.16X\nRCX: %.16X EDX: %.16X\n",
          state->RAX, state->RBX, state->RCX, state->RDX);
    DEBUG("RSI: %.16X RDI: %.16X\nRSP: %.16X RBP: %.16X\n",
          state->RSI, state->RDI, state->RSP, state->RBP);
    DEBUG("CS: %.4X RIP: %.16X\nEFLAGS: %.16X ErrorCode: %.16x\n",
          state->CS, state->RIP, state->RFLAGS, state->ErrorCode);
    DEBUG("DS: %.4X ES: %.4X FS: %.4X GS: %.4X SS: %.4X\n",
          state->DS, state->ES, state->FS, state->GS, state->SS);
    DEBUG("CR0: %.16X\nCR2: %.16X\nCR3: %.8X\n",
          cpuGetCR0(), cpuGetCR2(), cpuGetCR3());
}
