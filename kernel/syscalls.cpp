#include <cpu.hpp>
#include <debug.hpp>
#include <dentry.hpp>
#include <directoryentry.hpp>
#include <errno.h>
#include <file.hpp>
#include <filesystem.hpp>
#include <framebuffer.hpp>
#include <inode.hpp>
#include <memory.hpp>
#include <misc.hpp>
#include <paging.hpp>
#include <process.hpp>
#include <sharedmem.hpp>
#include <signal.hpp>
#include <string.hpp>
#include <stringbuilder.hpp>
#include <syscalls.hpp>
#include <sysdefs.h>
#include <thread.hpp>

#define ARCH_SET_GS		0x1001
#define ARCH_SET_FS		0x1002
#define ARCH_GET_FS		0x1003
#define ARCH_GET_GS		0x1004

#define SIG_BLOCK   0
#define SIG_UNBLOCK 1
#define SIG_SETMASK 2

extern "C" void syscallHandler();
asm(
INLINE_ASM_SYNTAX
".extern mainTSS\n"
".type syscallHandler, function\n"
"syscallHandler:\n"
"sub rsp, 128\n"                            // deal with red zone
"push rcx\n"                                // save return address
"push rbp\n"                                // save user frame pointer
"mov rbp, rsp\n"                            // save user stack pointer
"mov rsp, [rip + mainTSS + 4]\n"            // load kernel stack
"and rsp, ~0xF\n"                           // align stack for SSE
"sti\n"                                     // enable interrupts disabled by FMASK msr

"push rbx\n"
"push rdx\n"
"push rsi\n"
"push rdi\n"
"push r8\n"
"push r9\n"
"push r10\n"
"push r12\n"
"push r13\n"
"push r14\n"
"push r15\n"

"push r11\n"                                // save flags
"mov rcx, r10\n"                            // move argument 4 from r10 to rcx
"lea r11, [rip + _ZN8SysCalls8HandlersE]\n" // get SysCalls::Handler address
"mov r11, [r11 + rax * 8]\n"                // calculate handler address
"or r11, r11\n"                             // validate handler address
"jnz 1f\n"                                  //   and skip InvalidHandler call if handler is valid
"call _ZN8SysCalls14InvalidHandlerEv\n"     // call InvalidHandler
"jmp 2f\n"                                  //   and return
"1: call r11\n"                             // call actual handler
"2: pop r11\n"                              // restore flags

"push rax\n"                                // save syscall result
"call _ZN8SysCalls13SignalHandlerEv\n"      // call function for handling syscall initiated signals
"pop rax\n"                                 // restore syscall result

"pop r15\n"
"pop r14\n"
"pop r13\n"
"pop r12\n"
"pop r10\n"
"pop r9\n"
"pop r8\n"
"pop rdi\n"
"pop rsi\n"
"pop rdx\n"
"pop rbx\n"

"mov rsp, rbp\n"                            // restore user stack pointer
"pop rbp\n"                                 // restore user frame pointer
"pop rcx\n"                                 // restore return address
"add rsp, 128\n"                            // deal with red zone
"sysretq\n"                                 // return to usermode code
NORMAL_ASM_SYNTAX
);

#pragma pack(push, 1)

struct iovec
{
    void *iov_base;     /* Starting address */
    size_t iov_len;     /* Number of bytes to transfer */
};

// for sys_clock_get_time and sys_clock_getres
#define CLOCK_REALTIME              0
#define CLOCK_MONOTONIC             1
#define CLOCK_PROCESS_CPUTIME_ID    2
#define CLOCK_THREAD_CPUTIME_ID     3

#ifdef __i386__
typedef unsigned long long dev_t;
typedef unsigned int nlink_t;
typedef unsigned int blksize_t;
typedef unsigned long long blkcnt_t;

struct stat
{
    dev_t st_dev;
    int __st_dev_padding;
    long __st_ino_truncated;
    mode_t st_mode;
    nlink_t st_nlink;
    uid_t st_uid;
    gid_t st_gid;
    dev_t st_rdev;
    int __st_rdev_padding;
    off_t st_size;
    blksize_t st_blksize;
    blkcnt_t st_blocks;
    struct timespec st_atim;
    struct timespec st_mtim;
    struct timespec st_ctim;
    ino_t st_ino;
};
#endif // __i386__
#ifdef __x86_64__
typedef long dev_t;
typedef long nlink_t;
typedef long blksize_t;
typedef long blkcnt_t;

struct stat
{
    dev_t st_dev;
    ino_t st_ino;
    nlink_t st_nlink;

    mode_t st_mode;
    uid_t st_uid;
    gid_t st_gid;
    unsigned int __pad0;
    dev_t st_rdev;
    off_t st_size;
    blksize_t st_blksize;
    blkcnt_t st_blocks;

    struct timespec st_atim;
    struct timespec st_mtim;
    struct timespec st_ctim;
    long __unused[3];
};
#endif // __x86_64__

// USER_MATCH: libwoot/woot/video.h struct vidModeInfo
typedef struct vidModeInfo
{
    int Width, Height;
    int BitsPerPixel;
    int RefreshRate;
    int Pitch;
    int Flags;
    int AlphaBits, RedBits, GreenBits, BlueBits;
    int AlphaShift, RedShift, GreenShift, BlueShift;
} vidModeInfo_t;

// USER_MATCH: libwoot/woot/input.h
#define INP_MAX_RAW_BYTES           64
#define INP_MAX_MOUSE_AXES          13
#define INP_MAX_TABLET_COORDS       7
#define INP_MAX_TABLET_AXES         6
#define INP_MAX_CONTROLLER_COORDS   13

// USER_MATCH: libc/musl-1.1.21/include/dirent.h
struct dirent
{
    ino_t d_ino;
    off_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[0];
};

// USER_MATCH: libwoot/woot/process.h
#define PROC_MAP_READ   (1 << 0)
#define PROC_MAP_WRITE  (1 << 1)
#define PROC_MAP_EXEC   (1 << 2)
#define PROC_MAP_NAME   (1 << 3)
#define PROC_MAP_STACK  (1 << 4)
#define PROC_MAP_HEAP   (1 << 5)
#define PROC_MAP_LAST   (1 << 30)

// USER_MATCH: libwoot/woot/process.h
typedef struct processMapEntry
{
    unsigned long EntrySize;    // Size of this entry
    unsigned long Address;      // Start of the mapping
    unsigned long Size;         // Size of the mapping
    unsigned long Offset;       // File offset of the mapping
    unsigned long Flags;        // Flags
    char Name[];                // Optional NULL terminated filename
} processMapEntry_t;

SysCalls::SysCallHandler SysCalls::Handlers[1024];

// check if buffer is completely contained in userspace
#define BUFFER_CHECK(buf, size) \
{ \
    if(reinterpret_cast<uintptr_t>(buf) < USER_BASE && reinterpret_cast<uintptr_t>(buf) >= USER_END) return -EFAULT; \
    if((size + reinterpret_cast<uintptr_t>(buf)) >= USER_END) return -EFAULT; \
}

