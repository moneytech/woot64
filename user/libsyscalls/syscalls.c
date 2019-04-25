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

long sys_readv(int fd, const void *vec, size_t vlen)
{
    return __syscall3(SYS_readv, fd, (long)vec, vlen);
}

long sys_writev(int fd, const void *vec, size_t vlen)
{
    return __syscall3(SYS_writev, fd, (long)vec, vlen);
}

long sys_getpid()
{
    return __syscall0(SYS_getpid);
}

long sys_exit(long error_code)
{
    return __syscall1(SYS_exit, error_code);
}

int sys_arch_prctl(int code, uintptr_t addr)
{
    return __syscall2(SYS_arch_prctl, code, addr);
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

long sysFBGetCount()
{
    return __syscall0(SYS_FB_GET_COUNT);
}

long sysFBGetDefault()
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

long sysInDevGetCount()
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

long sysThreadDelete(int fd)
{
    return __syscall1(SYS_THREAD_DELETE, fd);
}

long sysThreadResume(int fd)
{
    return __syscall1(SYS_THREAD_RESUME, fd);
}

long sysThreadSuspend(int fd)
{
    return __syscall1(SYS_THREAD_SUSPEND, fd);
}

long sysThreadSleep(int fd, int ms)
{
    return __syscall2(SYS_THREAD_SLEEP, fd, ms);
}

long sysThreadWait(int fd, int timeout)
{
    return __syscall2(SYS_THREAD_WAIT, fd, timeout);
}

long sysThreadAbort(int fd, int retVal)
{
    return __syscall2(SYS_THREAD_ABORT, fd, retVal);
}

long sysThreadDaemonize()
{
    return __syscall0(SYS_THREAD_DAEMONIZE);
}

long sysThreadGetId(int fd)
{
    return __syscall1(SYS_THREAD_GET_ID, fd);
}

long sysProcessCreate(const char *cmdline)
{
    return __syscall1(SYS_PROCESS_CREATE, (long)cmdline);
}

long sysProcessDelete(int fd)
{
    return __syscall1(SYS_PROCESS_DELETE, fd);
}

long sysProcessWait(int fd, int timeout)
{
    return __syscall2(SYS_PROCESS_WAIT, fd, timeout);
}

long sysProcessAbort(int fd, int result)
{
    return __syscall2(SYS_PROCESS_ABORT, fd, result);
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
