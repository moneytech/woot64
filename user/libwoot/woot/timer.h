#pragma once

#define TIMER_ONE_SHOT      1

typedef struct
{
    int Id;
} timerMsg_t;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int timerInitialize();
int timerCleanup();
const char *timerGetServer();
int timerCreate(int timeout, int flags);
int timerDelete(int timer);
int timerStart(int timer);
int timerStop(int timer);
int timerPause(int timer);
int timerResume(int timer);
int timerReload(int timer);
int timerSetTimeout(int timer, int timeout);
int timerGetTimeout(int timer);
int timerSetFlags(int timer, int flags);
int timerGetFlags(int timer);

#ifdef __cplusplus
}
#endif // __cplusplus