struct sysinfo
{
    long uptime;             /* Seconds since boot */
    unsigned long loads[3];  /* 1, 5, and 15 minute load averages */
    unsigned long totalram;  /* Total usable main memory size */
    unsigned long freeram;   /* Available memory size */
    unsigned long sharedram; /* Amount of shared memory */
    unsigned long bufferram; /* Memory used by buffers */
    unsigned long totalswap; /* Total swap space size */
    unsigned long freeswap;  /* swap space still available */
    unsigned short procs;    /* Number of current processes */
    unsigned long totalhigh; /* Total high memory size */
    unsigned long freehigh;  /* Available high memory size */
    unsigned int mem_unit;   /* Memory unit size in bytes */
    char _f[20-2*sizeof(long)-sizeof(int)]; /* Padding to 64 bytes */
};

#pragma pack(pop)

long SysCalls::InvalidHandler()
{
    // not sure if this is stable
    uintn number; asm volatile("": "=a"(number));
    DEBUG("[syscalls] Unknown syscall %d\n", number);
    return -ENOSYS;
}

long SysCalls::SignalHandler()
{
    if(Thread::GetCurrent()->CurrentSignal < 0)
    {
        uintptr_t *thisStackFrame = reinterpret_cast<uintptr_t *>(__builtin_frame_address(0));
        uintptr_t *prevStackFrame = reinterpret_cast<uintptr_t *>(*thisStackFrame);
        Signal::HandleSignals(Thread::GetCurrent(), prevStackFrame + 1);
    }
    return 0;
}

long SysCalls::sys_read(int fd, char *buf, size_t count)
{
    BUFFER_CHECK(buf, count)
    return Process::GetCurrent()->Read(fd, buf, count);
}

long SysCalls::sys_write(int fd, const char *buf, size_t count)
{
    BUFFER_CHECK(buf, count)
    return Process::GetCurrent()->Write(fd, buf, count);
}

long SysCalls::sys_open(const char *filename, int flags, int mode)
{
    BUFFER_CHECK(filename, String::Size(filename))
    int res = Process::GetCurrent()->Open(filename, flags, mode);
    DEBUG("sys_open(\"%s\", %p) returned %d\n", filename, flags, res);
    return res;
}

long SysCalls::sys_close(int fd)
{
    return Process::GetCurrent()->Close(fd);
}

long SysCalls::sys_stat(const char *filename, stat *statbuf)
{
    BUFFER_CHECK(filename, String::Size(filename))
    BUFFER_CHECK(statbuf, sizeof(stat))

    Memory::Zero(statbuf, sizeof(struct stat));
    File *f = File::Open(filename, 0, 0, true);
    if(!f) return -EBADF;
    INode *inode = f->DEntry->INode;
    statbuf->st_ino = inode->Number;
    statbuf->st_mode = inode->GetMode();
    statbuf->st_nlink = inode->GetLinkCount();
    statbuf->st_uid = inode->GetUID();
    statbuf->st_gid = inode->GetGID();
    statbuf->st_size = inode->GetSize();
    statbuf->st_blksize = 512;
    statbuf->st_blocks = align(statbuf->st_size, statbuf->st_blksize);
    statbuf->st_atim.tv_sec = inode->GetAccessTime();
    statbuf->st_mtim.tv_sec = inode->GetModifyTime();
    statbuf->st_ctim.tv_sec = inode->GetCreateTime();
    delete f;
    return 0;
}

long SysCalls::sys_fstat(int fd, stat *statbuf)
{
    BUFFER_CHECK(statbuf, sizeof(stat))

    Memory::Zero(statbuf, sizeof(struct stat));
    File *f = (File *)Process::GetCurrent()->GetHandleData(fd, Process::Handle::HandleType::File);
    if(!f) return -EBADF;
    INode *inode = f->DEntry->INode;
    statbuf->st_ino = inode->Number;
    statbuf->st_mode = inode->GetMode();
    statbuf->st_nlink = inode->GetLinkCount();
    statbuf->st_uid = inode->GetUID();
    statbuf->st_gid = inode->GetGID();
    statbuf->st_size = inode->GetSize();
    statbuf->st_blksize = 512;
    statbuf->st_blocks = align(statbuf->st_size, statbuf->st_blksize);
    statbuf->st_atim.tv_sec = inode->GetAccessTime();
    statbuf->st_mtim.tv_sec = inode->GetModifyTime();
    statbuf->st_ctim.tv_sec = inode->GetCreateTime();
    return 0;
}

long SysCalls::sys_lstat(const char *filename, stat *statbuf)
{
    BUFFER_CHECK(filename, String::Size(filename))
    BUFFER_CHECK(statbuf, sizeof(stat))

    Memory::Zero(statbuf, sizeof(struct stat));
    File *f = File::Open(filename, 0, 0, false);
    if(!f) return -EBADF;
    INode *inode = f->DEntry->INode;
    statbuf->st_ino = inode->Number;
    statbuf->st_mode = inode->GetMode();
    statbuf->st_nlink = inode->GetLinkCount();
    statbuf->st_uid = inode->GetUID();
    statbuf->st_gid = inode->GetGID();
    statbuf->st_size = inode->GetSize();
    statbuf->st_blksize = 512;
    statbuf->st_blocks = align(statbuf->st_size, statbuf->st_blksize);
    statbuf->st_atim.tv_sec = inode->GetAccessTime();
    statbuf->st_mtim.tv_sec = inode->GetModifyTime();
    statbuf->st_ctim.tv_sec = inode->GetCreateTime();
    delete f;
    return 0;
}

long SysCalls::sys_lseek(int fd, off_t offset, unsigned int origin)
{
    File *f = (File *)Process::GetCurrent()->GetHandleData(fd, Process::Handle::HandleType::File);
    if(!f) return -errno;
    return f->Seek(offset, origin);
}

long SysCalls::sys_mmap(uintptr_t addr, unsigned long len, int prot, int flags, int fd, off_t off)
{
    //DEBUG("sys_mmap(%p, %p, %p, %p, %d, %p)\n", addr, len, prot, flags, fd, off);

    BUFFER_CHECK(addr, len)

    len = align(len, PAGE_SIZE);
    uintptr_t pgoffset = off << PAGE_SHIFT;

    if(addr >= KERNEL_BASE)
        return -EINVAL;

    if(!addr)
    {
        Process *cp = Process::GetCurrent();
        addr = cp->MMapSBrk(len, true);
    }
    else
    {
        for(uintptr_t va = addr; va < (addr + len); va += PAGE_SIZE)
        {
            uintptr_t pa = Paging::GetPhysicalAddress(PG_CURRENT_ADDR_SPC, va);
            if(pa == PG_INVALID_ADDRESS)
            {
                pa = Paging::AllocFrame();
                if(pa == PG_INVALID_ADDRESS) return -ENOMEM;
                if(!Paging::MapPage(PG_CURRENT_ADDR_SPC, va, pa, true, true))
                    return -1;
            }
            Memory::Zero((void *)va, PAGE_SIZE);    // zero mmapped memory to avoid
                                                    // information leak from kernel to userspace
        }
    }

    if(fd < 0)
        return addr;

    Process *cp = Process::GetCurrent();
    File *f = (File *)cp->GetHandleData(fd, Process::Handle::HandleType::File);
    if(f)
    {
        f->Seek((pgoffset + 0ULL) * PAGE_SIZE, SEEK_SET);
        f->Read((void *)addr, len);
    }

    return static_cast<long>(addr);
}

