#pragma once

// KERNEL_MATCH: syscalls.hpp
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

typedef __SIZE_TYPE__ size_t;
typedef __INTPTR_TYPE__ off_t;
typedef __UINTPTR_TYPE__ uintptr_t;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

static __inline long __syscall0(long n)
{
	unsigned long ret;
    asm volatile("syscall": "=a"(ret) :"a"(n) :"rcx", "r11", "memory");
    return ret;
}

static __inline long __syscall1(long n, long a1)
{
	unsigned long ret;
	asm volatile("syscall": "=a"(ret): "a"(n), "D"(a1): "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall2(long n, long a1, long a2)
{
	unsigned long ret;
	asm volatile("syscall": "=a"(ret) :"a"(n), "D"(a1), "S"(a2): "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall3(long n, long a1, long a2, long a3)
{
	unsigned long ret;
	asm volatile("syscall": "=a"(ret): "a"(n), "D"(a1), "S"(a2), "d"(a3): "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall4(long n, long a1, long a2, long a3, long a4)
{
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	asm volatile("syscall": "=a"(ret): "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10):"rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall5(long n, long a1, long a2, long a3, long a4, long a5)
{
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	asm volatile("syscall": "=a"(ret): "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8): "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6)
{
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	register long r9 __asm__("r9") = a6;
	asm volatile("syscall": "=a"(ret): "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8), "r"(r9): "rcx", "r11", "memory");
	return ret;
}

size_t sys_read(int fd, char *buf, size_t count);
size_t sys_write(int fd, const char *buf, size_t count);
int sys_open(const char *filename, int flags, int mode);
int sys_close(int fd);
int sys_stat(const char *filename, void *statbuf);
int sys_fstat(int fd, void *statbuf);
off_t sys_lseek(int fd, off_t offset, unsigned int origin);
void *sys_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off);
int sys_munmap(void *addr, size_t len);
void *sys_brk(void *brk);
long sys_readv(int fd, const void *vec, size_t vlen);
long sys_writev(int fd, const void *vec, size_t vlen);
long sys_pipe(int *fds);
long sys_dup(int fd);
long sys_dup2(int oldfd, int newfd);
long sys_getpid(void);
long sys_exit(long error_code);
long sys_getdents(int fd, void *de, size_t count);
long sys_getcwd(char *buf, size_t size);
long sys_chdir(char *pathname);
long sys_sysinfo(void *info);
int sys_arch_prctl(int code, uintptr_t addr);
long sys_getdents64(int fd, void *de, size_t count);
int sys_set_tid_address(int *tidptr);
long sys_clock_get_time(int clock, void *ts);
long sys_exit_group(long error_code);

long sysFBGetCount(void);
long sysFBGetDefault(void);
long sysFBListIds(int *buf, unsigned bufSize);
long sysFBGetName(int id, char *buf, unsigned bufSize);
long sysFBOpen(int id);
long sysFBClose(int fd);
long sysFBGetModeCount(int fd);
long sysFBGetModeInfo(int fd, int mode, void *modeInfo);
long sysFBSetMode(int fd, int mode);
void *sysFBMapPixels(int fd, void *hint);
long sysFBGetCurrentMode(int fd);

long sysInDevGetCount(void);
long sysInDevListIds(int *buf, unsigned bufSize);
long sysInDevGetType(int id);
long sysInDevGetName(int id, char *buf, unsigned bufSize);
long sysInDevOpen(int id);
long sysInDevClose(int fd);
long sysInDevGetEvent(int fd, int timeout, void *buf);

long sysThreadCreate(const char *name, void *entry, uintptr_t arg, int *retVal);
long sysThreadDelete(int tid);
long sysThreadResume(int tid);
long sysThreadSuspend(int tid);
long sysThreadSleep(int tid, int ms);
long sysThreadWait(int tid, int timeout);
long sysThreadAbort(int tid, int retVal);
long sysThreadDaemonize(void);
long sysThreadGetId(void);

long sysProcessCreate(const char *cmdline);
long sysProcessDelete(int pid);
long sysProcessWait(int pid, int timeout);
long sysProcessAbort(int pid, int result);
long sysProcessListIds(int *buf, unsigned size);
long sysProcessGetName(int pid, char *buf, unsigned size);
long sysProcessGetThreadCount(int pid);
long sysProcessGetUsedMemory(int pid);

long sysIPCSendMessage(int dst, int num, int flags, void *payload, unsigned payloadSize);
long sysIPCGetMessage(void *msg, int timeout);
long sysIPCCreateSharedMem(const char *name, unsigned size);
long sysIPCOpenSharedMem(const char *name);
long sysIPCCloseSharedMem(int fd);
long sysIPCGetSharedMemSize(int fd);
void *sysIPCMapSharedMem(int fd, void *hint, unsigned flags);
long sysIPCUnMapSharedMem(int fd, void *addr);
long sysIPCPeekMessage(void *msg, unsigned offset);
long sysIPCWaitMessage(void *msg, int number, int source, int rangeStart, int rangeSize, int timeout);

long sysSyncMutexCreate(unsigned flags);
long sysSyncMutexDelete(int fd);
long sysSyncMutexAcquire(int fd, int timeout);
long sysSyncMutexRelease(int fd);
long sysSyncSemaphoreCreate(int count);
long sysSyncSemaphoreDelete(int fd);
long sysSyncSemaphoreWait(int fd, int timeout);
long sysSyncSemaphoreSignal(int fd);

void *sysSignalGetHandler(unsigned signum);
long sysSignalSetHandler(unsigned signum, void *handler);
long sysSignalIsEnabled(unsigned signum);
long sysSignalEnable(unsigned signum);
long sysSignalDisable(unsigned signum);
long sysSignalRaise(int tid, unsigned signum);
long sysSignalReturn(void);
long sysSignalGetCurrent(void);

#ifdef __cplusplus
}
#endif // __cplusplus
