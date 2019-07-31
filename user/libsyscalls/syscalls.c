#include <syscalls/syscalls.h>

size_t sys_read(int fd, char *buf, size_t count)
{
    return __syscall3(SYS_read, fd, (long)buf, count);
}

size_t sys_write(int fd, const char *buf, size_t count)
{
    return __syscall3(SYS_write, fd, (long)buf, count);
}

int sys_open(const char *filename, int flags, int mode)
{
    return __syscall3(SYS_open, (long)filename, flags, mode);
}

int sys_close(int fd)
{
    return __syscall1(SYS_open, fd);
}

int sys_stat(const char *filename, void *statbuf)
{
    return __syscall2(SYS_stat, (long)filename, (long)statbuf);
}

int sys_fstat(int fd, void *statbuf)
{
    return __syscall2(SYS_fstat, fd, (long)statbuf);
}

int sys_lstat(const char *filename, void *statbuf)
{
    return __syscall2(SYS_stat, (long)filename, (long)statbuf);
}

off_t sys_lseek(int fd, off_t offset, unsigned int origin)
{
    return __syscall3(SYS_lseek, fd, offset, origin);
}

void *sys_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off)
{
    return (void *)__syscall6(SYS_mmap, (long)addr, len, prot, flags, fd, off);
}

int sys_munmap(void *addr, size_t len)
{
    return __syscall2(SYS_mmap, (long)addr, len);
}

void *sys_brk(void *brk)
{
    return (void *)__syscall1(SYS_brk, (long)brk);
}

long sys_rt_sigprocmask(int how, void *set, void *oldset, size_t sigsetsize)
{
    return __syscall4(SYS_rt_sigprocmask, (long)how, (long)set, (long)oldset, (long)sigsetsize);
}

long sys_rt_sigreturn(void)
{
    return __syscall0(SYS_rt_sigreturn);
}

long sys_readv(int fd, const void *vec, size_t vlen)
{
    return __syscall3(SYS_readv, fd, (long)vec, vlen);
}

long sys_writev(int fd, const void *vec, size_t vlen)
{
    return __syscall3(SYS_writev, fd, (long)vec, vlen);
}

long sys_pipe(int *fds)
{
    return __syscall1(SYS_pipe, (long)fds);
}

long sys_dup(int fd)
{
    return __syscall1(SYS_dup, fd);
}

long sys_dup2(int oldfd, int newfd)
{
    return __syscall2(SYS_dup2, oldfd, newfd);
}

long sys_getpid(void)
{
    return __syscall0(SYS_getpid);
}

long sys_exit(long error_code)
{
    return __syscall1(SYS_exit, error_code);
}

long sys_getdents(int fd, void *de, size_t count)
{
    return __syscall3(SYS_getdents, fd, (long)de, count);
}

long sys_getcwd(char *buf, size_t size)
{
    return __syscall2(SYS_getcwd, (long)buf, size);
}

long sys_chdir(char *pathname)
{
    return __syscall1(SYS_chdir, (long)pathname);
}

long sys_sysinfo(void *info)
{
    return __syscall1(SYS_sysinfo, (long)info);
}

int sys_arch_prctl(int code, uintptr_t addr)
{
    return __syscall2(SYS_arch_prctl, code, addr);
}

long sys_getdents64(int fd, void *de, size_t count)
{
    return __syscall3(SYS_getdents64, fd, (long)de, count);
}

int sys_set_tid_address(int *tidptr)
{
    return __syscall1(SYS_set_tid_address, (long)tidptr);
}

long sys_clock_get_time(int clock, void *ts)
{
    return __syscall2(SYS_clock_get_time, clock, (long)ts);
}

long sys_exit_group(long error_code)
{
    return __syscall1(SYS_exit_group, error_code);
}

long sys_pipe2(int *fds, int flags)
{
    return __syscall2(SYS_pipe2, (long)fds, (long)flags);
}

long sysFBGetCount(void)
{
    return __syscall0(SYS_FB_GET_COUNT);
}

long sysFBGetDefault(void)
{
    return __syscall0(SYS_FB_GET_DEFAULT);
}

long sysFBListIds(int *buf, unsigned bufSize)
{
    return __syscall2(SYS_FB_LIST_IDS, (long)buf, bufSize);
}

