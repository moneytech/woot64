#include <cpu.hpp>
#include <debug.hpp>
#include <irqs.hpp>
#include <misc.hpp>
#include <mutex.hpp>
#include <new.hpp>
#include <paging.hpp>
#include <process.hpp>
#include <semaphore.hpp>
#include <string.hpp>
#include <sysdefs.h>
#include <thread.hpp>
#include <time.hpp>

extern "C" void *kmain;
extern "C" uint8_t mainKernelThreadStack[];
extern "C" uint8_t mainKernelThreadStackEnd[];
extern "C" struct TSS mainTSS;

extern "C" void threadFinalize(Thread *thread, int returnValue)
{
    Thread::Finalize(thread, returnValue);
}

extern "C" void threadReturn(void);

// That asm here is a little bit meh
asm(
INLINE_ASM_SYNTAX
"threadReturn:\n"
"   mov rsi, rax\n"
"   pop rdi\n"
"   call threadFinalize\n"
NORMAL_ASM_SYNTAX
);

static void idleThreadProc()
{
    for(;;) cpuWaitForInterrupt(0x1D1E1D1E);
}

Sequencer<pid_t> Thread::id(1);
Thread *Thread::currentThread = nullptr;
Thread *Thread::idleThread = nullptr;
Thread::ThreadQueue Thread::readyThreads = nullptr;
Thread::ThreadQueue Thread::suspendedThreads = nullptr;
Thread::ThreadQueue Thread::sleepingThreads = nullptr;
Thread *Thread::lastVectorStateThread = nullptr;
Ints::Handler Thread::nmInterruptHandler = { nullptr, Thread::nmInterrupt, nullptr };

const char *Thread::StateNames[] =
{
    "Unknown",
    "Active",
    "Ready",
    "Suspending",
    "Suspended",
    "Sleeping",
    "Finalized"
};

bool Thread::nmInterrupt(Ints::State *state, void *context)
{
    cpuSetCR0(cpuGetCR0() & ~0x08);
    if(lastVectorStateThread && lastVectorStateThread->FXSaveData)
        cpuFXSave(lastVectorStateThread->FXSaveData);
    Thread *ct = currentThread;
    if(!ct || !ct->FXSaveData)
        return false;
    cpuFXRstor(ct->FXSaveData);
    lastVectorStateThread = ct;
    return true;
}

bool Thread::threadQueueContains(ThreadQueue *queue, Thread *thread)
{
    for(Thread *t = *queue; t; t = t->next)
    {
        if(t == thread)
            return true;
    }
    return false;
}

bool Thread::threadQueueRemove(ThreadQueue *queue, Thread *thread)
{
    for(Thread *prev = nullptr, *t = *queue; t; prev = t, t = t->next)
    {
        if(t == thread)
        {
            if(prev) prev->next = t->next;
            else *queue = t->next;
            t->next = nullptr;
            return true;
        }
    }
    return false;
}

void Thread::threadQueueAppend(ThreadQueue *queue, Thread *thread)
{
    Thread *last = *queue;
    if(!last)
    {
        *queue = thread;
        return;
    }
    for(; last->next; last = last->next)
    {
        if(last == thread)
        {
            DEBUG("[thread] %s(): Thread %d already on queue.", __FUNCTION__, thread->Id);
            return;
        }
    }
    last->next = thread;
}

void Thread::threadQueuePrepend(Thread::ThreadQueue *queue, Thread *thread)
{
    thread->next = *queue;
    *queue = thread;
}

Thread *Thread::threadQueueRead(Thread::ThreadQueue *queue)
{
    Thread *first = *queue;
    if(first)
    {
        *queue = first->next;
        first->next = nullptr;
    }
    return first;
}

void Thread::kernelPush(uintptr_t value)
{
    *(uintptr_t *)(StackPointer -= 8) = value;
}

void Thread::freeStack(uintptr_t stack, size_t size)
{
    if(!Process) return;
    uintptr_t as = Process->AddressSpace;
    size_t pageCount = size >> PAGE_SHIFT;
    for(uint i = 0; i < pageCount; ++i)
    {
        uintptr_t va = (i << PAGE_SHIFT) + stack;
        uintptr_t pa = Paging::GetPhysicalAddress(as, va);
        if(pa == ~0) continue;
        Paging::UnMapPage(as, va);
        Paging::FreeFrame(pa);
    }
}