long SysCalls::sys_mprotect(uintptr_t addr, size_t len, unsigned long prot)
{
    (void)addr, (void)len, (void)prot;
    DEBUG("[syscalls] dummy mprotect called\n");
    return ESUCCESS;
}

long SysCalls::sys_munmap(uintptr_t addr, size_t len)
{
    //DEBUG("sys_munmap(%p, %p)\n", addr, len);
    Paging::UnmapRange(PG_CURRENT_ADDR_SPC, addr, len);
    return 0;
}

long SysCalls::sys_brk(uintptr_t brk)
{
    //DEBUG("sys_brk(%p)\n", args[1]);
    return static_cast<long>(Process::GetCurrent()->Brk(brk, true));
}

long SysCalls::sys_rt_sigprocmask(int how, void *set, void *oldset, size_t sigsetsize)
{
    BUFFER_CHECK(set, sigsetsize)
    BUFFER_CHECK(oldset, sigsetsize)

    if(!sigsetsize)
        return -EINVAL;

    Process *cp = Process::GetCurrent();
    Thread *t = cp->Threads[0];

    uint128_t oldmask = t->SignalMask;

    if(oldset) Memory::Move(oldset, &oldmask, sigsetsize < sizeof(oldmask) ? sigsetsize : sizeof(oldmask));

    if(set)
    {
        uint128_t mask = 0;
        Memory::Move(&mask, set, sigsetsize < sizeof(mask) ? sigsetsize : sizeof(mask));

        switch(how)
        {
        case SIG_BLOCK:
            mask = oldmask & ~mask;
            break;
        case SIG_UNBLOCK:
            mask = oldmask | mask;
            break;
        case SIG_SETMASK:
            break;
        default:
            return -EINVAL;
        }

        t->SignalMask = mask;
    }
    return ESUCCESS;
}

long SysCalls::sys_rt_sigreturn()
{
    return sysSignalReturn();
}

long SysCalls::sys_readv(int fd, const iovec *vec, size_t vlen)
{
    BUFFER_CHECK(vec, sizeof(iovec) * vlen)

    long res = 0;
    Process *cp = Process::GetCurrent();

    for(size_t i = 0; i < vlen; ++i)
    {
        BUFFER_CHECK(vec[i].iov_base, vec[i].iov_len)
        long r = cp->Read(fd, vec[i].iov_base, vec[i].iov_len);
        if(r < 0) return r;
        res += r;
    }
    return res;
}

long SysCalls::sys_writev(int fd, const iovec *vec, size_t vlen)
{
    BUFFER_CHECK(vec, sizeof(iovec) * vlen)

    long res = 0;
    Process *cp = Process::GetCurrent();

    for(size_t i = 0; i < vlen; ++i)
    {
        BUFFER_CHECK(vec[i].iov_base, vec[i].iov_len)
        long r = cp->Write(fd, vec[i].iov_base, vec[i].iov_len);
        if(r < 0) return r;
        res += r;
    }
    return res;
}

long SysCalls::sys_pipe(int *fds)
{
    BUFFER_CHECK(fds, 2 * sizeof(*fds))
            return Process::GetCurrent()->CreatePipe(fds);
}

long SysCalls::sys_msync(uintptr_t addr, size_t len, int flags)
{
    DEBUG("[syscalls] dummy msync called\n");
    return (len + PAGE_SIZE - 1) / PAGE_SIZE;
}

long SysCalls::sys_mincore(uintptr_t addr, size_t len, unsigned char *vec)
{
    DEBUG("sys_mincore(%p, %p, %p)\n", addr, len, vec);

    size_t pageCount = (len + PAGE_SIZE - 1) / PAGE_SIZE;

    BUFFER_CHECK(vec, pageCount)

    Memory::Set(vec, 0, pageCount);

    long pg = 0;
    for(uintptr_t endAddr = addr + len; addr < endAddr; addr += PAGE_SIZE, ++pg)
    {
        if(addr < USER_BASE)
            continue;

        if(addr >= USER_END)
            break;

        uintptr_t pa = Paging::GetPhysicalAddress(PG_CURRENT_ADDR_SPC, addr);
        if(pa == PG_INVALID_ADDRESS)
            continue;

        vec[pg] = 1;
    }

    return pg;
}

long SysCalls::sys_dup(int fd)
{
    return Process::GetCurrent()->DuplicateFileDescriptor(fd);
}

long SysCalls::sys_dup2(int oldfd, int newfd)
{
    return Process::GetCurrent()->DuplicateFileDescriptor(oldfd, newfd);
}

long SysCalls::sys_getpid()
{
    return Process::GetCurrent()->Id;
}

long SysCalls::sys_exit(intn retVal)
{
    Thread::Finalize(nullptr, retVal);
    return ESUCCESS;
}

long SysCalls::sys_getdents(int fd, dirent *de, size_t count)
{
    return sys_getdents64(fd, de, count);
}

long SysCalls::sys_getcwd(char *buf, size_t size)
{
    BUFFER_CHECK(buf, size)
    DEntry *dentry = Process::GetCurrentDir();
    if(!dentry) return -ENOENT;
    dentry->GetFullPath(buf, size);
    return reinterpret_cast<long>(buf);
}

long SysCalls::sys_chdir(char *pathname)
{
    BUFFER_CHECK(pathname, String::Size(pathname))
    File *dir = File::Open(pathname, O_DIRECTORY, 0, true);
    if(!dir) return -ENOENT;
    DEntry *dentry = Process::GetCurrentDir();
    if(dentry) FileSystem::PutDEntry(dentry);
    Process::SetCurrentDir(FileSystem::GetDEntry(dir->DEntry));
    delete dir;
    return ESUCCESS;
}

long SysCalls::sys_sysinfo(struct sysinfo *info)
{
    BUFFER_CHECK(info, sizeof(sysinfo))
    info->uptime = static_cast<long>(Time::GetSystemUpTime());
    info->loads[0] = 0;
    info->loads[1] = 0;
    info->loads[2] = 0;
    info->totalram = Paging::GetTotalFrames();
    info->freeram = Paging::GetFreeFrames();
    info->sharedram = 0;
    info->bufferram = 0;
    info->totalswap = 0;
    info->freeswap = 0;
    info->procs = static_cast<unsigned short>(Process::GetCount());
    info->totalhigh = 0;
    info->freehigh = 0;
    info->mem_unit = PAGE_SIZE;
    return 0;
}

long SysCalls::sys_arch_prctl(int code, uintptr_t addr)
{
    switch(code)
    {
    case ARCH_SET_GS:
        cpuWriteMSR(0xC0000101, addr);
        return ESUCCESS;
    case ARCH_SET_FS:
        cpuWriteMSR(0xC0000100, addr);
        return ESUCCESS;
    case ARCH_GET_FS:
        if(!addr) return -EINVAL;
        *((uintptr_t *)(addr)) = cpuReadMSR(0xC0000100);
        return ESUCCESS;
    case ARCH_GET_GS:
        if(!addr) return -EINVAL;
        *((uintptr_t *)(addr)) = cpuReadMSR(0xC0000101);
        return ESUCCESS;
    }
    DEBUG("[syscalls] sys_arch_prctl: unknown code %p\n", code);
    return -ENOSYS;
}