long sysFBGetName(int id, char *buf, unsigned bufSize)
{
    return __syscall3(SYS_FB_GET_NAME, id, (long)buf, bufSize);
}

long sysFBOpen(int id)
{
    return __syscall1(SYS_FB_OPEN, id);
}

long sysFBClose(int fd)
{
    return __syscall1(SYS_FB_CLOSE, fd);
}

long sysFBGetModeCount(int fd)
{
    return __syscall1(SYS_FB_GET_MODE_COUNT, fd);
}


long sysFBGetModeInfo(int fd, int mode, void *modeInfo)
{
    return __syscall3(SYS_FB_GET_MODE_INFO, fd, mode, (long)modeInfo);
}

long sysFBSetMode(int fd, int mode)
{
    return __syscall2(SYS_FB_SET_MODE, fd, mode);
}

void *sysFBMapPixels(int fd, void *hint)
{
    return (void *)__syscall2(SYS_FB_MAP_PIXELS, fd, (long)hint);
}

long sysFBGetCurrentMode(int fd)
{
    return __syscall1(SYS_FB_GET_CURRENT_MODE, fd);
}

long sysInDevGetCount(void)
{
    return __syscall0(SYS_INDEV_GET_COUNT);
}

long sysInDevListIds(int *buf, unsigned bufSize)
{
    return __syscall2(SYS_INDEV_LIST_IDS, (long)buf, bufSize);
}

long sysInDevGetType(int id)
{
    return __syscall1(SYS_INDEV_GET_TYPE, id);
}

long sysInDevGetName(int id, char *buf, unsigned bufSize)
{
    return __syscall3(SYS_INDEV_GET_NAME, id, (long)buf, bufSize);
}

long sysInDevOpen(int id)
{
    return __syscall1(SYS_INDEV_OPEN, id);
}

long sysInDevClose(int fd)
{
    return __syscall1(SYS_INDEV_CLOSE, fd);
}

long sysInDevGetEvent(int fd, int timeout, void *buf)
{
    return __syscall3(SYS_INDEV_GET_EVENT, fd, timeout, (long)buf);
}

long sysThreadCreate(const char *name, void *entry, uintptr_t arg, int *retVal)
{
    return __syscall4(SYS_THREAD_CREATE, (long)name, (long)entry, arg, (long)retVal);
}

long sysThreadDelete(int tid)
{
    return __syscall1(SYS_THREAD_DELETE, tid);
}

long sysThreadResume(int tid)
{
    return __syscall1(SYS_THREAD_RESUME, tid);
}

long sysThreadSuspend(int tid)
{
    return __syscall1(SYS_THREAD_SUSPEND, tid);
}

long sysThreadSleep(int tid, int ms)
{
    return __syscall2(SYS_THREAD_SLEEP, tid, ms);
}

long sysThreadWait(int tid, int timeout)
{
    return __syscall2(SYS_THREAD_WAIT, tid, timeout);
}

long sysThreadAbort(int tid, int retVal)
{
    return __syscall2(SYS_THREAD_ABORT, tid, retVal);
}

long sysThreadDaemonize(void)
{
    return __syscall0(SYS_THREAD_DAEMONIZE);
}

long sysThreadGetId(void)
{
    return __syscall0(SYS_THREAD_GET_ID);
}

long sysProcessCreate(const char *cmdline)
{
    return __syscall1(SYS_PROCESS_CREATE, (long)cmdline);
}

long sysProcessDelete(int pid)
{
    return __syscall1(SYS_PROCESS_DELETE, pid);
}

long sysProcessWait(int pid, int timeout)
{
    return __syscall2(SYS_PROCESS_WAIT, pid, timeout);
}

long sysProcessAbort(int pid, int result)
{
    return __syscall2(SYS_PROCESS_ABORT, pid, result);
}

long sysProcessListIds(int *buf, unsigned size)
{
    return __syscall2(SYS_PROCESS_LIST_IDS, (long)buf, size);
}

long sysProcessGetName(int pid, char *buf, unsigned size)
{
    return __syscall3(SYS_PROCESS_GET_NAME, pid, (long)buf, size);
}

long sysProcessGetThreadCount(int pid)
{
    return __syscall1(SYS_PROCESS_GET_THREAD_COUNT, pid);
}

