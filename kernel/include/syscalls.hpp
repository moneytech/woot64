#pragma once

#include <types.h>

// USER_MATCH: libsyscalls/syscalls.h
// linux compatible syscalls
#define SYS_read                        0
#define SYS_write                       1
#define SYS_open                        2
#define SYS_close                       3
#define SYS_stat                        4
#define SYS_fstat                       5
#define SYS_lseek                       8
#define SYS_mmap                        9
#define SYS_munmap                      11
#define SYS_brk                         12
#define SYS_rt_sigprocmask              14
#define SYS_rt_sigreturn                15
#define SYS_readv                       19
#define SYS_writev                      20
#define SYS_pipe                        22
#define SYS_dup                         32
#define SYS_dup2                        33
#define SYS_getpid                      39
#define SYS_exit                        60
#define SYS_getdents                    78
#define SYS_getcwd                      79
#define SYS_chdir                       80
#define SYS_sysinfo                     99
#define SYS_arch_prctl                  158
#define SYS_getdents64                  217
#define SYS_set_tid_address             218
#define SYS_clock_get_time              228
#define SYS_exit_group                  231

// woot specific syscalls
#define SYS_FB_GET_COUNT                0x300
#define SYS_FB_GET_DEFAULT              0x301
#define SYS_FB_LIST_IDS                 0x302
#define SYS_FB_GET_NAME                 0x303
#define SYS_FB_OPEN                     0x304
#define SYS_FB_CLOSE                    0x305
#define SYS_FB_GET_MODE_COUNT           0x306
#define SYS_FB_GET_MODE_INFO            0x307
#define SYS_FB_SET_MODE                 0x308
#define SYS_FB_MAP_PIXELS               0x309
#define SYS_FB_GET_CURRENT_MODE         0x30A

#define SYS_INDEV_GET_COUNT             0x310
#define SYS_INDEV_LIST_IDS              0x311
#define SYS_INDEV_GET_TYPE              0x312
#define SYS_INDEV_GET_NAME              0x313
#define SYS_INDEV_OPEN                  0x314
#define SYS_INDEV_CLOSE                 0x315
#define SYS_INDEV_GET_EVENT             0x316

#define SYS_THREAD_CREATE               0x320
#define SYS_THREAD_DELETE               0x321
#define SYS_THREAD_RESUME               0x322
#define SYS_THREAD_SUSPEND              0x323
#define SYS_THREAD_SLEEP                0x324
#define SYS_THREAD_WAIT                 0x325
#define SYS_THREAD_ABORT                0x326
#define SYS_THREAD_DAEMONIZE            0x327
#define SYS_THREAD_GET_ID               0x328

#define SYS_PROCESS_CREATE              0x330
#define SYS_PROCESS_DELETE              0x331
#define SYS_PROCESS_WAIT                0x332
#define SYS_PROCESS_ABORT               0x333
#define SYS_PROCESS_LIST_IDS            0x334
#define SYS_PROCESS_GET_NAME            0x335
#define SYS_PROCESS_GET_THREAD_COUNT    0x336
#define SYS_PROCESS_GET_USED_MEMORY     0x337

#define SYS_IPC_SEND_MESSAGE            0x340
#define SYS_IPC_GET_MESSAGE             0x341
#define SYS_IPC_CREATE_SHMEM            0x342
#define SYS_IPC_OPEN_SHMEM              0x343
#define SYS_IPC_CLOSE_SHMEM             0x344
#define SYS_IPC_GET_SHMEM_SIZE          0x345
#define SYS_IPC_MAP_SHMEM               0x346
#define SYS_IPC_UNMAP_SHMEM             0x347
#define SYS_IPC_PEEK_MESSAGE            0x348
#define SYS_IPC_WAIT_MESSAGE            0x349

#define SYS_SYNC_MUTEX_CREATE           0x350
#define SYS_SYNC_MUTEX_DELETE           0x351
#define SYS_SYNC_MUTEX_ACQUIRE          0x352
#define SYS_SYNC_MUTEX_RELEASE          0x353
#define SYS_SYNC_SEMAPHORE_CREATE       0x354
#define SYS_SYNC_SEMAPHORE_DELETE       0x355
#define SYS_SYNC_SEMAPHORE_WAIT         0x356
#define SYS_SYNC_SEMAPHORE_SIGNAL       0x357

#define SYS_SIGNAL_GET_HANDLER          0x360
#define SYS_SIGNAL_SET_HANDLER          0x361
#define SYS_SIGNAL_IS_ENABLED           0x362
#define SYS_SIGNAL_ENABLE               0x363
#define SYS_SIGNAL_DISABLE              0x364
#define SYS_SIGNAL_RAISE                0x365
#define SYS_SIGNAL_RETURN               0x366
#define SYS_SIGNAL_GET_CURRENT          0x367

class SysCalls
{
    typedef intn (*SysCallHandler)(...);

    static SysCallHandler Handlers[1024];
    static long InvalidHandler();
    static long SignalHandler();

