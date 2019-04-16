#include <debug.hpp>
#include <mutex.hpp>
#include <thread.hpp>
#include <time.hpp>

// TODO: make waiters queue arbitrarily long without any heap allocations
#define MAX_WAITERS 32

Mutex::Mutex(bool recursive, const char *name) :
    Recursive(recursive), Count(0), Owner(nullptr),
    Waiters(new Queue<Thread *>(MAX_WAITERS)),
    Name(name)
{
}

bool Mutex::Acquire(uint timeout, bool tryAcquire)
{
    bool is = cpuDisableInterrupts();
    Thread *ct = Thread::GetCurrent();
    if(!ct)
    {   // just return if threads are not yet initialized
        cpuRestoreInterrupts(is);
        return true;
    }
    if(!Count || Owner == ct)
    {
        if(tryAcquire)
        {
            cpuRestoreInterrupts(is);
            return false;
        }
        if(!Recursive && Count)
        {
            DEBUG("[mutex] Multiple locks on non-recursive mutex %s!\n", Name);
            cpuSystemHalt(0xBADC0DE2);
        }
        ++Count;
        Owner = ct;
        cpuRestoreInterrupts(is);
        return true;
    }
    if(tryAcquire)
    {
        cpuRestoreInterrupts(is);
        return false;
    }
    if(Waiters->Write(ct))
    {
        bool success = true;

        ct->WaitingMutex = this;
        if(timeout) success = ct->Sleep(timeout, true) != 0;
        else ct->Suspend();
        ct->WaitingMutex = nullptr;

        if(!success) Waiters->RemoveFirst(ct);

        cpuRestoreInterrupts(is);
        return success;
    }
    // if no free waiter slots then print message and fail
    cpuRestoreInterrupts(is);
    DEBUG("!!! Mutex ran out of free waiter slots !!!\n");
    return false;
}

void Mutex::Release()
{
    bool is = cpuDisableInterrupts();
    Thread *ct = Thread::GetCurrent();
    if(!ct)
    {   // just return if threads are not yet initialized
        cpuRestoreInterrupts(is);
        return;
    }
    if(Owner != ct)
    {
        DEBUG("[mutex] Mutex::Release(): '%s' current thread(%d) != Owner(%d)\n", Name, ct->Id, Owner ? Owner->Id : -1);
        cpuRestoreInterrupts(is);
        return;
    }
    if(Count > 0)
        --Count;
    if(!Count)
    {
        Owner = nullptr;
        bool ok;
        Thread *t = Waiters->Read(&ok);
        if(!ok)
        { // no waiting threads in queue
            cpuRestoreInterrupts(is);
            return;
        }

        // make last unqueued thread an owner
        Owner = t;
        Count = 1;
        t->Resume(false);
    }
    cpuRestoreInterrupts(is);
}

void Mutex::Cancel(Thread *t)
{
    bool is = cpuDisableInterrupts();
    Waiters->ReplaceAll(t, nullptr);
    cpuRestoreInterrupts(is);
}

int Mutex::GetCount() const
{
    return Count;
}

Mutex::~Mutex()
{
    if(Waiters) delete Waiters;
}
