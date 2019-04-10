#pragma once

#include <types.h>

// linux compatible syscalls
#define SYS_read            0
#define SYS_write           1
#define SYS_open            2
#define SYS_close           3
#define SYS_lseek           8
#define SYS_mmap            9
#define SYS_munmap          11
#define SYS_brk             12
#define SYS_exit            60
#define SYS_exit_group      231

// woot specific syscalls

class SysCalls
{
    typedef intn (*SysCallHandler)(...);

    static SysCallHandler Handlers[1024];
    static long InvalidHandler();

    static long sys_read(int fd, char *buf, size_t count);
    static long sys_write(int fd, const char *buf, size_t count);
    static long sys_open(const char *filename, int flags, int mode);
    static long sys_close(int fd);
    static long sys_lseek(int fd, off_t offset, unsigned int origin);
    static long sys_mmap(uintptr_t addr, unsigned long len, int prot, int flags, int fd, off_t off);
    static long sys_munmap(uintptr_t addr, size_t len);
    static long sys_brk(uintptr_t brk);
    static long sys_exit(intn retVal);
    static long sys_exit_group(intn retVal);
public:
    static void Initialize();
};
