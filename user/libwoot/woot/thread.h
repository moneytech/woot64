#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>

#define THREAD_SELF (-1)

int threadCreate(const char *name, void *entry, uintptr_t arg, int *retVal);
int threadDelete(int tid);
int threadResume(int tid);
int threadSuspend(int tid);
int threadSleep(int tid, int ms);
int threadWait(int tid, int timeout);
int threadAbort(int tid, int retVal);
int threadDaemonize(void);
int threadGetId(void);

#ifdef __cplusplus
}
#endif // __cplusplus
