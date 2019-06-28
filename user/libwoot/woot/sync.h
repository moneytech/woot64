#pragma once

#define SYNC_FOREVER            -1
#define SYNC_MUTEX_RECURSIVE    1

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int syncMutexCreate(unsigned flags);
int syncMutexDelete(int mutex);
int syncMutexAcquire(int mutex, int timeout);
int syncMutexRelease(int mutex);

int syncSemaphoreCreate(int count);
int syncSemaphoreDelete(int semaphore);
int syncSemaphoreWait(int semaphore, int timeout);
int syncSemphoreSignal(int semaphore);

#ifdef __cplusplus
}
#endif // __cplusplus