long SysCalls::sys_getdents64(int fd, struct dirent *de, size_t count)
{
    BUFFER_CHECK(de, count)

    File *f = (File *)Process::GetCurrent()->GetHandleData(fd, Process::Handle::HandleType::File);
    if(!f) return -EBADF;
    if(!S_ISDIR(f->Mode))
        return -ENOTDIR;
    DirectoryEntry *d = f->ReadDir();
    if(!d)
    {
        delete d;
        return 0;
    }
    size_t nameLen = String::Length(d->Name);
    size_t recLen = sizeof(dirent) + nameLen + 1;
    if(count < recLen)
    {
        delete d;
        return -EINVAL; // buffer too small
    }
    de->d_ino = d->INode;
    de->d_off = f->Position;
    de->d_reclen = recLen;
    String::Copy(de->d_name, d->Name, nameLen + 1);
    delete d;
    return recLen;
}

long SysCalls::sys_set_tid_address(int *tidptr)
{
    BUFFER_CHECK(tidptr, sizeof(*tidptr))

    Thread *ct = Thread::GetCurrent();
    ct->tidPtr = tidptr;
    return ct->Id;
}

long SysCalls::sys_clock_get_time(int clock, struct timespec *t)
{
    BUFFER_CHECK(t, sizeof(*t))
    if(!t) return -EINVAL;
    switch(clock)
    {
    case CLOCK_REALTIME:
        t->tv_sec = Time::GetTime();
        t->tv_nsec = 0;
        break;
    case CLOCK_MONOTONIC:
    {
        uint64_t ticks = Time::GetTickCount();
        uint64_t tickFreq = Time::GetTickFrequency();
        t->tv_sec = ticks / tickFreq;
        uint64_t rem = ticks % tickFreq;
        t->tv_nsec = (rem * 1000000000) / tickFreq;
        break;
    }
    default:
        return -ENOSYS;
    }
    return ESUCCESS;
}

long SysCalls::sys_exit_group(intn retVal)
{
    Process::Finalize(0, retVal);
    return ESUCCESS;
}

long SysCalls::sys_pipe2(int *fds, int flags)
{
    BUFFER_CHECK(fds, sizeof(*fds) * 2)
    if(flags) DEBUG("[syscalls] WARNING: sys_pipe2 with flags != 0 (%.8x) not implemented\n", flags);
    return sys_pipe(fds);
}

long SysCalls::sysFBGetCount()
{
    return FrameBuffer::GetCount();
}

long SysCalls::sysFBGetDefault()
{
    return FrameBuffer::GetDefaultId();
}

long SysCalls::sysFBListIds(int *buf, size_t bufSize)
{
    BUFFER_CHECK(buf, bufSize * sizeof(*buf))
    return FrameBuffer::ListIds(buf, bufSize);
}

long SysCalls::sysFBGetName(int id, char *buf, size_t bufSize)
{
    BUFFER_CHECK(buf, bufSize * sizeof(*buf))
    FrameBuffer *fb = FrameBuffer::GetById(id);
    if(!fb) return -ENODEV;
    const char *fbName = fb->GetName();
    if(!fbName) fbName = "";
    StringBuilder sb(buf, bufSize);
    sb.WriteFmt("%s", fbName);
    return sb.Length();
}

long SysCalls::sysFBOpen(int id)
{
    FrameBuffer *fb = FrameBuffer::GetById(id);
    if(!fb) return -ENODEV;
    int res = fb->Open();
    if(res < 0) return res;
    Process *cp = Process::GetCurrent();
    res = cp->NewHandle(fb);
    if(res < 0) fb->Close();
    return res;
}

long SysCalls::sysFBClose(int fd)
{
    return Process::GetCurrent()->Close(fd);
}

long SysCalls::sysFBGetModeCount(int fd)
{
    FrameBuffer *fb = (FrameBuffer *)Process::GetCurrent()->GetHandleData(fd, Process::Handle::HandleType::FrameBuffer);
    if(!fb) return -EBADF;
    return fb->GetModeCount();
}

long SysCalls::sysFBGetModeInfo(int fd, int mode, struct vidModeInfo *modeInfo)
{
    BUFFER_CHECK(modeInfo, sizeof(*modeInfo))
    if(!modeInfo) return -EINVAL;

    FrameBuffer *fb = (FrameBuffer *)Process::GetCurrent()->GetHandleData(fd, Process::Handle::HandleType::FrameBuffer);
    if(!fb) return -EBADF;

    FrameBuffer::ModeInfo mi;
    int res = fb->GetModeInfo(mode, &mi);
    if(res < 0) return res;

    modeInfo->Width = mi.Width;
    modeInfo->Height = mi.Height;
    modeInfo->BitsPerPixel = mi.BitsPerPixel;
    modeInfo->RefreshRate = mi.RefreshRate;
    modeInfo->Pitch = mi.Pitch;
    modeInfo->Flags = mi.Flags; // that's a bit dodgy
    modeInfo->AlphaBits = mi.AlphaBits;
    modeInfo->RedBits = mi.RedBits;
    modeInfo->GreenBits = mi.GreenBits;
    modeInfo->BlueBits = mi.BlueBits;
    modeInfo->AlphaShift = mi.AlphaShift;
    modeInfo->RedShift = mi.RedShift;
    modeInfo->GreenShift = mi.GreenShift;
    modeInfo->BlueShift = mi.BlueShift;

    return ESUCCESS;
}

long SysCalls::sysFBSetMode(int fd, int mode)
{
    FrameBuffer *fb = (FrameBuffer *)Process::GetCurrent()->GetHandleData(fd, Process::Handle::HandleType::FrameBuffer);
    if(!fb) return -EBADF;

    return fb->SetMode(mode);
}

long SysCalls::sysFBMapPixels(int fd, uintptr_t hint)
{
    FrameBuffer *fb = (FrameBuffer *)Process::GetCurrent()->GetHandleData(fd, Process::Handle::HandleType::FrameBuffer);
    if(!fb) return -EBADF;

    uintptr_t startVA = hint;

    FrameBuffer::ModeInfo mi;
    fb->GetModeInfo(fb->GetCurrentMode(), &mi);
    size_t fbSize = align(mi.Pitch * mi.Height, PAGE_SIZE);

    Process *cp = Process::GetCurrent();
    cp->MemoryLock.Acquire(0, false);

    if(!startVA)
        startVA = cp->SBrk(fbSize, false);
    uintptr_t endVA = startVA + fbSize;

    uintptr_t pa = fb->GetBuffer();

    for(uintptr_t va = startVA; va < endVA; va += PAGE_SIZE, pa += PAGE_SIZE)
    {
        if(va >= USER_END)
            break;
        Paging::MapPage(cp->AddressSpace, va, pa, true, true);
    }
    cp->MemoryLock.Release();
    return startVA;
}

long SysCalls::sysFBGetCurrentMode(int fd)
{
    FrameBuffer *fb = (FrameBuffer *)Process::GetCurrent()->GetHandleData(fd, Process::Handle::HandleType::FrameBuffer);
    if(!fb) return -EBADF;

    return fb->GetCurrentMode();
}

