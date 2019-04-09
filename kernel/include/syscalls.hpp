#pragma once

#include <types.h>

// linux compatible syscalls
#define SYS_read            0
#define SYS_write           1
#define SYS_open            2
#define SYS_close           3
#define SYS_exit            60

// woot specific syscalls
#define SYS_EXIT_THREAD     0x200
#define SYS_EXIT_PROCESS    0x201

class SysCalls
{
    typedef intn (*SysCallHandler)(...);

    static SysCallHandler Handlers[1024];
    static intn InvalidHandler();

    static intn sys_read(unsigned int fd, char *buf, size_t count);
    static intn sys_write(unsigned int fd, const char *buf, size_t count);
    static intn sys_open(const char *filename, int flags, int mode);
    static intn sys_close(unsigned int fd);
    static intn sys_exit(intn retVal);

    static intn sysExitThread(intn retVal);
    static intn sysExitProcess(intn retVal);
public:
    static void Initialize();
};
