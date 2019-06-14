#include <syscalls/syscalls.h>
#include <woot/thread.h>

int threadCreate(const char *name, void *entry, uintptr_t arg, int *retVal)
{
    return sysThreadCreate(name, entry, arg, retVal);
}

int threadDelete(int tid)
{
    return sysThreadDelete(tid);
}

int threadResume(int tid)
{
    return sysThreadResume(tid);
}

int threadSuspend(int tid)
{
    return sysThreadSuspend(tid);
}

int threadSleep(int tid, int ms)
{
    return sysThreadSleep(tid, ms);
}

int threadWait(int tid, int timeout)
{
    return sysThreadWait(tid, timeout);
}

int threadAbort(int tid, int retVal)
{
    return sysThreadAbort(tid, retVal);
}

int threadDaemonize()
{
    return sysThreadDaemonize();
}
