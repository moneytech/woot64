#include <cpu.hpp>
#include <debug.hpp>
#include <dentry.hpp>
#include <errno.h>
#include <file.hpp>
#include <inode.hpp>
#include <memory.hpp>
#include <misc.hpp>
#include <paging.hpp>
#include <process.hpp>
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

struct timespec
{
    time_t tv_sec;
    long tv_nsec;
};

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
	unsigned int    __pad0;
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
    DEBUG("sys_open(\"%s\", %p)", filename, flags);
    int res = Process::GetCurrent()->Open(filename, flags);
    DEBUG(" returned %d\n", res);
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

long SysCalls::sys_exit(intn retVal)
{
    Thread::Finalize(nullptr, retVal);
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

long SysCalls::sys_exit_group(intn retVal)
{
    Process::Finalize(0, retVal);
    return ESUCCESS;
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
    Handlers[SYS_exit] = (SysCallHandler)sys_exit;
    Handlers[SYS_arch_prctl] = (SysCallHandler)sys_arch_prctl;
    Handlers[SYS_set_tid_address] = (SysCallHandler)sys_set_tid_address;
    Handlers[SYS_exit_group] = (SysCallHandler)sys_exit_group;

    cpuWriteMSR(0xC0000081, (uintptr_t)(SEG_KERNEL_DATA) << 48 | (uintptr_t)(SEG_KERNEL_CODE) << 32);
    cpuWriteMSR(0xC0000082, (uintptr_t)syscallHandler);
    cpuWriteMSR(0xC0000084, 0);
}