    static long sys_read(int fd, char *buf, size_t count);
    static long sys_write(int fd, const char *buf, size_t count);
    static long sys_open(const char *filename, int flags, int mode);
    static long sys_close(int fd);
    static long sys_stat(const char *filename, struct stat *statbuf);
    static long sys_fstat(int fd, struct stat *statbuf);
    static long sys_lseek(int fd, off_t offset, unsigned int origin);
    static long sys_mmap(uintptr_t addr, unsigned long len, int prot, int flags, int fd, off_t off);
    static long sys_munmap(uintptr_t addr, size_t len);
    static long sys_brk(uintptr_t brk);
    static long sys_rt_sigprocmask(int how, void *set, void *oldset, size_t sigsetsize);
    static long sys_rt_sigreturn();
    static long sys_readv(int fd, const struct iovec *vec, size_t vlen);
    static long sys_writev(int fd, const struct iovec *vec, size_t vlen);
    static long sys_pipe(int *fds);
    static long sys_dup(int fd);
    static long sys_dup2(int oldfd, int newfd);
    static long sys_getpid();
    static long sys_exit(intn retVal);
    static long sys_getdents(int fd, struct dirent *de, size_t count);
    static long sys_getcwd(char *buf, size_t size);
    static long sys_chdir(char *pathname);
    static long sys_sysinfo(struct sysinfo *info);
    static long sys_arch_prctl(int code, uintptr_t addr);
    static long sys_getdents64(int fd, struct dirent *de, size_t count);
    static long sys_set_tid_address(int *tidptr);
    static long sys_clock_get_time(int clock, struct timespec *t);
    static long sys_exit_group(intn retVal);

    static long sysFBGetCount();
    static long sysFBGetDefault();
    static long sysFBListIds(int *buf, size_t bufSize);
    static long sysFBGetName(int id, char *buf, size_t bufSize);
    static long sysFBOpen(int id);
    static long sysFBClose(int fd);
    static long sysFBGetModeCount(int fd);
    static long sysFBGetModeInfo(int fd, int mode, struct vidModeInfo *modeInfo);
    static long sysFBSetMode(int fd, int mode);
    static long sysFBMapPixels(int fd, uintptr_t hint);
    static long sysFBGetCurrentMode(int fd);

    static long sysInDevGetCount();
    static long sysInDevListIds(int *buf, size_t bufSize);
    static long sysInDevGetType(int id);
    static long sysInDevGetName(int id, char *buf, size_t bufSize);
    static long sysInDevOpen(int id);
    static long sysInDevClose(int fd);
    static long sysInDevGetEvent(int fd, int timeout, struct inpEvent *event);

    static long sysThreadCreate(const char *name, void *entry, uintptr_t arg, int *retVal);
    static long sysThreadDelete(int tid);
    static long sysThreadResume(int tid);
    static long sysThreadSuspend(int tid);
    static long sysThreadSleep(int tid, int ms);
    static long sysThreadWait(int tid, int timeout);
    static long sysThreadAbort(int tid, int retVal);
    static long sysThreadDaemonize();
    static long sysThreadGetId();

    static long sysProcessCreate(const char *cmdline);
    static long sysProcessDelete(int pid);
    static long sysProcessWait(int pid, int timeout);
    static long sysProcessAbort(int pid, int result);
    static long sysProcessListIds(int *buf, size_t bufSize);
    static long sysProcessGetName(int pid, char *buf, size_t bufSize);
    static long sysProcessGetThreadCount(int pid);
    static long sysProcessGetUsedMemory(int pid);

    static long sysIPCSendMessage(int dst, int num, int flags, void *payload, unsigned payloadSize);
    static long sysIPCGetMessage(void *msg, int timeout);
    static long sysIPCCreateSharedMem(const char *name, unsigned size);
    static long sysIPCOpenSharedMem(const char *name);
    static long sysIPCCloseSharedMem(int fd);
    static long sysIPCGetSharedMemSize(int fd);
    static long sysIPCMapSharedMem(int fd, uintptr_t hint, unsigned flags);
    static long sysIPCUnMapSharedMem(int fd, uintptr_t addr);
    static long sysIPCPeekMessage(void *msg, unsigned offset);
    static long sysIPCWaitMessage(void *msg, int number, int source, int rangeStart, int rangeSize, int timeout);

    static long sysSyncMutexCreate(unsigned flags);
    static long sysSyncMutexDelete(int fd);
    static long sysSyncMutexAcquire(int fd, int timeout);
    static long sysSyncMutexRelease(int fd);
    static long sysSyncSemaphoreCreate(int count);
    static long sysSyncSemaphoreDelete(int fd);
    static long sysSyncSemaphoreWait(int fd, int timeout);
    static long sysSyncSemaphoreSignal(int fd);

    static long sysSignalGetHandler(unsigned signum);
    static long sysSignalSetHandler(unsigned signum, void *handler);
    static long sysSignalIsEnabled(unsigned signum);
    static long sysSignalEnable(unsigned signum);
    static long sysSignalDisable(unsigned signum);
    static long sysSignalRaise(pid_t tid, unsigned signum);
    static long sysSignalReturn();
    static long sysSignalGetCurrent();

public:
    static void Initialize();
};