long SysCalls::sysInDevGetCount()
{
    return InputDevice::GetCount();
}

long SysCalls::sysInDevListIds(int *buf, size_t bufSize)
{
    BUFFER_CHECK(buf, bufSize * sizeof(*buf))
    return InputDevice::ListIds(buf, bufSize);
}

long SysCalls::sysInDevGetType(int id)
{
    InputDevice *dev = InputDevice::GetById(id);
    return dev ? (long)dev->GetType() : -ENODEV;
}

long SysCalls::sysInDevGetName(int id, char *buf, size_t bufSize)
{
    BUFFER_CHECK(buf, bufSize)
    InputDevice *dev = InputDevice::GetById(id);
    if(!dev) return -ENODEV;
    const char *devName = dev->GetName();
    if(!devName) devName = "";
    StringBuilder sb(buf, bufSize);
    sb.WriteFmt("%s", devName);
    return sb.Length();
}

long SysCalls::sysInDevOpen(int id)
{
    InputDevice *dev = InputDevice::GetById(id);
    if(!dev) return -ENODEV;
    int res = dev->Open();
    if(res < 0) return res;
    Process *cp = Process::GetCurrent();
    res = cp->NewHandle(dev);
    if(res < 0) dev->Close();
    return res;
}

long SysCalls::sysInDevClose(int fd)
{
    return Process::GetCurrent()->Close(fd);
}

long SysCalls::sysInDevGetEvent(int fd, int timeout, struct inpEvent *event)
{
    BUFFER_CHECK(event, PAGE_SIZE)
    if(!event) return -EINVAL;
    InputDevice *dev = (InputDevice *)Process::GetCurrent()->GetHandleData(fd, Process::Handle::HandleType::InputDevice);
    if(!dev) return -EBADF;
    InputDevice::Event ev;
    int res = dev->GetEvent(&ev, timeout);
    if(res >= 0) Memory::Move(event, &ev.RawData, sizeof(ev.RawData));
    return res;
}

long SysCalls::sysThreadCreate(const char *name, void *entry, uintptr_t arg, int *retVal)
{
    BUFFER_CHECK(name, String::Size(name))
    BUFFER_CHECK(entry, PAGE_SIZE)
    BUFFER_CHECK(retVal, sizeof(*retVal))
    return Process::GetCurrent()->NewThread(name, entry, arg, retVal);
}

long SysCalls::sysThreadDelete(int tid)
{
    Thread *t = Thread::GetByID(tid);
    if(!t) return -ESRCH;
    Thread::Finalize(t, -127);
    delete t;
    return ESUCCESS;
}

long SysCalls::sysThreadResume(int tid)
{
    Thread *t = Thread::GetByID(tid);
    if(!t) return -ESRCH;
    int res = t->Resume(false) ? ESUCCESS : -EINVAL;
    if(res < 0) return res;
    res = t->Initialized->Wait(0, false, false);
    if(res < 0) return res;
    t->Initialized->Signal(nullptr);
    return ESUCCESS;
}

long SysCalls::sysThreadSuspend(int tid)
{
    Thread *t = Thread::GetByID(tid);
    if(!t) return -ESRCH;
    t->Suspend();
    return ESUCCESS;
}

long SysCalls::sysThreadSleep(int tid, int ms)
{
    Thread *t = Thread::GetByID(tid);
    if(!t) return -ESRCH;
    return t->Sleep(ms, false);
}

long SysCalls::sysThreadWait(int tid, int timeout)
{
    Thread *t = Thread::GetByID(tid);
    if(!t) return -ESRCH;
    int timeleft = t->Finished->Wait(timeout < 0 ? 0 : timeout, timeout == 0, false);
    return timeleft >= 0 ? timeleft : -EBUSY;
}

long SysCalls::sysThreadAbort(int tid, int retVal)
{
    Thread *t = Thread::GetByID(tid);
    if(!t) return -ESRCH;
    t->Finalize(t, retVal);
    return ESUCCESS;
}

long SysCalls::sysThreadDaemonize()
{
    Thread::GetCurrent()->Finished->Signal(nullptr);
    return ESUCCESS;
}

long SysCalls::sysThreadGetId()
{
    return Thread::GetCurrent()->Id;
}

long SysCalls::sysProcessCreate(const char *cmdline)
{
    BUFFER_CHECK(cmdline, String::Size(cmdline))
    Process *p = Process::Create(cmdline, nullptr, false, nullptr);
    if(!p) return -EAGAIN;
    p->Start();
    return p->Id;
}

long SysCalls::sysProcessDelete(int pid)
{
    Process *p = Process::GetByID(pid);
    if(!p) return -ESRCH;
    delete p;
    return ESUCCESS;
}

long SysCalls::sysProcessWait(int pid, int timeout)
{
    Process *p = Process::GetByID(pid);
    if(!p) return -ESRCH;
    int timeleft = p->Finished->Wait(timeout < 0 ? 0 : timeout, timeout == 0, false);
    return timeleft >= 0 ? timeleft : -EBUSY;
}

long SysCalls::sysProcessAbort(int pid, int result)
{
    Process::Finalize(pid, result);
    return ESUCCESS;
}

long SysCalls::sysProcessListIds(int *buf, size_t bufSize)
{
    BUFFER_CHECK(buf, bufSize * sizeof(*buf))
    return Process::ListIds(buf, bufSize);
}

long SysCalls::sysProcessGetName(int pid, char *buf, size_t bufSize)
{
    BUFFER_CHECK(buf, bufSize * sizeof(*buf))
    return Process::GetName(pid, buf, bufSize);
}

long SysCalls::sysProcessGetThreadCount(int pid)
{
    Process *p = Process::GetByID(pid);
    if(!p) return -ESRCH;
    if(!p->Lock())
        return -EBUSY;
    int res = p->Threads.Count();
    p->UnLock();
    return res;
}

long SysCalls::sysProcessGetUsedMemory(int pid)
{
    Process *p = Process::GetByID(pid);
    if(!p) return -ESRCH;
    return Paging::CountPresentPages(p->AddressSpace, 0, USER_END) << PAGE_SHIFT;
}

long SysCalls::sysProcessGetExecPath(int pid, char *buf, size_t bufSize)
{
    BUFFER_CHECK(buf, bufSize)

    Process *p = Process::GetByID(pid);
    if(!p) return -ESRCH;

    char *execName = p->GetExecName();
    String::Copy(buf, execName, bufSize);
    return ESUCCESS;
}

