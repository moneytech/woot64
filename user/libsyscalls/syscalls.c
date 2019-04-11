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

int sys_stat(const char *filename, void *statbuf)
{
    return __syscall2(SYS_stat, (long)filename, (long)statbuf);
}

int sys_fstat(int fd, void *statbuf)
{
    return __syscall2(SYS_fstat, fd, (long)statbuf);
}

off_t sys_lseek(int fd, off_t offset, unsigned int origin)
{
    return __syscall3(SYS_lseek, fd, offset, origin);
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

long sys_readv(int fd, const void *vec, size_t vlen)
{
    return __syscall3(SYS_readv, fd, (long)vec, vlen);
}

long sys_writev(int fd, const void *vec, size_t vlen)
{
    return __syscall3(SYS_writev, fd, (long)vec, vlen);
}

long sys_exit(long error_code)
{
    return __syscall1(SYS_exit, error_code);
}

int sys_arch_prctl(int code, uintptr_t addr)
{
    return __syscall2(SYS_arch_prctl, code, addr);
}

int sys_set_tid_address(int *tidptr)
{
    return __syscall1(SYS_set_tid_address, (long)tidptr);
}

long sys_exit_group(long error_code)
{
    return __syscall1(SYS_exit_group, error_code);
}
