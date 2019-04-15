#pragma once

#include <types.h>

// USER_MATCH: libsyscalls/syscalls.h
// linux compatible syscalls
#define SYS_read            0
#define SYS_write           1
#define SYS_open            2
#define SYS_close           3
#define SYS_stat            4
#define SYS_fstat           5
#define SYS_lseek           8
#define SYS_mmap            9
#define SYS_munmap          11
#define SYS_brk             12
#define SYS_readv           19
#define SYS_writev          20
#define SYS_exit            60
#define SYS_arch_prctl      158
#define SYS_set_tid_address 218
#define SYS_exit_group      231

// woot specific syscalls
#define SYS_FB_GET_COUNT        0x300
#define SYS_FB_GET_DEFAULT      0x301
#define SYS_FB_LIST_IDS         0x302
#define SYS_FB_GET_NAME         0x303
#define SYS_FB_OPEN             0x304
#define SYS_FB_CLOSE            0x305
#define SYS_FB_GET_MODE_COUNT   0x306
#define SYS_FB_GET_MODE_INFO    0x307
#define SYS_FB_SET_MODE         0x308
#define SYS_FB_MAP_PIXELS       0x309
#define SYS_FB_GET_CURRENT_MODE 0x30A

#define SYS_INDEV_GET_COUNT     0x310
#define SYS_INDEV_LIST_IDS      0x311
#define SYS_INDEV_GET_TYPE      0x312
#define SYS_INDEV_GET_NAME      0x313
#define SYS_INDEV_OPEN          0x314
#define SYS_INDEV_CLOSE         0x315
#define SYS_INDEV_GET_EVENT     0x316

#define SYS_THREAD_CREATE       0x320
#define SYS_THREAD_DELETE       0x321
#define SYS_THREAD_RESUME       0x322
#define SYS_THREAD_SUSPEND      0x323
#define SYS_THREAD_SLEEP        0x324
#define SYS_THREAD_WAIT         0x325
#define SYS_THREAD_ABORT        0x326
#define SYS_THREAD_DAEMONIZE    0x327
#define SYS_THREAD_GET_ID       0x328

#define SYS_PROCESS_CREATE      0x330
#define SYS_PROCESS_DELETE      0x331
#define SYS_PROCESS_WAIT        0x332
#define SYS_PROCESS_ABORT       0x333

class SysCalls
{
    typedef intn (*SysCallHandler)(...);

    static SysCallHandler Handlers[1024];
    static long InvalidHandler();

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
    static long sys_readv(int fd, const struct iovec *vec, size_t vlen);
    static long sys_writev(int fd, const struct iovec *vec, size_t vlen);
    static long sys_exit(intn retVal);
    static long sys_arch_prctl(int code, uintptr_t addr);
    static long sys_set_tid_address(int *tidptr);
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
    static long sysThreadDelete(int fd);
    static long sysThreadResume(int fd);
    static long sysThreadSuspend(int fd);
    static long sysThreadSleep(int fd, int ms);
    static long sysThreadWait(int fd, int timeout);
    static long sysThreadAbort(int fd, int retVal);
    static long sysThreadDaemonize();
    static long sysThreadGetId(int fd);
public:
    static void Initialize();
};
