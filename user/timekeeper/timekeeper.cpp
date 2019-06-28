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
            else fprintf(stderr, "[windowmanager] Unknown RPC request '%s' from process %d\n", req, msg.Source);
        }
        else if(msg.Number == MSG_RPC_FIND_SERVER && !strcmp("timekeeper", (const char *)msg.Data))
            rpcIPCFindServerRespond(msg.Source, msg.ID);
    }
    fprintf(stderr, "[" PROG_NAME "] Stopping time keeper\n");
    return 0;
}
