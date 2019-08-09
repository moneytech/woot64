#include <syscalls/syscalls.h>
#include <woot/process.h>

int processCreate(const char *cmdline)
{
    return (int)sysProcessCreate(cmdline);
}

int processDelete(int handle)
{
    return (int)sysProcessDelete(handle);
}

int processWait(int handle, int timeout)
{
    return (int)sysProcessWait(handle, timeout);
}

int processAbort(int handle, int result)
{
    return (int)sysProcessAbort(handle, result);
}

int processListIds(int *buf, unsigned bufSize)
{
    return (int)sysProcessListIds(buf, bufSize);
}

int processGetName(int pid, char *buf, unsigned bufSize)
{
    return (int)sysProcessGetName(pid, buf, bufSize);
}

int processGetThreadCount(int pid)
{
    return (int)sysProcessGetThreadCount(pid);
}

unsigned long processGetUsedMemory(int pid)
{
    return (unsigned long)sysProcessGetUsedMemory(pid);
}

int processGetExecPath(int pid, char *buf, unsigned bufSize)
{
    return (int)sysProcessGetExecPath(pid, buf, bufSize);
}

int processGetMap(int pid, processMapEntry_t *buf, unsigned bufSize)
{
    return (int)sysProcessGetMap(pid, buf, bufSize);
}