void Thread::Initialize()
{
    bool ints = cpuDisableInterrupts();

    Thread *mainThread = new Thread("main kernel thread", nullptr, kmain, 0, ~0, 0, nullptr, nullptr);
    mainThread->KernelStack = mainKernelThreadStack;
    mainThread->KernelStackSize = (uintptr_t)mainKernelThreadStackEnd - (uintptr_t)mainKernelThreadStack;
    currentThread = mainThread;

    idleThread = new Thread("idle thread", nullptr, (void *)idleThreadProc, 0, 0, 0, nullptr, nullptr);
    idleThread->State = State::Ready;

    lastVectorStateThread = currentThread;
    Ints::RegisterHandler(7, &nmInterruptHandler);

    cpuRestoreInterrupts(ints);
}

Thread *Thread::GetIdleThread()
{
    return idleThread;
}

void Thread::Finalize(Thread *thread, int returnValue)
{
    bool is = cpuDisableInterrupts();
    if(!thread) thread = currentThread;
    if(thread->ReturnCodePtr)
        *thread->ReturnCodePtr = returnValue;
    if(thread->Finished)
        thread->Finished->Signal(nullptr);
    thread->State = State::Finalized;
    if(thread->WaitingMutex)
        thread->WaitingMutex->Cancel(thread);
    if(thread->WaitingSemaphore)
        thread->WaitingSemaphore->Cancel(thread);
    threadQueueRemove(&readyThreads, thread);
    threadQueueRemove(&suspendedThreads, thread);
    threadQueueRemove(&sleepingThreads, thread);
    if(lastVectorStateThread == thread)
        lastVectorStateThread = nullptr;
    bool self = currentThread == thread;

    // BUGBUG: we might be using deallocated stack right now
    //         maybe using asm here would be a better idea

    if(self)
    {
        currentThread = nullptr;
        Time::FakeTick();
    }
    cpuRestoreInterrupts(is);
}

Thread::Thread(const char *name, class Process *process, void *entryPoint, uintptr_t argument, size_t kernelStackSize, size_t userStackSize, int *returnCodePtr, Semaphore *finished) :
    Id(id.GetNext()),
    Name(String::Duplicate(name)),
    Process(process),
    EntryPoint(entryPoint),
    Argument(argument),
    State(State::Unknown),
    KernelStackSize(kernelStackSize ? kernelStackSize : DEFAULT_STACK_SIZE),
    KernelStack(kernelStackSize == PG_INVALID_ADDRESS ? nullptr : new (PAGE_SIZE) uint8_t[KernelStackSize]),
    UserStackSize(userStackSize ? userStackSize : DEFAULT_USER_STACK_SIZE),
    UserStack(nullptr),
    StackPointer(KernelStackSize + (uintptr_t)KernelStack),
    SleepTicks(0),
    InterruptibleSleep(false),
    SelfSleep(false),
    FXSaveData(new(16) uint8_t[512]),
    SignalMask(0),
    SignalQueue(64),
    CurrentSignal(-1),
    ReturnCodePtr(returnCodePtr),
    Finished(finished ? finished : new Semaphore(0)),
    DeleteFinished(!finished),
    WaitingMutex(nullptr),
    WaitingSemaphore(nullptr),
    WakeCount(0),
    Initialized(new Semaphore(0, "Thread::Initizlized"))
{
    if(!Process) Process = Process::GetCurrent();
    else process->AddThread(this);

    cpuFXSave(FXSaveData);                  // FIXME: should be initialized to known good state

    if(kernelStackSize == PG_INVALID_ADDRESS)
        return;

    // initialize stack
    kernelPush(0x0000000000000000);         // stack alignment (for SSE)
    kernelPush((uintptr_t)this);            // for threadReturn
    kernelPush((uintptr_t)threadReturn);    // return address
    uintptr_t entryStackPointer = StackPointer;

    // this stack layout here MUST match interrupt stack defined in ints.h
    kernelPush(SEG_KERNEL_DATA);            // SS
    kernelPush(entryStackPointer);          // RSP
    kernelPush(0x0000000000000202);         // RFLAGS
    kernelPush(SEG_KERNEL_CODE);            // CS
    kernelPush((uintptr_t)entryPoint);      // RIP

    kernelPush(0x0000000000000000);         // error code
    kernelPush(0x0000000000000000);         // interrupt number

    kernelPush(0x0000000000000000);         // RAX
    kernelPush(0x0000000000000000);         // RCX
    kernelPush(0x0000000000000000);         // RDX
    kernelPush(0x0000000000000000);         // RBX
    kernelPush(0x0000000000000000);         // RBP
    kernelPush(0x0000000000000000);         // RSI
    kernelPush(argument);                   // RDI
    kernelPush(0x0000000000000000);         // R8
    kernelPush(0x0000000000000000);         // R9
    kernelPush(0x0000000000000000);         // R10
    kernelPush(0x0000000000000000);         // R11
    kernelPush(0x0000000000000000);         // R12
    kernelPush(0x0000000000000000);         // R13
    kernelPush(0x0000000000000000);         // R14
    kernelPush(0x0000000000000000);         // R15

    kernelPush(SEG_KERNEL_DATA);            // DS
    kernelPush(SEG_KERNEL_DATA);            // ES
    kernelPush(SEG_KERNEL_DATA);            // FS
    kernelPush(SEG_KERNEL_DATA);            // GS
}

