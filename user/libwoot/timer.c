#include <errno.h>
#include <stdlib.h>
#include <woot/rpc.h>
#include <woot/timer.h>

#define DEFAULT_RPC_TIMEOUT 1000

static char timerServer[MSG_RPC_PAYLOAD_SIZE] = { 0 };

struct timerCreateArgs
{
    int timeout;
    int flags;
};

struct timerSetTimeoutArgs
{
    int timer;
    int timeout;
};

struct timerSetFlagsArgs
{
    int timer;
    int flags;
};

int timerInitialize()
{
    return rpcFindServer("timekeeper", timerServer, sizeof(timerServer), DEFAULT_RPC_TIMEOUT);
}

int timerCleanup()
{
    timerServer[0] = 0;
    return 0;
}

const char *timerGetServer()
{
    return timerServer[0] ? timerServer : NULL;
}

int timerCreate(int timeout, int flags)
{
    struct timerCreateArgs args = { timeout, flags };
    int id = -ENOSYS;
    int res = rpcCall(timerServer, __FUNCTION__, &args, sizeof(args), &id, sizeof(id), DEFAULT_RPC_TIMEOUT);
    if(res < 0) return res;
    return id;
}

int timerDelete(int timer)
{
    return rpcCall(timerServer, __FUNCTION__, &timer, sizeof(timer), NULL, 0, 0);
}

int timerStart(int timer)
{
    return rpcCall(timerServer, __FUNCTION__, &timer, sizeof(timer), NULL, 0, 0);
}

int timerStop(int timer)
{
    return rpcCall(timerServer, __FUNCTION__, &timer, sizeof(timer), NULL, 0, DEFAULT_RPC_TIMEOUT);
}

int timerPause(int timer)
{
    return rpcCall(timerServer, __FUNCTION__, &timer, sizeof(timer), NULL, 0, DEFAULT_RPC_TIMEOUT);
}

int timerResume(int timer)
{
    return rpcCall(timerServer, __FUNCTION__, &timer, sizeof(timer), NULL, 0, 0);
}

int timerReload(int timer)
{
    return rpcCall(timerServer, __FUNCTION__, &timer, sizeof(timer), NULL, 0, 0);
}

int timerSetTimeout(int timer, int timeout)
{
    struct timerSetTimeoutArgs args = { timer, timeout };
    return rpcCall(timerServer, __FUNCTION__, &args, sizeof(args), NULL, 0, DEFAULT_RPC_TIMEOUT);
}

int timerGetTimeout(int timer)
{
    int timeout = -ENOSYS;
    int res = rpcCall(timerServer, __FUNCTION__, &timer, sizeof(timer), &timeout, sizeof(timeout), DEFAULT_RPC_TIMEOUT);
    return res < 0 ? res : timeout;
}

int timerSetFlags(int timer, int flags)
{
    struct timerSetFlagsArgs args = { timer, flags };
    return rpcCall(timerServer, __FUNCTION__, &args, sizeof(args), NULL, 0, DEFAULT_RPC_TIMEOUT);
}

int timerGetFlags(int timer)
{
    int flags = -ENOSYS;
    int res = rpcCall(timerServer, __FUNCTION__, &timer, sizeof(timer), &flags, sizeof(flags), DEFAULT_RPC_TIMEOUT);
    return res < 0 ? res : flags;
}