long SysCalls::sysProcessGetMap(int pid, struct processMapEntry *buf, size_t bufSize)
{
    BUFFER_CHECK(buf, bufSize)

    Process *p = Process::GetByID(pid);
    if(!p) return -ESRCH;

    static const size_t sizeOfEntryStruct = offsetof(processMapEntry_t, Name);

    uintptr_t bufAddr = reinterpret_cast<uintptr_t>(buf);
    size_t currEntryOffs = 0;
    processMapEntry_t *entry = nullptr;
    for(ELF *elf : p->Images)
    {
        char *elfName = elf->FullPath;
        size_t nameLen = String::Length(elfName);
        size_t thisEntrySize = sizeOfEntryStruct + (elfName ? nameLen + 1 : 0);

        Elf_Ehdr *ehdr = elf->GetEHdr();
        uintptr_t phdrAddr = reinterpret_cast<uintptr_t>(elf->GetPHdr());
        for(int i = 0; i < ehdr->e_phnum; ++i, phdrAddr += ehdr->e_phentsize)
        {
            Elf_Phdr *phdr = reinterpret_cast<Elf_Phdr *>(phdrAddr);
            if(phdr->p_type != PT_LOAD)
                continue;

            entry = reinterpret_cast<processMapEntry_t *>(bufAddr + currEntryOffs);
            currEntryOffs += thisEntrySize;

            if(currEntryOffs > bufSize)
                return -ENOMEM; // buffer too small

            entry->EntrySize = thisEntrySize;
            entry->Address = elf->GetBase() + phdr->p_vaddr;
            entry->Size = phdr->p_memsz;
            entry->Offset = phdr->p_offset;
            entry->Flags = PROC_MAP_READ | PROC_MAP_WRITE | PROC_MAP_EXEC | (elfName ? PROC_MAP_NAME : 0);
            if(elfName) Memory::Move(entry->Name, elfName, nameLen + 1);
        }
    }
    if(entry) entry->Flags |= PROC_MAP_LAST;

    return ESUCCESS;
}

long SysCalls::sysIPCSendMessage(int dst, int num, int flags, void *payload, unsigned payloadSize)
{
    BUFFER_CHECK(payload, payloadSize);
    return IPC::SendMessage(dst, num, flags, payload, payloadSize);
}

long SysCalls::sysIPCGetMessage(void *msg, int timeout)
{
    BUFFER_CHECK(msg, sizeof(ipcMessage))
    return IPC::GetMessage((ipcMessage *)msg, timeout);
}

long SysCalls::sysIPCCreateSharedMem(const char *name, unsigned size)
{
    BUFFER_CHECK(name, String::Size(name))
    if(!size) return -EINVAL;
    NamedSharedMem *shm = new NamedSharedMem(name, align(size, PAGE_SIZE), false);
    return Process::GetCurrent()->CreateNamedObjectHandle(shm);
}

long SysCalls::sysIPCOpenSharedMem(const char *name)
{
    BUFFER_CHECK(name, String::Size(name))
    NamedObject *no = NamedObject::Get(name);
    if(!no) return -ENOENT;
    NamedSharedMem *shm = dynamic_cast<NamedSharedMem *>(no);
    if(!shm)
    {
        no->Put();
        return -EINVAL;
    }
    int handle = Process::GetCurrent()->CreateNamedObjectHandle(shm);
    if(handle < 0)
        no->Put();
    return handle;
}

long SysCalls::sysIPCCloseSharedMem(int fd)
{
    return Process::GetCurrent()->Close(fd); // TODO: add fd type check
}

long SysCalls::sysIPCGetSharedMemSize(int fd)
{
    NamedObject *no = Process::GetCurrent()->GetNamedObject(fd);
    if(!no) return -EBADF;
    NamedSharedMem *shm = dynamic_cast<NamedSharedMem *>(no);
    if(!shm) return -EBADF;
    return shm->GetSize();
}

long SysCalls::sysIPCMapSharedMem(int fd, uintptr_t hint, unsigned flags)
{
    uintptr_t va = hint;

    Process *cp = Process::GetCurrent();
    NamedObject *no = cp->GetNamedObject(fd);
    if(!no) return -EBADF;
    NamedSharedMem *shm = dynamic_cast<NamedSharedMem *>(no);
    if(!shm) return -EBADF;

    if(!va) va = cp->MMapSBrk(shm->GetSize(), false);

    int res = shm->Map(cp, va, true, flags & 1);
    if(res < 0) return res;

    return va;
}

long SysCalls::sysIPCUnMapSharedMem(int fd, uintptr_t addr)
{
    Process *cp = Process::GetCurrent();
    NamedObject *no = cp->GetNamedObject(fd);
    if(!no) return -EBADF;
    NamedSharedMem *shm = dynamic_cast<NamedSharedMem *>(no);
    if(!shm) return -EBADF;

    return shm->UnMap(cp, addr);
}

long SysCalls::sysIPCPeekMessage(void *msg, unsigned offset)
{
    BUFFER_CHECK(msg, sizeof(ipcMessage))
    return IPC::PeekMessage((ipcMessage *)msg, offset);
}

long SysCalls::sysIPCWaitMessage(void *msg, int number, int source, int rangeStart, int rangeSize, int timeout)
{
    BUFFER_CHECK(msg, sizeof(ipcMessage))
    return IPC::WaitMessage((ipcMessage *)msg, number, source, rangeStart, rangeSize, timeout);
}

long SysCalls::sysSyncMutexCreate(unsigned flags)
{
    return Process::GetCurrent()->NewMutex(flags & 1);
}

long SysCalls::sysSyncMutexDelete(int fd)
{
    return Process::GetCurrent()->Close(fd); // TODO: add handle type check
}

long SysCalls::sysSyncMutexAcquire(int fd, int timeout)
{
    Mutex *mtx = Process::GetCurrent()->GetMutex(fd);
    if(!mtx) return -EBADF;
    return mtx->Acquire(timeout < 0 ? 0 : timeout, !timeout);
}

long SysCalls::sysSyncMutexRelease(int fd)
{
    Mutex *mtx = Process::GetCurrent()->GetMutex(fd);
    if(!mtx) return -EBADF;
    mtx->Release();
    return ESUCCESS;
}

long SysCalls::sysSyncSemaphoreCreate(int count)
{
    return Process::GetCurrent()->NewSemaphore(count);
}

long SysCalls::sysSyncSemaphoreDelete(int fd)
{
    return Process::GetCurrent()->Close(fd); // TODO: add handle type check
}

long SysCalls::sysSyncSemaphoreWait(int fd, int timeout)
{
    Semaphore *sem = Process::GetCurrent()->GetSemaphore(fd);
    if(!sem) return -EBADF;
    return sem->Wait(timeout < 0 ? 0 : timeout, !timeout, false);

}

long SysCalls::sysSyncSemaphoreSignal(int fd)
{
    Semaphore *sem = Process::GetCurrent()->GetSemaphore(fd);
    if(!sem) return -EBADF;
    sem->Signal(nullptr);
    return ESUCCESS;
}

long SysCalls::sysSignalGetHandler(unsigned signum)
{
    return signum < SIGNAL_COUNT ? reinterpret_cast<long>(Thread::GetCurrent()->SignalHandlers[signum]) : 0;
}

long SysCalls::sysSignalSetHandler(unsigned signum, void *handler)
{
    if(signum >= SIGNAL_COUNT)
        return -EINVAL;
    Thread::GetCurrent()->SignalHandlers[signum] = handler;
    return ESUCCESS;
}

long SysCalls::sysSignalIsEnabled(unsigned signum)
{
    if(signum >= SIGNAL_COUNT)
        return -EINVAL;
    return ((decltype(Thread::SignalMask)(1)) << signum) & Thread::GetCurrent()->SignalMask ? 1 : 0;
}

