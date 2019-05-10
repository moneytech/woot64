#include <cpu.hpp>
#include <debug.hpp>
#include <dentry.hpp>
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
#include <string.hpp>
#include <stringbuilder.hpp>
#include <syscalls.hpp>
#include <sysdefs.h>
#include <thread.hpp>

#define ARCH_SET_GS		0x1001
#define ARCH_SET_FS		0x1002
#define ARCH_GET_FS		0x1003
#define ARCH_GET_GS		0x1004

extern "C" void syscallHandler();
asm(
INLINE_ASM_SYNTAX
".extern mainTSS\n"
".type syscallHandler, function\n"
"syscallHandler:\n"
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
"sysret\n"                                  // return to usermode code
NORMAL_ASM_SYNTAX
);

struct iovec
{
    void *iov_base;     /* Starting address */
    size_t iov_len;     /* Number of bytes to transfer */
};

#define CLOCK_REALTIME           0

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

SysCalls::SysCallHandler SysCalls::Handlers[1024];

long SysCalls::InvalidHandler()
{
    // not sure if this is stable
    uintn number; asm volatile("": "=a"(number));
    DEBUG("[syscalls] Unknown syscall %d\n", number);
    return -ENOSYS;
}

long SysCalls::sys_read(int fd, char *buf, size_t count)
{
    if(fd < 3) return Debug::DebugRead(buf, count); // temporary hack
    File *f = (File *)Process::GetCurrent()->GetHandleData(fd, Process::Handle::HandleType::File);
    if(!f) return -EBADF;
    return f->Read(buf, count);
}

long SysCalls::sys_write(int fd, const char *buf, size_t count)
{
    if(fd < 3) return Debug::DebugWrite(buf, count); // temporary hack
    File *f = (File *)Process::GetCurrent()->GetHandleData(fd, Process::Handle::HandleType::File);
    if(!f) return -EBADF;
    return f->Write(buf, count);
}

long SysCalls::sys_open(const char *filename, int flags, int mode)
{
    int res = Process::GetCurrent()->Open(filename, flags);
    DEBUG("sys_open(\"%s\", %p) returned %d\n", filename, flags, res);
    return res;
}

long SysCalls::sys_close(int fd)
{
    return Process::GetCurrent()->Close(fd);
}