Thread *Thread::GetByID(pid_t id)
{
    bool ints = cpuDisableInterrupts();
    Thread *res = nullptr;
    if(currentThread->Id == id || id == -1)
    {
        res = currentThread;
        cpuRestoreInterrupts(ints);
        return res;
    }

    for(Thread *t = (Thread *)readyThreads; t; t = (Thread *)t->next)
    {
        if(t->Id == id)
        {
            cpuRestoreInterrupts(ints);
            return t;
        }
    }
    for(Thread *t = (Thread *)suspendedThreads; t; t = (Thread *)t->next)
    {
        if(t->Id == id)
        {
            cpuRestoreInterrupts(ints);
            return t;
        }
    }
    for(Thread *t = (Thread *)sleepingThreads; t; t = (Thread *)t->next)
    {
        if(t->Id == id)
        {
            cpuRestoreInterrupts(ints);
            return t;
        }
    }

    return nullptr;
}

bool Thread::Exists(Thread *thread)
{
    if(thread == GetCurrent())
        return true;
    if(threadQueueContains(&readyThreads, thread))
        return true;
    if(threadQueueContains(&suspendedThreads, thread))
        return true;
    if(threadQueueContains(&sleepingThreads, thread))
        return true;
    return false;
}

Thread *Thread::GetNext(bool doTick)
{
    // handle sleeping threads
    if(doTick)
    {
        for(Thread *t = sleepingThreads; t; t = t->next)
        {
            if(t->SleepTicks > 0)
                --t->SleepTicks;
        }
    }
    for(Thread *t = sleepingThreads; t; t = t->next)
    {
        if(!t->SleepTicks)
        {
            threadQueueRemove(&sleepingThreads, t);
            t->State = State::Ready;
            t->InterruptibleSleep = false;
            if(t->SelfSleep)
            {
                uintptr_t *RAX = (uintptr_t *)(t->StackPointer + offsetof(Ints::State, RAX) - 8);
                *RAX = t->SleepTicks;
            }
            t->SelfSleep = false;
            threadQueueAppend(&readyThreads, t);
            break;
        }
    }

    // get next thread from queue
    if(currentThread && currentThread != idleThread &&
            currentThread->State != State::Sleeping)
    {
        if(currentThread->State != State::Suspending)
        {
            currentThread->State = State::Ready;
            threadQueueAppend(&readyThreads, currentThread);
        }
        else
        {
            currentThread->State = State::Suspended;
            threadQueueAppend(&suspendedThreads, currentThread);
        }
    }
    Thread *t = (Thread *)threadQueueRead(&readyThreads);
    t = t ? t : idleThread;
    return t;
}

void Thread::Switch(Ints::State *state, Thread *thread)
{
    if(currentThread == thread)
        return; // nothing to be done here

    if(currentThread)
    {
        currentThread->StackPointer = state->SwitchRSP;
        //currentThread->State = State::Ready;
        currentThread->FS = cpuReadMSR(0xC0000100);
        currentThread->GS = cpuReadMSR(0xC0000101);
    }

    state->SwitchRSP = thread->StackPointer;

    uintptr_t kernelRSP = thread->KernelStackSize + (uintptr_t)thread->KernelStack;
    mainTSS.RSP[0] = kernelRSP; // without that stack overflow happens
    cpuWriteMSR(0xC0000100, thread->FS);
    cpuWriteMSR(0xC0000101, thread->GS);

    cpuSetCR0(cpuGetCR0() | 0x08); // set TS bit
    if(thread->Process)
    {
        uintptr_t _cr3 = cpuGetCR3();
        uintptr_t newCr3 = thread->Process->AddressSpace;
        if(_cr3 != newCr3) // avoid unnecesary tlb flushing
            cpuSetCR3(newCr3);
    }

    currentThread = thread;
    currentThread->State = State::Active;
}

Thread *Thread::GetCurrent()
{
    bool ints = cpuDisableInterrupts();
    Thread *res = currentThread;
    cpuRestoreInterrupts(ints);
    return res;
}

void Thread::Enable()
{
    bool ints = cpuDisableInterrupts();
    threadQueueAppend(&suspendedThreads, this);
    this->State = State::Suspended;
    cpuRestoreInterrupts(ints);
}

void Thread::Yield()
{
    bool ints = cpuDisableInterrupts();
    Time::FakeTick();
    cpuRestoreInterrupts(ints);
}