long SysCalls::sysSignalEnable(unsigned signum)
{
    if(signum >= SIGNAL_COUNT)
        return -EINVAL;
    Thread::GetCurrent()->SignalMask |= (decltype(Thread::SignalMask)(1)) << signum;
    return ESUCCESS;
}

long SysCalls::sysSignalDisable(unsigned signum)
{
    if(signum >= SIGNAL_COUNT)
        return -EINVAL;
    Thread::GetCurrent()->SignalMask &= ~((decltype(Thread::SignalMask)(1)) << signum);
    return ESUCCESS;
}

long SysCalls::sysSignalRaise(pid_t tid, unsigned signum)
{
    Thread *t = tid == -1 ? Thread::GetCurrent() : Thread::GetByID(tid);
    if(!t) return -ESRCH;
    if(signum >= SIGNAL_COUNT)
        return -EINVAL;
    Signal::Raise(t, signum);
    return ESUCCESS;
}

long SysCalls::sysSignalReturn()
{
    Thread *ct = Thread::GetCurrent();

    // this call is invalid outside signal handler
    if(ct->CurrentSignal < 0)
        return -EINVAL;

    // HACKHACK: modifying syscall return address
    uintptr_t *thisStackFrame = reinterpret_cast<uintptr_t *>(__builtin_frame_address(0));
    uintptr_t *prevStackFrame = reinterpret_cast<uintptr_t *>(*thisStackFrame);
    prevStackFrame[1] = ct->SignalRetAddr;
    ct->Suspend();  // Resume called from Signal::Raise should keep
                    // thread from actually suspending thanks to WakeCount

    ct->CurrentSignal = -1;
    return ESUCCESS;
}

long SysCalls::sysSignalGetCurrent()
{
    return Thread::GetCurrent()->CurrentSignal;
}

