#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <woot/ipc.h>
#include <woot/rpc.h>
#include <woot/sync.h>
#include <woot/thread.h>
#include <woot/timer.h>
#include <woot/vector.h>

#define PROG_NAME "timekeeper"

static int timerMutex;
static int runSemaphore;

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

enum class TimerFlags : unsigned
{
    OneShot = 1
};

TimerFlags operator &(TimerFlags a, TimerFlags b)
{
    return (TimerFlags)((unsigned)a & (unsigned)b);
}

enum class TimerState
{
    Stopped = 0,
    Running,
    Paused
};

struct Timer
{
    int id;
    int process;
    int timeout;
    TimerFlags flags;
    TimerState state;
};

struct Event
{
    int delta;
    Timer *timer;
};

static vecVector_t *timers;
static vecVector_t *events;

static void enqueueTimer(Timer *t)
{
    t->state = TimerState::Running;
    int eventCount = vecSize(events);
    if(eventCount <= 0)
    {
        Event event = { .delta = t->timeout, .timer = t };
        vecAppend(events, &event);
    }
    else
    {
        int delta = t->timeout;
        int i = eventCount - 1;
        int sum = 0;
        for(; i >= 0; --i)
        {
            Event newEvent = { .delta = delta, .timer = t };
            Event *event = (Event *)vecGet(events, i);
            sum += event->delta;
            if(delta < sum)
            {
                event->delta -= delta;
                vecInsertAfter(events, i, &newEvent);
                break;
            }
            delta -= event->delta;
        }
        if(i < 0)
        {
            Event newEvent = { .delta = delta, .timer = t };
            vecPrepend(events, &newEvent);
        }
    }
    /*eventCount = vecSize(events);
    for(unsigned i = 0; i < eventCount; ++i)
    {
        Event *event = (Event *)vecGet(events, i);
        fprintf(stderr, "%d ", event->delta);
    }
    fprintf(stderr, "\n");*/
    syncSemphoreSignal(runSemaphore);
}

static void dequeueTimer(Timer *t)
{
    int eventCount = vecSize(events);
    for(unsigned i = 0; i < eventCount; ++i)
    {
        Event *event = (Event *)vecGet(events, i);
        if(event->timer != t)
            continue;
        if(i)
        {
            Event *prevEvent = (Event *)vecGet(events, i - 1);
            prevEvent->delta += event->delta;
        }
        vecRemoveAt(events, i);
        t->state = TimerState::Stopped;
    }
}

static int getNewId()
{
    static int ids = 0;
    return __sync_add_and_fetch(&ids, 1);
}

static int createTimer(int process, int timeout, TimerFlags flags)
{
    if(timeout <= 0) return -EINVAL;

    if(syncMutexAcquire(timerMutex, 1000) < 0)
        return -EBUSY;
    Timer timer =
    {
        .id = getNewId(),
        .process = process,
        .timeout = timeout,
        .flags = flags,
        .state = TimerState::Stopped
    };
    vecAppend(timers, &timer);
    syncMutexRelease(timerMutex);
    return timer.id;
}

static int deleteTimer(int process, int timer)
{
    if(syncMutexAcquire(timerMutex, 1000) < 0)
        return -EBUSY;
    unsigned timerCount = vecSize(timers);
    for(unsigned i = 0; i < timerCount; ++i)
    {
        Timer *t = (Timer *)vecGet(timers, i);
        if(t->process != process || t->id != timer)
            continue;
        dequeueTimer(t);
        vecRemoveAt(timers, i);
        syncMutexRelease(timerMutex);
        return 0;
    }
    syncMutexRelease(timerMutex);
    return -ENOENT;
}

static int startTimer(int process, int timer)
{
    if(syncMutexAcquire(timerMutex, 1000) < 0)
        return -EBUSY;
    unsigned timerCount = vecSize(timers);
    for(unsigned i = 0; i < timerCount; ++i)
    {
        Timer *t = (Timer *)vecGet(timers, i);
        if(t->process != process || t->id != timer)
            continue;
        if(t->state == TimerState::Stopped)
            enqueueTimer(t);
        syncMutexRelease(timerMutex);
        return 0;
    }
    syncMutexRelease(timerMutex);
    return -ENOENT;
}

static int stopTimer(int process, int timer)
{
    if(syncMutexAcquire(timerMutex, 1000) < 0)
        return -EBUSY;
    unsigned timerCount = vecSize(timers);
    for(unsigned i = 0; i < timerCount; ++i)
    {
        Timer *t = (Timer *)vecGet(timers, i);
        if(t->process != process || t->id != timer)
            continue;
        if(t->state == TimerState::Running)
            dequeueTimer(t);
        syncMutexRelease(timerMutex);
        return 0;
    }
    syncMutexRelease(timerMutex);
    return -ENOENT;
}

