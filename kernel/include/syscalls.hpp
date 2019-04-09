#pragma once

#include <types.h>

#define SYS_EXIT_THREAD     0x200
#define SYS_EXIT_PROCESS    0x201

class SysCalls
{
    typedef intn (*SysCallHandler)(...);

    static SysCallHandler Handlers[1024];
    static intn InvalidHandler(uintn args[6]);

    static intn sysExitThread(intn retVal);
    static intn sysExitProcess(intn retVal);
public:
    static void Initialize();
};
