#pragma once

#include <ints.hpp>
#include <queue.hpp>
#include <sequencer.hpp>
#include <signal.hpp>
#include <types.h>

#pragma pack(push, 1)
struct TSS
{
    uint32_t Reserved1;
    uint64_t RSP[3];
    uint64_t Reserved2;
    uint64_t IST[7];
    uint64_t Reserved3;
    uint16_t Reserved4;
    uint16_t IOBMAddr;
};
#pragma pack(pop)

class Mutex;
class Process;
class Semaphore;

class Thread
{
    typedef Thread *ThreadQueue;

    static Sequencer<pid_t> id;
    static Thread *currentThread;
    static Thread *idleThread;
    static ThreadQueue readyThreads;
    static ThreadQueue suspendedThreads;
    static ThreadQueue sleepingThreads;
    static Thread *lastVectorStateThread;
    static Ints::Handler nmInterruptHandler;

    Thread *next = nullptr;

    static bool nmInterrupt(Ints::State *state, void *context);
    static bool threadQueueContains(ThreadQueue *queue, Thread *thread);
    static bool threadQueueRemove(ThreadQueue *queue, Thread *thread);
    static void threadQueueAppend(ThreadQueue *queue, Thread *thread);
    static void threadQueuePrepend(ThreadQueue *queue, Thread *thread);
    static Thread *threadQueueRead(ThreadQueue *queue);

    void kernelPush(uintptr_t value);
    void freeStack(uintptr_t stack, size_t size);
public:
    enum class State
    {
        Unknown = 0,
        Active,
        Ready,
        Suspending,
        Suspended,
        Sleeping,
        Finalized
    };

    static const char *StateNames[];

    // thread info
    pid_t Id;
    char *Name;
    ::Process *Process;
    void *EntryPoint;
    uintptr_t Argument;
    void *UserEntryPoint;
    uintptr_t UserArgument;

    // state
    State State;
    int ErrNo;
    int ExcCount;
    uintptr_t FS, GS;

    // stacks
    size_t KernelStackSize;
    uint8_t *KernelStack;
    size_t UserStackSize;
    uint8_t *UserStack;
    uintptr_t StackPointer;

    // sleeping
    int SleepTicks;
    bool InterruptibleSleep;
    bool SelfSleep;

    // floating point stuff
    uint8_t *FXSaveData;

    // signals
    uint128_t SignalMask;
    Queue<uint8_t> SignalQueue;
    Ints::State SavedMachineState;
    int CurrentSignal;
    uintptr_t SignalRetAddr;
    void *SignalHandlers[SIGNAL_COUNT];

    // finalize stuff
    int *ReturnCodePtr;
    Semaphore *Finished;
    bool DeleteFinished;

    // locking
    Mutex *WaitingMutex;
    Semaphore *WaitingSemaphore;
    int WakeCount; // used to avoid lost wakeup problem

    // threads
    int *tidPtr;
    Semaphore *Initialized; // signaled when thread initialized its usermode stuff

    static void Initialize();
    static Thread *GetIdleThread();
    static void Finalize(Thread *thread, int returnValue);

    Thread(const char *name, class Process *process, void *entryPoint, uintptr_t argument, size_t kernelStackSize, size_t userStackSize, int *returnCodePtr, Semaphore *finished);    
    static Thread *GetByID(pid_t id);
    static bool Exists(Thread *thread);
    static Thread *GetNext(bool doTick);
    static void Switch(Ints::State *state, Thread *thread);
    static Thread *GetCurrent();
    void Enable();
    void Yield();
    void Suspend();
    bool Resume(bool prepend);
    bool QuickResume(Ints::State *state);
    uint TicksSleep(uint ticks, bool interruptible);
    uint Sleep(uint millis, bool interruptible);
    uintptr_t AllocStack(uint8_t **stackAddr, size_t size);
    ~Thread();
};
