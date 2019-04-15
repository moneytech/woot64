#include <syscalls/syscalls.h>
#include <woot/process.h>

int processCreate(const char *cmdline)
{
    return sysProcessCreate(cmdline);
}

int processDelete(int handle)
{
    return sysProcessDelete(handle);
}

int processWait(int handle, int timeout)
{
    return sysProcessWait(handle, timeout);
}

int processAbort(int handle, int result)
{
    return sysProcessAbort(handle, result);
}