long sysProcessGetUsedMemory(int pid)
{
    return __syscall1(SYS_PROCESS_GET_USED_MEMORY, pid);
}

long sysIPCSendMessage(int dst, int num, int flags, void *payload, unsigned payloadSize)
{
    return __syscall5(SYS_IPC_SEND_MESSAGE, dst, num, flags, (long)payload, payloadSize);
}

long sysIPCGetMessage(void *msg, int timeout)
{
    return __syscall2(SYS_IPC_GET_MESSAGE, (long)msg, timeout);
}

long sysIPCCreateSharedMem(const char *name, unsigned size)
{
    return __syscall2(SYS_IPC_CREATE_SHMEM, (long)name, size);
}

long sysIPCOpenSharedMem(const char *name)
{
    return __syscall1(SYS_IPC_OPEN_SHMEM, (long)name);
}

long sysIPCCloseSharedMem(int fd)
{
    return __syscall1(SYS_IPC_CLOSE_SHMEM, fd);
}

long sysIPCGetSharedMemSize(int fd)
{
    return __syscall1(SYS_IPC_GET_SHMEM_SIZE, fd);
}

void *sysIPCMapSharedMem(int fd, void *hint, unsigned flags)
{
    return (void *)__syscall3(SYS_IPC_MAP_SHMEM, fd, (long)hint, flags);
}

long sysIPCUnMapSharedMem(int fd, void *addr)
{
    return __syscall2(SYS_IPC_UNMAP_SHMEM, fd, (long)addr);
}

long sysIPCPeekMessage(void *msg, unsigned offset)
{
    return __syscall2(SYS_IPC_PEEK_MESSAGE, (long)msg, offset);
}

long sysIPCWaitMessage(void *msg, int number, int source, int rangeStart, int rangeSize, int timeout)
{
    return __syscall6(SYS_IPC_WAIT_MESSAGE, (long)msg, number, source, rangeStart, rangeSize, timeout);
}

long sysSyncMutexCreate(unsigned flags)
{
    return __syscall1(SYS_SYNC_MUTEX_CREATE, flags);
}

long sysSyncMutexDelete(int fd)
{
    return __syscall1(SYS_SYNC_MUTEX_DELETE, fd);
}

long sysSyncMutexAcquire(int fd, int timeout)
{
    return __syscall2(SYS_SYNC_MUTEX_ACQUIRE, fd, timeout);
}

long sysSyncMutexRelease(int fd)
{
    return __syscall1(SYS_SYNC_MUTEX_RELEASE, fd);
}

long sysSyncSemaphoreCreate(int count)
{
    return __syscall1(SYS_SYNC_SEMAPHORE_CREATE, count);
}

long sysSyncSemaphoreDelete(int fd)
{
    return __syscall1(SYS_SYNC_SEMAPHORE_DELETE, fd);
}

long sysSyncSemaphoreWait(int fd, int timeout)
{
    return __syscall2(SYS_SYNC_SEMAPHORE_WAIT, fd, timeout);
}

long sysSyncSemaphoreSignal(int fd)
{
    return __syscall1(SYS_SYNC_SEMAPHORE_SIGNAL, fd);
}

void *sysSignalGetHandler(unsigned signum)
{
    return (void *)__syscall1(SYS_SIGNAL_GET_HANDLER, signum);
}

long sysSignalSetHandler(unsigned signum, void *handler)
{
    return __syscall2(SYS_SIGNAL_SET_HANDLER, signum, (long)handler);
}

long sysSignalIsEnabled(unsigned signum)
{
    return __syscall1(SYS_SIGNAL_IS_ENABLED, signum);
}

long sysSignalEnable(unsigned signum)
{
    return __syscall1(SYS_SIGNAL_ENABLE, signum);
}

long sysSignalDisable(unsigned signum)
{
    return __syscall1(SYS_SIGNAL_DISABLE, signum);
}

long sysSignalRaise(int tid, unsigned signum)
{
    return __syscall2(SYS_SIGNAL_RAISE, tid, signum);
}

long sysSignalReturn(void)
{
    return __syscall0(SYS_SIGNAL_RETURN);
}

long sysSignalGetCurrent(void)
{
    return __syscall0(SYS_SIGNAL_GET_CURRENT);
}
