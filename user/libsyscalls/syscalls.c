#include <syscalls/syscalls.h>

size_t sys_read(int fd, char *buf, size_t count)
{
    return __syscall3(SYS_read, fd, (long)buf, count);
}

size_t sys_write(int fd, const char *buf, size_t count)
{
    return __syscall3(SYS_write, fd, (long)buf, count);
}

int sys_open(const char *filename, int flags, int mode)
{
    return __syscall3(SYS_open, (long)filename, flags, mode);
}

int sys_close(int fd)
{
    return __syscall1(SYS_open, fd);
}

off_t sys_lseek(int fd, off_t offset, unsigned int origin)
{
    return __syscall3(SYS_lseek, fd, offset, origin);
}

long sys_exit(long error_code)
{
    return __syscall1(SYS_exit, error_code);
}

void *sys_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off)
{
    return (void *)__syscall6(SYS_mmap, (long)addr, len, prot, flags, fd, off);
}

int sys_munmap(void *addr, size_t len)
{
    return __syscall2(SYS_mmap, (long)addr, len);
}

void *sys_brk(void *brk)
{
    return (void *)__syscall1(SYS_brk, (long)brk);
}
