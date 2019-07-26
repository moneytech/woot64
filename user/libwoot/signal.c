#include <woot/signal.h>
#include <syscalls/syscalls.h>

void *signalGetHandler(unsigned signum)
{
    return sysSignalGetHandler(signum);
}

int signalSetHandler(unsigned signum, void *handler)
{
    return (int)sysSignalSetHandler(signum, handler);
}

int signalIsEnabled(unsigned signum)
{
    return (int)sysSignalIsEnabled(signum);
}

int signalEnable(unsigned signum)
{
    return (int)sysSignalEnable(signum);
}

int signalDisable(unsigned signum)
{
    return (int)sysSignalDisable(signum);
}

int signalRaise(int tid, unsigned signum)
{
    return (int)sysSignalRaise(tid, signum);
}

int signalReturn(void)
{
    return (int)sysSignalReturn();
}

int signalGetCurrent(void)
{
    return (int)sysSignalGetCurrent();
}

