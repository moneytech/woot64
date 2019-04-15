#include <syscalls/syscalls.h>
#include <woot/thread.h>

int threadCreate(const char *name, void *entry, uintptr_t arg, int *retVal)
{
    return sysThreadCreate(name, entry, arg, retVal);
}

int threadDelete(int handle)
{
    return sysThreadDelete(handle);
}

int threadResume(int handle)
{
    return sysThreadResume(handle);
}

int threadSuspend(int handle)
{
    return sysThreadSuspend(handle);
}

int threadSleep(int handle, int ms)
{
    return sysThreadSleep(handle, ms);
}

int threadWait(int handle, int timeout)
{
    return sysThreadWait(handle, timeout);
}

int threadAbort(int handle, int retVal)
{
    return sysThreadAbort(handle, retVal);
}

int threadDaemonize()
{
    return sysThreadDaemonize();
}

int threadGetId(int handle)
{
    return sysThreadGetId(handle);
}
