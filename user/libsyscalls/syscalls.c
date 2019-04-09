#include <syscalls.h>

void sysExitThread(long result)
{
    __syscall1(SYS_EXIT_THREAD, result);
}

void sysExitProcess(long result)
{
    __syscall1(SYS_EXIT_PROCESS, result);
}
