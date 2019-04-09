#include <syscalls/syscalls.h>

size_t sys_read(unsigned int fd, char *buf, size_t count)
{
    return __syscall3(SYS_read, fd, (long)buf, count);
}

size_t sys_write(unsigned int fd, const char *buf, size_t count)
{
    return __syscall3(SYS_write, fd, (long)buf, count);
}

int sys_open(const char *filename, int flags, int mode)
{
    return __syscall3(SYS_open, (long)filename, flags, mode);
}

int sys_close(unsigned int fd)
{
    return __syscall1(SYS_open, fd);
}

long sys_exit(long error_code)
{
    return __syscall1(SYS_exit, error_code);
}

void sysExitThread(long result)
{
    __syscall1(SYS_EXIT_THREAD, result);
}

void sysExitProcess(long result)
{
    __syscall1(SYS_EXIT_PROCESS, result);
}