void SysCalls::Initialize()
{
    Memory::Zero(Handlers, sizeof(Handlers));

    // GCC doesn't support nontrivial initializer so it has to be like that
    Handlers[SYS_read] = reinterpret_cast<SysCallHandler>(sys_read);
    Handlers[SYS_write] = reinterpret_cast<SysCallHandler>(sys_write);
    Handlers[SYS_open] = reinterpret_cast<SysCallHandler>(sys_open);
    Handlers[SYS_close] = reinterpret_cast<SysCallHandler>(sys_close);
    Handlers[SYS_stat] = reinterpret_cast<SysCallHandler>(sys_stat);
    Handlers[SYS_fstat] = reinterpret_cast<SysCallHandler>(sys_fstat);
    Handlers[SYS_lstat] = reinterpret_cast<SysCallHandler>(sys_lstat);
    Handlers[SYS_lseek] = reinterpret_cast<SysCallHandler>(sys_lseek);
    Handlers[SYS_mmap] = reinterpret_cast<SysCallHandler>(sys_mmap);
    Handlers[SYS_mprotect] = reinterpret_cast<SysCallHandler>(sys_mprotect);
    Handlers[SYS_munmap] = reinterpret_cast<SysCallHandler>(sys_munmap);
    Handlers[SYS_brk] = reinterpret_cast<SysCallHandler>(sys_brk);
    Handlers[SYS_rt_sigprocmask] = reinterpret_cast<SysCallHandler>(sys_rt_sigprocmask);
    Handlers[SYS_rt_sigreturn] = reinterpret_cast<SysCallHandler>(sys_rt_sigreturn);
    Handlers[SYS_readv] = reinterpret_cast<SysCallHandler>(sys_readv);
    Handlers[SYS_writev] = reinterpret_cast<SysCallHandler>(sys_writev);
    Handlers[SYS_pipe] = reinterpret_cast<SysCallHandler>(sys_pipe);
    Handlers[SYS_msync] = reinterpret_cast<SysCallHandler>(sys_msync);
    Handlers[SYS_mincore] = reinterpret_cast<SysCallHandler>(sys_mincore);
    Handlers[SYS_dup] = reinterpret_cast<SysCallHandler>(sys_dup);
    Handlers[SYS_dup2] = reinterpret_cast<SysCallHandler>(sys_dup2);
    Handlers[SYS_getpid] = reinterpret_cast<SysCallHandler>(sys_getpid);
    Handlers[SYS_exit] = reinterpret_cast<SysCallHandler>(sys_exit);
    Handlers[SYS_getdents] = reinterpret_cast<SysCallHandler>(sys_getdents);
    Handlers[SYS_getcwd] = reinterpret_cast<SysCallHandler>(sys_getcwd);
    Handlers[SYS_chdir] = reinterpret_cast<SysCallHandler>(sys_chdir);
    Handlers[SYS_sysinfo] = reinterpret_cast<SysCallHandler>(sys_sysinfo);
    Handlers[SYS_arch_prctl] = reinterpret_cast<SysCallHandler>(sys_arch_prctl);
    Handlers[SYS_getdents64] = reinterpret_cast<SysCallHandler>(sys_getdents64);
    Handlers[SYS_set_tid_address] = reinterpret_cast<SysCallHandler>(sys_set_tid_address);
    Handlers[SYS_clock_get_time] = reinterpret_cast<SysCallHandler>(sys_clock_get_time);
    Handlers[SYS_exit_group] = reinterpret_cast<SysCallHandler>(sys_exit_group);
    Handlers[SYS_pipe2] = reinterpret_cast<SysCallHandler>(sys_pipe2);

    Handlers[SYS_FB_GET_COUNT] = reinterpret_cast<SysCallHandler>(sysFBGetCount);
    Handlers[SYS_FB_GET_DEFAULT] = reinterpret_cast<SysCallHandler>(sysFBGetDefault);
    Handlers[SYS_FB_LIST_IDS] = reinterpret_cast<SysCallHandler>(sysFBListIds);
    Handlers[SYS_FB_GET_NAME] = reinterpret_cast<SysCallHandler>(sysFBGetName);
    Handlers[SYS_FB_OPEN] = reinterpret_cast<SysCallHandler>(sysFBOpen);
    Handlers[SYS_FB_CLOSE] = reinterpret_cast<SysCallHandler>(sysFBClose);
    Handlers[SYS_FB_GET_MODE_COUNT] = reinterpret_cast<SysCallHandler>(sysFBGetModeCount);
    Handlers[SYS_FB_GET_MODE_INFO] = reinterpret_cast<SysCallHandler>(sysFBGetModeInfo);
    Handlers[SYS_FB_SET_MODE] = reinterpret_cast<SysCallHandler>(sysFBSetMode);
    Handlers[SYS_FB_MAP_PIXELS] = reinterpret_cast<SysCallHandler>(sysFBMapPixels);
    Handlers[SYS_FB_GET_CURRENT_MODE] = reinterpret_cast<SysCallHandler>(sysFBGetCurrentMode);

    Handlers[SYS_INDEV_GET_COUNT] = reinterpret_cast<SysCallHandler>(sysInDevGetCount);
    Handlers[SYS_INDEV_LIST_IDS] = reinterpret_cast<SysCallHandler>(sysInDevListIds);
    Handlers[SYS_INDEV_GET_TYPE] = reinterpret_cast<SysCallHandler>(sysInDevGetType);
    Handlers[SYS_INDEV_GET_NAME] = reinterpret_cast<SysCallHandler>(sysInDevGetName);
    Handlers[SYS_INDEV_OPEN] = reinterpret_cast<SysCallHandler>(sysInDevOpen);
    Handlers[SYS_INDEV_CLOSE] = reinterpret_cast<SysCallHandler>(sysInDevClose);
    Handlers[SYS_INDEV_GET_EVENT] = reinterpret_cast<SysCallHandler>(sysInDevGetEvent);

    Handlers[SYS_THREAD_CREATE] = reinterpret_cast<SysCallHandler>(sysThreadCreate);
    Handlers[SYS_THREAD_DELETE] = reinterpret_cast<SysCallHandler>(sysThreadDelete);
    Handlers[SYS_THREAD_RESUME] = reinterpret_cast<SysCallHandler>(sysThreadResume);
    Handlers[SYS_THREAD_SUSPEND] = reinterpret_cast<SysCallHandler>(sysThreadSuspend);
    Handlers[SYS_THREAD_SLEEP] = reinterpret_cast<SysCallHandler>(sysThreadSleep);
    Handlers[SYS_THREAD_WAIT] = reinterpret_cast<SysCallHandler>(sysThreadWait);
    Handlers[SYS_THREAD_ABORT] = reinterpret_cast<SysCallHandler>(sysThreadAbort);
    Handlers[SYS_THREAD_DAEMONIZE] = reinterpret_cast<SysCallHandler>(sysThreadDaemonize);
    Handlers[SYS_THREAD_GET_ID] = reinterpret_cast<SysCallHandler>(sysThreadGetId);

    Handlers[SYS_PROCESS_CREATE] = reinterpret_cast<SysCallHandler>(sysProcessCreate);
    Handlers[SYS_PROCESS_DELETE] = reinterpret_cast<SysCallHandler>(sysProcessDelete);
    Handlers[SYS_PROCESS_WAIT] = reinterpret_cast<SysCallHandler>(sysProcessWait);
    Handlers[SYS_PROCESS_ABORT] = reinterpret_cast<SysCallHandler>(sysProcessAbort);
    Handlers[SYS_PROCESS_LIST_IDS] = reinterpret_cast<SysCallHandler>(sysProcessListIds);
    Handlers[SYS_PROCESS_GET_NAME] = reinterpret_cast<SysCallHandler>(sysProcessGetName);
    Handlers[SYS_PROCESS_GET_THREAD_COUNT] = reinterpret_cast<SysCallHandler>(sysProcessGetThreadCount);
    Handlers[SYS_PROCESS_GET_USED_MEMORY] = reinterpret_cast<SysCallHandler>(sysProcessGetUsedMemory);
    Handlers[SYS_PROCESS_GET_EXEC_PATH] = reinterpret_cast<SysCallHandler>(sysProcessGetExecPath);
    Handlers[SYS_PROCESS_GET_MAP] = reinterpret_cast<SysCallHandler>(sysProcessGetMap);

    Handlers[SYS_IPC_SEND_MESSAGE] = reinterpret_cast<SysCallHandler>(sysIPCSendMessage);
    Handlers[SYS_IPC_GET_MESSAGE] = reinterpret_cast<SysCallHandler>(sysIPCGetMessage);
    Handlers[SYS_IPC_CREATE_SHMEM] = reinterpret_cast<SysCallHandler>(sysIPCCreateSharedMem);
    Handlers[SYS_IPC_OPEN_SHMEM] = reinterpret_cast<SysCallHandler>(sysIPCOpenSharedMem);
    Handlers[SYS_IPC_CLOSE_SHMEM] = reinterpret_cast<SysCallHandler>(sysIPCCloseSharedMem);
    Handlers[SYS_IPC_GET_SHMEM_SIZE] = reinterpret_cast<SysCallHandler>(sysIPCGetSharedMemSize);
    Handlers[SYS_IPC_MAP_SHMEM] = reinterpret_cast<SysCallHandler>(sysIPCMapSharedMem);
    Handlers[SYS_IPC_UNMAP_SHMEM] = reinterpret_cast<SysCallHandler>(sysIPCUnMapSharedMem);
    Handlers[SYS_IPC_PEEK_MESSAGE] = reinterpret_cast<SysCallHandler>(sysIPCPeekMessage);
    Handlers[SYS_IPC_WAIT_MESSAGE] = reinterpret_cast<SysCallHandler>(sysIPCWaitMessage);

    Handlers[SYS_SYNC_MUTEX_CREATE] = reinterpret_cast<SysCallHandler>(sysSyncMutexCreate);
    Handlers[SYS_SYNC_MUTEX_DELETE] = reinterpret_cast<SysCallHandler>(sysSyncMutexDelete);
    Handlers[SYS_SYNC_MUTEX_ACQUIRE] = reinterpret_cast<SysCallHandler>(sysSyncMutexAcquire);
    Handlers[SYS_SYNC_MUTEX_RELEASE] = reinterpret_cast<SysCallHandler>(sysSyncMutexRelease);
    Handlers[SYS_SYNC_SEMAPHORE_CREATE] = reinterpret_cast<SysCallHandler>(sysSyncSemaphoreCreate);
    Handlers[SYS_SYNC_SEMAPHORE_DELETE] = reinterpret_cast<SysCallHandler>(sysSyncSemaphoreDelete);
    Handlers[SYS_SYNC_SEMAPHORE_WAIT] = reinterpret_cast<SysCallHandler>(sysSyncSemaphoreWait);
    Handlers[SYS_SYNC_SEMAPHORE_SIGNAL] = reinterpret_cast<SysCallHandler>(sysSyncSemaphoreSignal);

    Handlers[SYS_SIGNAL_GET_HANDLER] = reinterpret_cast<SysCallHandler>(sysSignalGetHandler);
    Handlers[SYS_SIGNAL_SET_HANDLER] = reinterpret_cast<SysCallHandler>(sysSignalSetHandler);
    Handlers[SYS_SIGNAL_IS_ENABLED] = reinterpret_cast<SysCallHandler>(sysSignalIsEnabled);
    Handlers[SYS_SIGNAL_ENABLE] = reinterpret_cast<SysCallHandler>(sysSignalEnable);
    Handlers[SYS_SIGNAL_DISABLE] = reinterpret_cast<SysCallHandler>(sysSignalDisable);
    Handlers[SYS_SIGNAL_RAISE] = reinterpret_cast<SysCallHandler>(sysSignalRaise);
    Handlers[SYS_SIGNAL_RETURN] = reinterpret_cast<SysCallHandler>(sysSignalReturn);
    Handlers[SYS_SIGNAL_GET_CURRENT] = reinterpret_cast<SysCallHandler>(sysSignalGetCurrent);

    cpuWriteMSR(0xC0000081,
                static_cast<uintptr_t>(SEG_KERNEL_DATA) << 48 |
                static_cast<uintptr_t>(SEG_KERNEL_CODE) << 32);

    cpuWriteMSR(0xC0000082, reinterpret_cast<uintptr_t>(syscallHandler));
    cpuWriteMSR(0xC0000084, 0x00000200);
}