void Thread::Suspend()
{
    bool ints = cpuDisableInterrupts();
    if(WakeCount) --WakeCount;
    if(WakeCount)
    {   // ignore suspend if WakeCount != 0
        cpuRestoreInterrupts(ints);
        return;
    }
    if(this == currentThread)
    {
        currentThread->State = State::Suspending;
        Time::FakeTick();
        cpuRestoreInterrupts(ints);
        return;
    }
    if(!threadQueueRemove(&readyThreads, this) && !threadQueueRemove(&sleepingThreads, this))
    {
        cpuRestoreInterrupts(ints);
        return;
    }
    State = State::Suspended;
    threadQueueAppend(&suspendedThreads, this);
    cpuRestoreInterrupts(ints);
}

bool Thread::Resume(bool prepend)
{
    bool ints = cpuDisableInterrupts();
    ++WakeCount;
    if(threadQueueRemove(&suspendedThreads, this) || (InterruptibleSleep && threadQueueRemove(&sleepingThreads, this)))
    {
        if(State == State::Sleeping)
        {
            if(SelfSleep)
            {
                uintptr_t *RAX = (uintptr_t *)(StackPointer + offsetof(Ints::State, RAX) - 8);
                *RAX = SleepTicks;
            }
            SelfSleep = false;
            SleepTicks = 0;
        }
        State = State::Ready;
        if(prepend) threadQueuePrepend(&readyThreads, this);
        else threadQueueAppend(&readyThreads, this);
        cpuRestoreInterrupts(ints);
        return true;
    }
    cpuRestoreInterrupts(ints);
    return false;
}

bool Thread::QuickResume(Ints::State *state)
{
    bool ints = cpuDisableInterrupts();
    if(!Resume(true)) return false;
    Switch(state, GetNext(false));
    cpuRestoreInterrupts(ints);
    return true;
}

uint Thread::TicksSleep(uint ticks, bool interruptible)
{
    bool ints = cpuDisableInterrupts();
    if(interruptible)
    {
        if(WakeCount) --WakeCount;
        if(WakeCount)
        {   // ignore sleep if WakeCount != 0
            cpuRestoreInterrupts(ints);
            return ticks;
        }
    }
    threadQueueRemove(&readyThreads, this);
    threadQueueRemove(&suspendedThreads, this);
    threadQueueRemove(&sleepingThreads, this);
    State = State::Sleeping;
    SleepTicks = ticks;
    InterruptibleSleep = interruptible;
    threadQueueAppend(&sleepingThreads, this);
    uint result = 0;
    if(currentThread == this)
    {
        Time::isFakeTick = true;
        SelfSleep = true;
        result = Time::FakeTick();
    }
    cpuRestoreInterrupts(ints);
    return result;
}

uint Thread::Sleep(uint millis, bool interruptible)
{
    uint64_t tickFreq = Time::GetTickFrequency();
    uint ticks = ((tickFreq * millis) / 1000) + 1; // +1 make sure it's at
                                                   // least as long as
                                                   // specified (may be longer)
    uint64_t nanosPerTick = max(1, 1000000000 / tickFreq);
    uint ticksLeft = TicksSleep(ticks, interruptible);
    return (ticksLeft * nanosPerTick) / 1000000;
}

uintptr_t Thread::AllocStack(uint8_t **stackAddr, size_t size)
{
    if(!size) return INVALID_POINTER;
    uintptr_t as = Process->AddressSpace;
    uintptr_t res = Process->UserStackPtr;
    size_t pageCount = align(size, PAGE_SIZE) / PAGE_SIZE;
    uintptr_t startPtr = align(res, PAGE_SIZE) - pageCount * PAGE_SIZE;
    *stackAddr = reinterpret_cast<uint8_t *>(startPtr);
    for(uint i = 0; i < pageCount; ++i)
    {
        uintptr_t pa = Paging::AllocFrame();
        if(pa == INVALID_POINTER)
        {
            freeStack(reinterpret_cast<uintptr_t>(*stackAddr), size);
            return INVALID_POINTER;
        }
        if(!Paging::MapPage(as, startPtr + i * PAGE_SIZE, pa, true, true, false))
        {
            freeStack(reinterpret_cast<uintptr_t>(*stackAddr), size);
            return INVALID_POINTER;
        }
    }
    Process->UserStackPtr = reinterpret_cast<uintptr_t>(*stackAddr);
    return res;
}

Thread::~Thread()
{
    if(Initialized) delete Initialized;
    if(Name) delete[] Name;
    if(KernelStack) delete[] KernelStack;
    if(UserStack)
    {   // we have user stack
        freeStack(reinterpret_cast<uintptr_t>(UserStack), UserStackSize);
        UserStack = nullptr;
    }
    if(FXSaveData) delete[] FXSaveData;
    if(DeleteFinished && Finished) delete Finished;
}
