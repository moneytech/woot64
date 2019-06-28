#include <syscalls/syscalls.h>
#include <woot/sync.h>

int syncMutexCreate(unsigned flags)
{
    return sysSyncMutexCreate(flags);
}

int syncMutexDelete(int mutex)
{
    return sysSyncMutexDelete(mutex);
}

int syncMutexAcquire(int mutex, int timeout)
{
    return sysSyncMutexAcquire(mutex, timeout);
}

int syncMutexRelease(int mutex)
{
    return sysSyncMutexRelease(mutex);
}

int syncSemaphoreCreate(int count)
{
    return sysSyncSemaphoreCreate(count);
}

int syncSemaphoreDelete(int semaphore)
{
    return sysSyncSemaphoreDelete(semaphore);
}

int syncSemaphoreWait(int semaphore, int timeout)
{
    return sysSyncSemaphoreWait(semaphore, timeout);
}

int syncSemphoreSignal(int semaphore)
{
    return sysSyncSemaphoreSignal(semaphore);
}