static int pauseTimer(int timer)
{
    if(syncMutexAcquire(timerMutex, 1000) < 0)
        return -EBUSY;
    int res = -ENOSYS;
    syncMutexRelease(timerMutex);
    return res;
}

static int resumeTimer(int timer)
{
    if(syncMutexAcquire(timerMutex, 1000) < 0)
        return -EBUSY;
    int res = -ENOSYS;
    syncMutexRelease(timerMutex);
    return res;
}

static int reloadTimer(int timer)
{
    if(syncMutexAcquire(timerMutex, 1000) < 0)
        return -EBUSY;
    int res = -ENOSYS;
    syncMutexRelease(timerMutex);
    return res;
}

static int setTimeout(int timer, int timeout)
{
    if(syncMutexAcquire(timerMutex, 1000) < 0)
        return -EBUSY;
    int res = -ENOSYS;
    syncMutexRelease(timerMutex);
    return res;
}

static int getTimeout(int timer)
{
    if(syncMutexAcquire(timerMutex, 1000) < 0)
        return -EBUSY;
    int res = -ENOSYS;
    syncMutexRelease(timerMutex);
    return res;
}

static int setFlags(int timer, int flags)
{
    if(syncMutexAcquire(timerMutex, 1000) < 0)
        return -EBUSY;
    int res = -ENOSYS;
    syncMutexRelease(timerMutex);
    return res;
}

static int getFlags(int timer)
{
    if(syncMutexAcquire(timerMutex, 1000) < 0)
        return -EBUSY;
    int res = -ENOSYS;
    syncMutexRelease(timerMutex);
    return res;
}

static int timerThreadProc(uintptr_t arg)
{
    for(int cycle = 0;; ++cycle)
    {
        syncSemaphoreWait(runSemaphore, SYNC_FOREVER);
        if(syncMutexAcquire(timerMutex, 1000) < 0)
            continue; // something is locked
        int eventCount = vecSize(events);
        if(eventCount <= 0)
        {   // there are no events (shouldn't happen at this point)
            syncMutexRelease(timerMutex);
            continue;
        }
        int eventIdx = eventCount - 1;
        Event *event = (Event *)vecGet(events, eventIdx);
        if(!event)
        {   // couldn't get the event (shouldn't happen at this point)
            syncMutexRelease(timerMutex);
            continue;
        }
        Timer timer = *event->timer;
        int millis = event->delta;
        vecRemoveAt(events, eventIdx); // remove event
        if(!(bool)(timer.flags & TimerFlags::OneShot))
            enqueueTimer(event->timer); // requeue this timer;
        else event->timer->state = TimerState::Stopped;
        syncMutexRelease(timerMutex);

        threadSleep(THREAD_SELF, millis); // FIXME: there is a long lag when starting
                                          //        timer with smaller timeout than current delta
        timerMsg_t msg;
        msg.Id = timer.id;
        int res = ipcSendMessage(timer.process, MSG_TIMER, MSG_FLAG_NONE, &msg, sizeof(msg));
        if(res == -ESRCH)   // target process is most likely not there anymore
            deleteTimer(timer.process, timer.id);
    }
    return 0;
}

extern "C" int main(int argc, char *argv[])
{
    fprintf(stderr, "[" PROG_NAME "] Time keeper started\n");

    timers = vecCreate(sizeof(Timer), 64, 64, __INT_MAX__);
    events = vecCreate(sizeof(Event), 256, 64, __INT_MAX__);
    timerMutex = syncMutexCreate(0);
    runSemaphore = syncSemaphoreCreate(0);
    int timerThread = threadCreate("timer thread", (void *)timerThreadProc, 0, NULL);
    threadResume(timerThread);
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
                int id = createTimer(msg.Source, args->timeout, (TimerFlags)args->flags);
                rpcIPCReturn(msg.Source, msg.ID, &id, sizeof(id));
            }
            else if(!strcmp(req, "timerDelete"))
            {
                int timer = *(int *)_args;
                int resp = deleteTimer(msg.Source, timer);
                rpcIPCReturn(msg.Source, msg.ID, &resp, sizeof(resp));
            }
            else if(!strcmp(req, "timerStart"))
            {
                int timer = *(int *)_args;
                int resp = startTimer(msg.Source, timer);
                rpcIPCReturn(msg.Source, msg.ID, &resp, sizeof(resp));
            }
            else if(!strcmp(req, "timerStop"))
            {
                int timer = *(int *)_args;
                int resp = stopTimer(msg.Source, timer);
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

    threadDelete(timerThread);
    syncSemaphoreDelete(runSemaphore);
    syncMutexDelete(timerMutex);
    if(events) vecDelete(events);
    if(timers) vecDelete(timers);

    return 0;
}