long SysCalls::sys_stat(const char *filename, stat *statbuf)
{
    Memory::Zero(statbuf, sizeof(struct stat));
    File *f = File::Open(filename, 0);
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

long SysCalls::sys_lseek(int fd, off_t offset, unsigned int origin)
{
    File *f = (File *)Process::GetCurrent()->GetHandleData(fd, Process::Handle::HandleType::File);
    if(!f) return -errno;
    return f->Seek(offset, origin);
}

long SysCalls::sys_mmap(uintptr_t addr, unsigned long len, int prot, int flags, int fd, off_t off)
{
    //DEBUG("sys_mmap(%p, %p, %p, %p, %d, %p)\n", addr, len, prot, flags, fd, off);

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

    return addr;
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
    return Process::GetCurrent()->Brk(brk, true);
}

long SysCalls::sys_readv(int fd, const iovec *vec, size_t vlen)
{
    if(vlen < 0) return -EINVAL;
    long res = 0;
    if(fd < 3)
    {   // temporary hack
        for(int i = 0; i < vlen; ++i)
        {
            long r = Debug::DebugRead(vec[i].iov_base, vec[i].iov_len);
            if(r < 0) return r;
            res += r;
        }
    }
    else
    {
        File *f = (File *)Process::GetCurrent()->GetHandleData(fd, Process::Handle::HandleType::File);
        if(!f) return -EBADF;
        for(int i = 0; i < vlen; ++i)
        {
            long r = f->Read(vec[i].iov_base, vec[i].iov_len);
            if(r < 0) return r;
            res += r;
        }
    }
    return res;
}

long SysCalls::sys_writev(int fd, const iovec *vec, size_t vlen)
{
    if(vlen < 0) return -EINVAL;
    long res = 0;
    if(fd < 3)
    {   // temporary hack
        for(int i = 0; i < vlen; ++i)
        {
            long r = Debug::DebugWrite(vec[i].iov_base, vec[i].iov_len);
            if(r < 0) return r;
            res += r;
        }
    }
    else
    {
        File *f = (File *)Process::GetCurrent()->GetHandleData(fd, Process::Handle::HandleType::File);
        if(!f) return -EBADF;
        for(int i = 0; i < vlen; ++i)
        {
            long r = f->Write(vec[i].iov_base, vec[i].iov_len);
            if(r < 0) return r;
            res += r;
        }
    }
    return res;
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

long SysCalls::sys_getcwd(char *buf, size_t size)
{
    DEntry *dentry = Process::GetCurrentDir();
    if(!dentry) return -ENOENT;
    dentry->GetFullPath(buf, size);
    return (long)buf;
}

long SysCalls::sys_chdir(char *pathname)
{
    File *dir = File::Open(pathname, O_DIRECTORY);
    if(!dir) return -ENOENT;
    DEntry *dentry = Process::GetCurrentDir();
    if(dentry) FileSystem::PutDEntry(dentry);
    Process::SetCurrentDir(FileSystem::GetDEntry(dir->DEntry));
    delete dir;
    return ESUCCESS;
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

long SysCalls::sys_set_tid_address(int *tidptr)
{
    Thread *ct = Thread::GetCurrent();
    ct->tidPtr = tidptr;
    return ct->Id;
}

long SysCalls::sys_clock_get_time(int clock, struct timespec *t)
{
    if(clock != CLOCK_REALTIME)
        return -ENOSYS;
    if(!t) return -EINVAL;
    t->tv_sec = Time::GetTime();
    t->tv_nsec = 0;
    return ESUCCESS;
}

long SysCalls::sys_exit_group(intn retVal)
{
    Process::Finalize(0, retVal);
    return ESUCCESS;
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
    return FrameBuffer::ListIds(buf, bufSize);
}

long SysCalls::sysFBGetName(int id, char *buf, size_t bufSize)
{
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
    return InputDevice::ListIds(buf, bufSize);
}

long SysCalls::sysInDevGetType(int id)
{
    InputDevice *dev = InputDevice::GetById(id);
    return dev ? (long)dev->GetType() : -ENODEV;
}

long SysCalls::sysInDevGetName(int id, char *buf, size_t bufSize)
{
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
    return Process::GetCurrent()->NewThread(name, entry, arg, retVal);
}

long SysCalls::sysThreadDelete(int fd)
{
    return Process::GetCurrent()->DeleteThread(fd);
}

long SysCalls::sysThreadResume(int fd)
{
    return Process::GetCurrent()->ResumeThread(fd);
}

long SysCalls::sysThreadSuspend(int fd)
{
    if(fd < 0)
    {
        Thread::GetCurrent()->Suspend();
        return ESUCCESS;
    }
    return Process::GetCurrent()->SuspendThread(fd);
}

long SysCalls::sysThreadSleep(int fd, int ms)
{
    if(fd < 0) return Time::Sleep(ms, false);
    return Process::GetCurrent()->SleepThread(fd, ms);
}

long SysCalls::sysThreadWait(int fd, int timeout)
{
    return Process::GetCurrent()->WaitThread(fd, timeout);
}

long SysCalls::sysThreadAbort(int fd, int retVal)
{
    if(fd < 0) Thread::Finalize(Thread::GetCurrent(), retVal);
    return Process::GetCurrent()->AbortThread(fd, retVal);
}

long SysCalls::sysThreadDaemonize()
{
    Thread::GetCurrent()->Finished->Signal(nullptr);
    return ESUCCESS;
}

long SysCalls::sysThreadGetId(int fd)
{
    if(fd < 0) return Thread::GetCurrent()->Id;
    Thread *t = Process::GetCurrent()->GetThread(fd);
    if(!t) return -EINVAL;
    return t->Id;
}

long SysCalls::sysProcessCreate(const char *cmdline)
{
    return Process::GetCurrent()->NewProcess(cmdline);
}

long SysCalls::sysProcessDelete(int fd)
{
    return Process::GetCurrent()->DeleteProcess(fd);
}

long SysCalls::sysProcessWait(int fd, int timeout)
{
    return Process::GetCurrent()->WaitProcess(fd, timeout);
}

long SysCalls::sysProcessAbort(int fd, int result)
{
    return Process::GetCurrent()->AbortProcess(fd, result);
}

long SysCalls::sysIPCSendMessage(int dst, int num, int flags, void *payload, unsigned payloadSize)
{
    return IPC::SendMessage(dst, num, flags, payload, payloadSize);
}

long SysCalls::sysIPCGetMessage(void *msg, int timeout)
{
    return IPC::GetMessage((ipcMessage *)msg, timeout);
}

long SysCalls::sysIPCCreateSharedMem(const char *name, unsigned size)
{
    if(!size) return -EINVAL;
    NamedSharedMem *shm = new NamedSharedMem(name, align(size, PAGE_SIZE), false);
    return Process::GetCurrent()->CreateNamedObjectHandle(shm);
}

long SysCalls::sysIPCOpenSharedMem(const char *name)
{
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
    return Process::GetCurrent()->Close(fd);
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

void SysCalls::Initialize()
{
    Memory::Zero(Handlers, sizeof(Handlers));

    Handlers[SYS_read] = (SysCallHandler)sys_read;
    Handlers[SYS_write] = (SysCallHandler)sys_write;
    Handlers[SYS_open] = (SysCallHandler)sys_open;
    Handlers[SYS_close] = (SysCallHandler)sys_close;
    Handlers[SYS_stat] = (SysCallHandler)sys_stat;
    Handlers[SYS_fstat] = (SysCallHandler)sys_fstat;
    Handlers[SYS_lseek] = (SysCallHandler)sys_lseek;
    Handlers[SYS_mmap] = (SysCallHandler)sys_mmap;
    Handlers[SYS_munmap] = (SysCallHandler)sys_munmap;
    Handlers[SYS_brk] = (SysCallHandler)sys_brk;
    Handlers[SYS_readv] = (SysCallHandler)sys_readv;
    Handlers[SYS_writev] = (SysCallHandler)sys_writev;
    Handlers[SYS_getpid] = (SysCallHandler)sys_getpid;
    Handlers[SYS_exit] = (SysCallHandler)sys_exit;
    Handlers[SYS_getcwd] = (SysCallHandler)sys_getcwd;
    Handlers[SYS_chdir] = (SysCallHandler)sys_chdir;
    Handlers[SYS_arch_prctl] = (SysCallHandler)sys_arch_prctl;
    Handlers[SYS_set_tid_address] = (SysCallHandler)sys_set_tid_address;
    Handlers[SYS_clock_get_time] = (SysCallHandler)sys_clock_get_time;
    Handlers[SYS_exit_group] = (SysCallHandler)sys_exit_group;

    Handlers[SYS_FB_GET_COUNT] = (SysCallHandler)sysFBGetCount;
    Handlers[SYS_FB_GET_DEFAULT] = (SysCallHandler)sysFBGetDefault;
    Handlers[SYS_FB_LIST_IDS] = (SysCallHandler)sysFBListIds;
    Handlers[SYS_FB_GET_NAME] = (SysCallHandler)sysFBGetName;
    Handlers[SYS_FB_OPEN] = (SysCallHandler)sysFBOpen;
    Handlers[SYS_FB_CLOSE] = (SysCallHandler)sysFBClose;
    Handlers[SYS_FB_GET_MODE_COUNT] = (SysCallHandler)sysFBGetModeCount;
    Handlers[SYS_FB_GET_MODE_INFO] = (SysCallHandler)sysFBGetModeInfo;
    Handlers[SYS_FB_SET_MODE] = (SysCallHandler)sysFBSetMode;
    Handlers[SYS_FB_MAP_PIXELS] = (SysCallHandler)sysFBMapPixels;
    Handlers[SYS_FB_GET_CURRENT_MODE] = (SysCallHandler)sysFBGetCurrentMode;

    Handlers[SYS_INDEV_GET_COUNT] = (SysCallHandler)sysInDevGetCount;
    Handlers[SYS_INDEV_LIST_IDS] = (SysCallHandler)sysInDevListIds;
    Handlers[SYS_INDEV_GET_TYPE] = (SysCallHandler)sysInDevGetType;
    Handlers[SYS_INDEV_GET_NAME] = (SysCallHandler)sysInDevGetName;
    Handlers[SYS_INDEV_OPEN] = (SysCallHandler)sysInDevOpen;
    Handlers[SYS_INDEV_CLOSE] = (SysCallHandler)sysInDevClose;
    Handlers[SYS_INDEV_GET_EVENT] = (SysCallHandler)sysInDevGetEvent;

    Handlers[SYS_THREAD_CREATE] = (SysCallHandler)sysThreadCreate;
    Handlers[SYS_THREAD_DELETE] = (SysCallHandler)sysThreadDelete;
    Handlers[SYS_THREAD_RESUME] = (SysCallHandler)sysThreadResume;
    Handlers[SYS_THREAD_SUSPEND] = (SysCallHandler)sysThreadSuspend;
    Handlers[SYS_THREAD_SLEEP] = (SysCallHandler)sysThreadSleep;
    Handlers[SYS_THREAD_WAIT] = (SysCallHandler)sysThreadWait;
    Handlers[SYS_THREAD_ABORT] = (SysCallHandler)sysThreadAbort;
    Handlers[SYS_THREAD_DAEMONIZE] = (SysCallHandler)sysThreadDaemonize;
    Handlers[SYS_THREAD_GET_ID] = (SysCallHandler)sysThreadGetId;

    Handlers[SYS_PROCESS_CREATE] = (SysCallHandler)sysProcessCreate;
    Handlers[SYS_PROCESS_DELETE] = (SysCallHandler)sysProcessDelete;
    Handlers[SYS_PROCESS_WAIT] = (SysCallHandler)sysProcessWait;
    Handlers[SYS_PROCESS_ABORT] = (SysCallHandler)sysProcessAbort;

    Handlers[SYS_IPC_SEND_MESSAGE] = (SysCallHandler)sysIPCSendMessage;
    Handlers[SYS_IPC_GET_MESSAGE] = (SysCallHandler)sysIPCGetMessage;
    Handlers[SYS_IPC_CREATE_SHMEM] = (SysCallHandler)sysIPCCreateSharedMem;
    Handlers[SYS_IPC_OPEN_SHMEM] = (SysCallHandler)sysIPCOpenSharedMem;
    Handlers[SYS_IPC_CLOSE_SHMEM] = (SysCallHandler)sysIPCCloseSharedMem;
    Handlers[SYS_IPC_GET_SHMEM_SIZE] = (SysCallHandler)sysIPCGetSharedMemSize;
    Handlers[SYS_IPC_MAP_SHMEM] = (SysCallHandler)sysIPCMapSharedMem;
    Handlers[SYS_IPC_UNMAP_SHMEM] = (SysCallHandler)sysIPCUnMapSharedMem;

    cpuWriteMSR(0xC0000081, (uintptr_t)(SEG_KERNEL_DATA) << 48 | (uintptr_t)(SEG_KERNEL_CODE) << 32);
    cpuWriteMSR(0xC0000082, (uintptr_t)syscallHandler);
    cpuWriteMSR(0xC0000084, 0x00000200);
}
