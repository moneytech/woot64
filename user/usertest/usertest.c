#include <syscalls.h>

void _start()
{
    for(int i = 0; i < 10; ++i)
        __syscall0(1000);
    sysExitThread(123);
}
