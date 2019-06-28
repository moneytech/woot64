#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <woot/ipc.h>
#include <woot/rpc.h>
#include <woot/thread.h>

#define PROG_NAME "timekeeper"

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

struct Timer
{
    int id;
    int process;
    int timeout;
    int flags;
    int state;
};

int getNewId()
{
    static int ids = 0;
    return __sync_add_and_fetch(&ids, 1);
}

int createTimer(int timeout, int flags)
{
    return -ENOSYS;
}

int deleteTimer(int timer)
{
    return -ENOSYS;
}

int startTimer(int timer)
{
    return -ENOSYS;
}

int stopTimer(int timer)
{
    return -ENOSYS;
}

int pauseTimer(int timer)
{
    return -ENOSYS;
}

int resumeTimer(int timer)
{
    return -ENOSYS;
}

int reloadTimer(int timer)
{
    return -ENOSYS;
}

int setTimeout(int timer, int timeout)
{
    return -ENOSYS;
}

int getTimeout(int timer)
{
    return -ENOSYS;
}

int setFlags(int timer, int flags)
{
    return -ENOSYS;
}

int getFlags(int timer)
{
    return -ENOSYS;
}

extern "C" int main(int argc, char *argv[])
{
    fprintf(stderr, "[" PROG_NAME "] Time keeper started\n");
    threadDaemonize();
    for(ipcMessage_t msg;;)
    {
        ipcGetMessage(&msg, -1);
        ipcProcessMessage(&msg);
        if(msg.Number == MSG_QUIT)
            break;
        else if(msg.Number == MSG_RPC_REQUEST)
        {
            char *req = (char *)msg.Data;
            char *_args = (char *)(req + strlen(req) + 1);
            if(!strcmp(req, "timerCreate"))
            {
                timerCreateArgs *args = (timerCreateArgs *)_args;
                int id = createTimer(args->timeout, args->flags);
                rpcIPCReturn(msg.Source, msg.ID, &id, sizeof(id));
            }
            else if(!strcmp(req, "timerDelete"))
            {
                int timer = *(int *)_args;
                int resp = deleteTimer(timer);
                rpcIPCReturn(msg.Source, msg.ID, &resp, sizeof(resp));
            }
            else if(!strcmp(req, "timerStart"))
            {
                int timer = *(int *)_args;
                int resp = startTimer(timer);
                rpcIPCReturn(msg.Source, msg.ID, &resp, sizeof(resp));
            }
            else if(!strcmp(req, "timerStop"))
            {
                int timer = *(int *)_args;
                int resp = stopTimer(timer);
                rpcIPCReturn(msg.Source, msg.ID, &resp, sizeof(resp));
            }
            else if(!strcmp(req, "timerPause"))
            {
                int timer = *(int *)_args;
                int resp = pauseTimer(timer);
                rpcIPCReturn(msg.Source, msg.ID, &resp, sizeof(resp));
            }
            else if(!strcmp(req, "timerResume"))
            {
                int timer = *(int *)_args;
                int resp = resumeTimer(timer);
                rpcIPCReturn(msg.Source, msg.ID, &resp, sizeof(resp));
            }
            else if(!strcmp(req, "timerReload"))
            {
                int timer = *(int *)_args;
                int resp = reloadTimer(timer);
                rpcIPCReturn(msg.Source, msg.ID, &resp, sizeof(resp));
            }
            else if(!strcmp(req, "timerSetTimeout"))
            {
                timerSetTimeoutArgs *args = (timerSetTimeoutArgs *)_args;
                int id = setTimeout(args->timer, args->timeout);
                rpcIPCReturn(msg.Source, msg.ID, &id, sizeof(id));
            }
            else if(!strcmp(req, "timerGetTimeout"))
            {
                int timer = *(int *)_args;
                int resp = getTimeout(timer);
                rpcIPCReturn(msg.Source, msg.ID, &resp, sizeof(resp));
            }
            else if(!strcmp(req, "timerSetFlags"))
            {
                timerSetFlagsArgs *args = (timerSetFlagsArgs *)_args;
                int id = setFlags(args->timer, args->flags);
                rpcIPCReturn(msg.Source, msg.ID, &id, sizeof(id));
            }
            else if(!strcmp(req, "timerGetFlags"))
            {
                int timer = *(int *)_args;
                int resp = getFlags(timer);
                rpcIPCReturn(msg.Source, msg.ID, &resp, sizeof(resp));
            }
            else fprintf(stderr, "[windowmanager] Unknown RPC request '%s' from process %d\n", req, msg.Source);
        }
        else if(msg.Number == MSG_RPC_FIND_SERVER && !strcmp("timekeeper", (const char *)msg.Data))
            rpcIPCFindServerRespond(msg.Source, msg.ID);
    }
    fprintf(stderr, "[" PROG_NAME "] Stopping time keeper\n");
    return 0;
}
