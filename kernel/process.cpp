#include <cpu.hpp>
#include <debug.hpp>
#include <errno.h>
#include <file.hpp>
#include <filesystem.hpp>
#include <framebuffer.hpp>
#include <memory.hpp>
#include <misc.hpp>
#include <mutex.hpp>
#include <namedobject.hpp>
#include <process.hpp>
#include <paging.hpp>
#include <semaphore.hpp>
#include <string.hpp>
#include <stringbuilder.hpp>
#include <syscalls.hpp>
#include <sysdefs.h>
#include <thread.hpp>
#include <tokenizer.hpp>


extern "C" void userThreadReturn(int retVal);

#define MAKE_STR(s) #s
#define STRINGIFY(s) MAKE_STR(s)

struct ForkEntryArgs
{
    Process *CallerProcess;
    ForkRegisters Registers;
};

#ifdef __i386__
asm(
INLINE_ASM_SYNTAX
".section .text.user\n"
".globl userThreadReturn\n"
"userThreadReturn:\n"
"push eax\n"
"push 0xFFFFFFFF\n"
"push " STRINGIFY(SYS_THREAD_ABORT) "\n"
"call __doSyscall\n"
".section .text\n"
NORMAL_ASM_SYNTAX
);
#endif // __i386__
#if defined(__x86_64__) || defined(__amd64__)
asm(
INLINE_ASM_SYNTAX
".section .text.user\n"
".globl userThreadReturn\n"
"userThreadReturn:\n"
"mov rsi, rax\n"
"mov rdi, -1\n"
"mov rax, " STRINGIFY(SYS_THREAD_ABORT) "\n"
"syscall\n"
".section .text\n"
NORMAL_ASM_SYNTAX
);
#endif // defined(__x86_64__) || defined(__amd64__)


Sequencer<pid_t> Process::id(1);
List<Process *> Process::processList;
Mutex Process::listLock(false, "procList");
uintptr_t Process::kernelAddressSpace;

typedef struct AuxVector
{
    uintptr_t a_type;
    uintptr_t a_val;
} AuxVector;

#define AT_NULL         0               /* End of vector */
#define AT_IGNORE       1               /* Entry should be ignored */
#define AT_EXECFD       2               /* File descriptor of program */
#define AT_PHDR         3               /* Program headers for program */
#define AT_PHENT        4               /* Size of program header entry */
#define AT_PHNUM        5               /* Number of program headers */
#define AT_PAGESZ       6               /* System page size */
#define AT_BASE         7               /* Base address of interpreter */
#define AT_FLAGS        8               /* Flags */
#define AT_ENTRY        9               /* Entry point of program */
#define AT_NOTELF       10              /* Program is not ELF */
#define AT_UID          11              /* Real uid */
#define AT_EUID         12              /* Effective uid */
#define AT_GID          13              /* Real gid */
#define AT_EGID         14              /* Effective gid */
#define AT_CLKTCK       17              /* Frequency of times() */
#define AT_SECURE       23
#define AT_SYSINFO      32

uintptr_t Process::buildUserStack(uintptr_t stackPtr, const char *cmdLine, int envCount, const char *envVars[], ELF *elf)
{
    auto stackPush = [](uintptr_t stackPtr, void *data, size_t size) -> uintptr_t
    {
        stackPtr -= size;
        void *buf = reinterpret_cast<void *>(stackPtr);
        Memory::Move(buf, data, size);
        return stackPtr;
    };

    Tokenizer cmd(cmdLine, " ", 0);
    long argCount = cmd.Tokens.Count();

    uintptr_t envPtrs[32];
    uintptr_t argPtrs[32];

    uintptr_t zeroPtr = 0;
    stackPtr = stackPush(stackPtr, &zeroPtr, sizeof(zeroPtr));

    // info block
    int i;
    for(i = 0; i < envCount; ++i)
    {
        envPtrs[i] = stackPtr =
                stackPush(stackPtr,
                          reinterpret_cast<void *>(const_cast<char *>(envVars[i])),
                          String::Length(envVars[i]) + 1);
    }
    i = 0;
    for(Tokenizer::Token token : cmd.Tokens)
    {
        char *str = token.String;
        argPtrs[i++] = stackPtr = stackPush(stackPtr, str, String::Length(str) + 1);
    }

    // align stack pointer
    uintptr_t padding[4];
    Memory::Zero(&padding, sizeof(padding));
    stackPtr = stackPush(stackPtr, &padding, stackPtr % sizeof(padding));

    AuxVector auxVectors[] =
    {
        { AT_SECURE, 0 },
        { AT_UID, 0 },
        { AT_GID, 0 },
        { AT_EUID, 0 },
        { AT_EGID, 0 },
        { AT_ENTRY, reinterpret_cast<uintptr_t>(elf->EntryPoint) },
        { AT_BASE, elf->GetBase() },
        { AT_PAGESZ, PAGE_SIZE },
        { AT_PHNUM, static_cast<uintptr_t>(elf->ehdr->e_phnum) },
        { AT_PHENT, static_cast<uintptr_t>(elf->ehdr->e_phentsize) },
        { AT_PHDR, reinterpret_cast<uintptr_t>(elf->phdrData) }
    };

    // aux vectors
    for(i = 0; i < 2; ++i)
        stackPtr = stackPush(stackPtr, &zeroPtr, sizeof zeroPtr);
    stackPtr = stackPush(stackPtr, auxVectors, sizeof auxVectors);

    // env pointers
    stackPtr = stackPush(stackPtr, &zeroPtr, sizeof zeroPtr);
    for(i = 0; i < envCount; ++i)
        stackPtr = stackPush(stackPtr, &envPtrs[envCount - i - 1], sizeof(uintptr_t));

    // arg pointers
    stackPtr = stackPush(stackPtr, &zeroPtr, sizeof zeroPtr);
    for(i = 0; i < argCount; ++i)
        stackPtr = stackPush(stackPtr, &argPtrs[argCount - i - 1], sizeof(uintptr_t));
    stackPtr = stackPush(stackPtr, &argCount, sizeof argCount);
    return stackPtr;
}

int Process::processEntryPoint(const char *cmdline)
{
    Process *proc = GetCurrent();
    Tokenizer cmd(cmdline, " ", 2);
    ELF *elf = ELF::Load(cmd[0], true, false, !proc->noAutoRelocs, PG_INVALID_ADDRESS, false);
    if(!elf) return 127;
    if(!elf->EntryPoint) return 126;
    if(!proc->lock.Acquire(0, false)) return 126;

    proc->MemoryLock.Acquire(0, false);
    for(ELF *elf : proc->Images)
        proc->MinBrk = max(proc->MinBrk, align(elf->GetEndPtr(), (64 << 10)));
    proc->CurrentBrk = proc->MinBrk;
    proc->MappedBrk = proc->CurrentBrk;
    proc->MaxBrk = USER_MAX_BRK;
    proc->MemoryLock.Release();

    Thread *ct = Thread::GetCurrent();

    // allocate and initialize user stack
    uintptr_t stackPointer = ct->AllocStack(&ct->UserStack, ct->UserStackSize);
    const char *envVars[] =
    {
        "LC_ALL=C",
        "PATH=WOOT_OS~/bin",
        "LD_LIBRARY_PATH=WOOT_OS~/lib",
        "TEST=value"
    };
    stackPointer = buildUserStack(stackPointer, cmdline, sizeof(envVars) / sizeof(const char *), envVars, elf);

    ct->FS = proc->SBrk(PAGE_SIZE, true); //  FIXME: may leak
    cpuWriteMSR(0xC0000100, ct->FS);
    cpuWriteMSR(0xC0000101, ct->GS);

    proc->lock.Release();
    ct->Initialized->Signal(nullptr);
    cpuEnterUserMode(ct->UserArgument, stackPointer, reinterpret_cast<uintptr_t>(elf->EntryPoint), 0);
    return 0;
}

int Process::userThreadEntryPoint(void *arg)
{
    (void)arg;
    Thread *ct = Thread::GetCurrent();

    // allocate and initialize user stack
    uintptr_t *stack = reinterpret_cast<uintptr_t *>(ct->AllocStack(&ct->UserStack, ct->UserStackSize));
    *(--stack) = reinterpret_cast<uintptr_t>(userThreadReturn);

    ct->Initialized->Signal(nullptr);
    cpuEnterUserMode(ct->UserArgument,
                     reinterpret_cast<uintptr_t>(stack),
                     reinterpret_cast<uintptr_t>(ct->UserEntryPoint),
                     0);
    return 0;
}

int Process::forkThreadEntryPoint(ForkEntryArgs *forkArgs)
{
    Process *cp = Process::GetCurrent();
    ForkEntryArgs args = *forkArgs;
    delete forkArgs;

    Thread *ct = Thread::GetCurrent();
    ct->Initialized->Signal(nullptr);
    cpuEnterUserModeFork(&args.Registers);
    return 0;
}

int Process::allocHandleSlot(Handle handle)
{
    size_t handlesSize = Handles.Size();
    for(int i = 0; i < handlesSize; ++i)
    {
        if(Handles.Get(i).Type == Handle::HandleType::Free ||
                Handles.Get(i).Type == Handle::HandleType::Invalid)
        {
            Handles.Set(i, handle);
            return i;
        }
    }
    if(!Handles.Append(handle))
        return -ENOMEM;
    return handlesSize;
}

void Process::freeHandleSlot(int handle)
{
    size_t handlesSize = Handles.Size();
    if(handle < 0 || handle >= handlesSize)
        return;
    Handles.Set(handle, Handle(Handle::HandleType::Free));
}

uintptr_t Process::brk(uintptr_t brk, bool allocPages)
{
    brk = align(brk, PAGE_SIZE);

    if(brk < MinBrk || brk > MaxBrk)
        return (brk = CurrentBrk);

    uintptr_t mappedNeeded = align(brk, PAGE_SIZE);

    if(mappedNeeded > MappedBrk)
    {   // alloc and map needed memory
        if(allocPages)
        {
            for(uintptr_t va = MappedBrk; va < mappedNeeded; va += PAGE_SIZE)
            {
                uintptr_t pa = Paging::AllocFrame();
                if(pa == PG_INVALID_ADDRESS)
                    return CurrentBrk;
                if(!Paging::MapPage(AddressSpace, va, pa, true, true, false))
                    return CurrentBrk;
            }
        }
        MappedBrk = mappedNeeded;
    }
    else
    {   // unmap and free excess memory
        for(uintptr_t va = mappedNeeded; va < MappedBrk; va += PAGE_SIZE)
        {
            uintptr_t pa = Paging::GetPhysicalAddress(AddressSpace, va);
            if(pa != PG_INVALID_ADDRESS) Paging::FreeFrame(pa);
            Paging::UnMapPage(AddressSpace, va);
        }
        MappedBrk = mappedNeeded;
    }
    //DEBUG("[process] %d brk %p -> %p %d\n", ID, obrk, brk, allocPages);
    CurrentBrk = brk;
    return brk;
}

void Process::Initialize()
{
    kernelAddressSpace = Paging::GetCurrentAddressSpace();
    Thread *ct = Thread::GetCurrent();
    Process *kernelProc = new Process("Main kernel process", ct, kernelAddressSpace, false);
    kernelProc->Threads.Append(Thread::GetIdleThread());
}

Process *Process::GetByID(pid_t pid)
{
    if(!pid) return Process::GetCurrent();
    for(Process *proc : processList)
    {
        if(pid == proc->Id)
            return proc;
    }
    return nullptr;
}

Process *Process::Create(const char *filename, Semaphore *finished, bool noAutoRelocs, int *retVal)
{
    if(!filename) return nullptr;
    bool deleteFinished = false;
    if(!finished)
    {
        finished = new Semaphore(0);
        deleteFinished = true;
    }
    char *cmdLine = String::Duplicate(filename);

    Thread *thread = new Thread("main", nullptr, reinterpret_cast<void *>(processEntryPoint),
                                reinterpret_cast<uintptr_t>(cmdLine), DEFAULT_STACK_SIZE,
                                DEFAULT_USER_STACK_SIZE, retVal, finished);

    Process *proc = new Process(filename, thread, 0, deleteFinished);
    proc->noAutoRelocs = noAutoRelocs;
    proc->Finished = finished;
    proc->DeleteFinished = deleteFinished;
    proc->CommandLine = cmdLine;
    return proc;
}

Process *Process::Create(ForkRegisters *regs)
{
    Process *cp = Process::GetCurrent();
    Thread *ct = Thread::GetCurrent();
    ForkEntryArgs *args = new ForkEntryArgs { cp, *regs };
    Thread *thread = new Thread("forked", nullptr, reinterpret_cast<void *>(forkThreadEntryPoint),
                                reinterpret_cast<uintptr_t>(args), DEFAULT_STACK_SIZE,
                                DEFAULT_USER_STACK_SIZE, nullptr, nullptr);
    thread->FS = ct->FS;
    thread->GS = ct->GS;
    Process *proc =  new Process(cp->Name, thread, 0, true);
    Paging::CloneRange(proc->AddressSpace, args->CallerProcess->AddressSpace, 0, USER_END);
    return proc;
}

Process *Process::GetCurrent()
{
    Thread *ct = Thread::GetCurrent();
    if(!ct) return nullptr;
    return ct->Process;
}

DEntry *Process::GetCurrentDir()
{
    bool ints = cpuDisableInterrupts();
    Process *cp = GetCurrent();
    if(!cp)
    {
        cpuRestoreInterrupts(ints);
        return nullptr;
    }
    DEntry *de = cp->CurrentDirectory;
    cpuRestoreInterrupts(ints);
    return de;
}

void Process::SetCurrentDir(DEntry *dentry)
{
    bool ints = cpuDisableInterrupts();
    Process *cp = GetCurrent();
    if(!cp)
    {
        cpuRestoreInterrupts(ints);
        return;
    }
    cp->CurrentDirectory = dentry;
    cpuRestoreInterrupts(ints);
    return;
}

bool Process::Finalize(pid_t pid, int retVal)
{
    if(!listLock.Acquire(0, false))
        return false;
    bool res = false;
    Process *proc = GetByID(pid);
    Thread *currentThread = Thread::GetCurrent();
    bool finalizeCurrentThread = false;
    if(proc)
    {
        res = true;
        for(Thread *thread : proc->Threads)
        {
            if(thread != currentThread)
                Thread::Finalize(thread, -127);
            else finalizeCurrentThread = true;
        }
    }
    listLock.Release();
    if(finalizeCurrentThread && currentThread)
        Thread::Finalize(currentThread, -127);
    return res;
}

void Process::Dump()
{
    DEBUG("Process dump:\n");
    Process *cp = Process::GetCurrent();
    DEBUG("Current process: %s (%d)\n", cp ? cp->Name : "no current process", cp ? cp->Id : -1);
    for(Process *p : processList)
    {
        DEBUG("Process: %s (%d)\n", p->Name, p->Id);
        for(Thread *t : p->Threads)
            DEBUG(" %s(%d; %p)\n"
                  "   st %s\n"
                  "   mtx %p(%s; %d)\n"
                  "   sem %p(%s; %d)\n",
                  t->Name, t->Id, t,
                  Thread::StateNames[(int)t->State],
                  t->WaitingMutex,
                  t->WaitingMutex ? t->WaitingMutex->Name : "none",
                  t->WaitingMutex ? t->WaitingMutex->GetCount() : -1,
                  t->WaitingSemaphore,
                  t->WaitingSemaphore ? t->WaitingSemaphore->Name : "none",
                  t->WaitingSemaphore ? t->WaitingSemaphore->GetCount() : -1);
    }
}

int Process::ForEach(bool (*handler)(Process *proc, void *arg), void *arg)
{
    if(!listLock.Acquire(0, false))
        return -EBUSY;
    for(Process *proc : processList)
    {
        if(!handler(proc, arg))
            break;
    }
    listLock.Release();
    return ESUCCESS;
}

size_t Process::GetCount()
{
    if(!listLock.Acquire(0, false))
        return (size_t)(-EBUSY);
    size_t res = processList.Count();
    listLock.Release();
    return res;
}

int Process::ListIds(pid_t *buf, size_t bufSize)
{
    if(!buf || !bufSize)
        return -EINVAL;
    if(!listLock.Acquire(0, false))
        return -EBUSY;
    int res = 0;
    for(Process *proc : processList)
    {
        if(res >= bufSize)
            break;
        buf[res++] = proc->Id;
    }
    listLock.Release();
    return res;
}

int Process::GetName(pid_t pid, char *buf, size_t bufSize)
{
    if(!buf || !bufSize)
        return -EINVAL;
    if(!listLock.Acquire(0, false))
        return -EBUSY;
    Process *proc = GetByID(pid);
    int res = ESUCCESS;
    if(!proc) res = -ESRCH;
    else String::Copy(buf, proc->Name, bufSize);
    listLock.Release();
    return res;
}

Process::Process(const char *name, Thread *mainThread, uintptr_t addressSpace, bool selfDestruct) :
    lock(true, "processLock"),
    UserStackPtr(USER_END - PAGE_SIZE),
    Handles(8, 8, MAX_HANDLES),
    Id(id.GetNext()),
    Messages(64),
    Parent(Process::GetCurrent()),
    Name(String::Duplicate(name)),
    AddressSpace(addressSpace),
    CurrentMMapBrk(0x80000000),
    MemoryLock(true, "processMemoryLock"),
    SelfDestruct(selfDestruct)
{
    if(!AddressSpace)
    {
        deleteAddressSpace = true;
        AddressSpace = Paging::AllocFrame();
        Paging::BuildAddressSpace(AddressSpace);
    }

    // stdin, stdout and stderr default to debug stream for now
    Handles.Append(Handle(&Debug::DebugStream));
    Handles.Append(Handle(&Debug::DebugStream));
    Handles.Append(Handle(&Debug::DebugStream));

    // clone stdin, stdout and stderr if possible
    if(Parent)
    {
        Handle h[3] =
        {
            Parent->Handles.Get(0),
            Parent->Handles.Get(1),
            Parent->Handles.Get(2)
        };

        for(int i = 0; i < 3; ++i)
        {
            if(h[i].Type != Handle::HandleType::Free && h[i].Type != Handle::HandleType::Invalid)
                Handles.Set(i, h[i]);
        }
    }

    DEntry *cdir = Parent ? Parent->GetCurrentDir() : nullptr;
    if(cdir) CurrentDirectory = FileSystem::GetDEntry(cdir);
    AddThread(mainThread);
    listLock.Acquire(0, false);
    processList.Append(this);
    listLock.Release();
}

bool Process::Lock()
{
    if(lock.Acquire(5000, false))
        return true;
    errno = -EBUSY;
    return false;
}

void Process::UnLock()
{
    lock.Release();
}

bool Process::Start()
{
    if(!Lock()) return false;
    Thread *t = Threads[0];
    if(!t)
    {
        UnLock();
        return false;
    }
    t->Enable();
    bool res = t->Resume(false);
    UnLock();
    return res;
}

bool Process::AddThread(Thread *thread)
{
    if(!thread) return false;
    if(!Lock()) return false;
    Threads.Append(thread);
    thread->Process = this;
    UnLock();
    return true;
}

bool Process::RemoveThread(Thread *thread)
{
    if(!Lock()) return false;
    bool res = Threads.Remove(thread, nullptr, false) != 0;
    thread->Process = nullptr;
    UnLock();
    return res;
}

bool Process::AddELF(ELF *elf)
{
    if(!elf || !Lock())
        return false;
    Images.Append(elf);
    UnLock();
    return true;
}

ELF *Process::GetELF(const char *name)
{
    if(!Lock()) return nullptr;
    ELF *res = nullptr;
    for(ELF *elf : Images)
    {
        if(!String::Compare(name, elf->Name))
        {
            res = elf;
            break;
        }
    }
    UnLock();
    return res;
    return nullptr;
}

bool Process::RemoveELF(ELF *elf)
{
    if(!elf || !Lock())
        return false;
    bool res = Images.Remove(elf, nullptr, false);
    UnLock();
    return res;
}

Elf_Sym *Process::FindSymbol(const char *name, ELF *skip, ELF **elf)
{
    if(!Lock()) return nullptr;
    for(ELF *e : Images)
    {
        if(e == skip)
            continue;
        Elf_Sym *sym = e->FindSymbol(name);
        if(sym)
        {
            if(elf) *elf = e;
            UnLock();
            return sym;
        }
    }
    UnLock();
    return nullptr;
}

const char *Process::GetSymbolName_nolock(uintptr_t addr, ptrdiff_t *delta)
{
    for(ELF *elf : Images)
    {
        const char *name = elf->GetSymbolName(addr, delta);
        if(name) return name;
    }
    return nullptr;
}

const char *Process::GetSymbolName(uintptr_t addr, ptrdiff_t *delta)
{
    if(!Lock()) return nullptr;
    const char *res = GetSymbolName_nolock(addr, delta);
    UnLock();
    return res;
}

bool Process::ApplyRelocations()
{
    if(!Lock()) return false;
    for(ELF *e : Images)
    {
        if(!e->ApplyRelocations())
        {
            UnLock();
            return false;
        }
    }
    UnLock();
    return true;
}

uintptr_t Process::Brk(uintptr_t brk, bool allocPages)
{
    if(!MemoryLock.Acquire(5000, false))
        return ~0;
    uintptr_t res = this->brk(brk, allocPages);
    MemoryLock.Release();
    return res;
}

uintptr_t Process::SBrk(intptr_t incr, bool allocPages)
{
    if(!MemoryLock.Acquire(5000, false))
        return ~0;
    uintptr_t obrk = CurrentBrk;
    uintptr_t res = this->brk(CurrentBrk + incr, allocPages);
    MemoryLock.Release();
    return obrk;
}

uintptr_t Process::MMapSBrk(intptr_t incr, bool allocPages)
{
    if(!MemoryLock.Acquire(5000, false))
        return ~0;
    uintptr_t oEBrk = CurrentMMapBrk;
    CurrentMMapBrk += incr;
    CurrentMMapBrk = align(CurrentMMapBrk, PAGE_SIZE);
    for(uintptr_t va = oEBrk; allocPages && va < CurrentMMapBrk; va += PAGE_SIZE)
    {
        if(va < KERNEL_BASE)
        {
            uintptr_t pa = Paging::AllocFrame();
            Paging::MapPage(AddressSpace, va, pa, true, true, false);
        }
    }
    MemoryLock.Release();
    return oEBrk;
}

char *Process::GetExecName()
{
    ELF *execELF = Images.Get(0);
    if(!execELF) return nullptr;
    return execELF->FullPath;
}

long Process::ExecVE(const char *filename, const char * const argv[], const char * const envp[])
{
    if(Process::GetCurrent() != this)
        return -ENOEXEC; // this function must be called by this process itself

    // we need local copies of input arguments since we will be unmapping
    // the whole userspace of the calling process
    char *_filename = String::Duplicate(filename);
    size_t nargv = 0;
    size_t nenvp = 0;
    for(;argv[nargv] ; ++nargv);
    for(;envp[nenvp] ; ++nenvp);
    char **_argv = new char *[nargv];
    char **_envp = new char *[nenvp];
    for(size_t i = 0; i < nargv; ++i)
        _argv[i] = String::Duplicate(argv[i]);
    for(size_t i = 0; i < nenvp; ++i)
        _envp[i] = String::Duplicate(envp[i]);

    // Unload all images
    for(ELF *elf : Images)
        delete elf;

    // unmap everything
    Paging::UnmapRange(AddressSpace, USER_BASE, USER_END - USER_BASE);

    // finalize all possibly existing threads, except current thread
    Thread *ct = Thread::GetCurrent();
    for(Thread *t : Threads)
    {
        if(t == ct) continue;
        Thread::Finalize(t, 0);
    }

    // load new image
    ELF *elf = ELF::Load(_filename, true, false, true, PG_INVALID_ADDRESS, false);
    if(!elf)
    {
        // delete local copies of arguments
        for(size_t i = 0; i < nargv; ++i)
            delete[] _argv[i];
        for(size_t i = 0; i < nenvp; ++i)
            delete[] _envp[i];
        delete[] _filename;
        delete[] _argv;
        delete[] _envp;
        return -ENOENT;
    }

    // initialize heap
    MemoryLock.Acquire(0, false);
    for(ELF *elf : Images)
        MinBrk = max(MinBrk, align(elf->GetEndPtr(), (64 << 10)));
    CurrentBrk = MinBrk;
    MappedBrk = CurrentBrk;
    MaxBrk = USER_MAX_BRK;
    MemoryLock.Release();

    // create new userspace stack
    auto stackPush = [](uintptr_t stackPtr, void *data, size_t size) -> uintptr_t
    {
        stackPtr -= size;
        void *buf = reinterpret_cast<void *>(stackPtr);
        Memory::Move(buf, data, size);
        return stackPtr;
    };
    uintptr_t stackPtr = ct->AllocStack(&ct->UserStack, ct->UserStackSize);
    uintptr_t zero = 0;
    stackPtr = stackPush(stackPtr, &zero, sizeof(zero));
    Vector<uintptr_t> envPtrs(128, 128);
    for(size_t i = 0; i < nenvp; ++i)
    {
        stackPtr = stackPush(stackPtr, _envp[i], String::Length(_envp[i]) + 1);
        envPtrs.Append(stackPtr);
    }
    Vector<uintptr_t> argPtrs(128, 128);
    for(size_t i = 0; i < nargv; ++i)
    {
        stackPtr = stackPush(stackPtr, _argv[i], String::Length(_argv[i]) + 1);
        argPtrs.Append(stackPtr);
    }
    AuxVector auxVectors[] =
    {
        { AT_SECURE, 0 },
        { AT_UID, 0 },
        { AT_GID, 0 },
        { AT_EUID, 0 },
        { AT_EGID, 0 },
        { AT_ENTRY, reinterpret_cast<uintptr_t>(elf->EntryPoint) },
        { AT_BASE, elf->GetBase() },
        { AT_PAGESZ, PAGE_SIZE },
        { AT_PHNUM, static_cast<uintptr_t>(elf->ehdr->e_phnum) },
        { AT_PHENT, static_cast<uintptr_t>(elf->ehdr->e_phentsize) },
        { AT_PHDR, reinterpret_cast<uintptr_t>(elf->phdrData) }
    };
    // aux vectors
    for(size_t i = 0; i < 2; ++i)
        stackPtr = stackPush(stackPtr, &zero, sizeof zero);
    stackPtr = stackPush(stackPtr, auxVectors, sizeof auxVectors);
    // env pointers
    stackPtr = stackPush(stackPtr, &zero, sizeof zero);
    for(size_t i = 0; i < nenvp; ++i)
    {
        uintptr_t envPtr = envPtrs.Get(static_cast<uint>(nenvp - i - 1));
        stackPtr = stackPush(stackPtr, &envPtr, sizeof envPtr);
    }
    // arg pointers
    stackPtr = stackPush(stackPtr, &zero, sizeof zero);
    for(size_t i = 0; i < nargv; ++i)
    {
        uintptr_t argPtr = argPtrs.Get(static_cast<uint>(nargv - i - 1));
        stackPtr = stackPush(stackPtr, &argPtr, sizeof argPtr);
    }
    ct->StackPointer = stackPtr = stackPush(stackPtr, &nargv, sizeof nargv);

    // delete local copies of arguments
    for(size_t i = 0; i < nargv; ++i)
        delete[] _argv[i];
    for(size_t i = 0; i < nenvp; ++i)
        delete[] _envp[i];
    delete[] _filename;
    delete[] _argv;
    delete[] _envp;

    cpuEnterUserMode(ct->UserArgument, ct->StackPointer, reinterpret_cast<uintptr_t>(elf->EntryPoint), 0);
    return ESUCCESS;
}

int Process::Open(const char *filename, int flags, mode_t mode)
{
    if(!filename) return -EINVAL;
    if(!Lock()) return -EBUSY;
    File *f = File::Open(filename, flags, mode, true);
    if(!f)
    {
        UnLock();
        return -ENOENT;
    }
    int res = allocHandleSlot(Handle(f));
    if(res < 0) delete f;
    UnLock();
    return res;
}

int Process::Close(int handle)
{
    if(!Lock()) return -EBUSY;
    Handle h = Handles.Get(handle);
    freeHandleSlot(handle);
    if(h.Type == Handle::HandleType::File)
    {
        if(h.File) delete h.File;
        UnLock();
        return ESUCCESS;
    }
    else if(h.Type == Handle::HandleType::NamedObject)
    {
        int res = h.NamedObject->Put();
        UnLock();
        return res;
    }
    else if(h.Type == Handle::HandleType::FrameBuffer)
    {
        int res = h.FrameBuffer->Close();
        UnLock();
        return res;
    }
    else if(h.Type == Handle::HandleType::InputDevice)
    {
        int res = h.InputDevice->Close();
        UnLock();
        return res;
    }
    else if(h.Type == Handle::HandleType::Pipe)
    {
        if(h.Pipe->FDs[0] == handle)
            h.Pipe->FDs[0] = -EPIPE;
        if(h.Pipe->FDs[1] == handle)
            h.Pipe->FDs[1] = -EPIPE;

        if(h.Pipe->FDs[0] < 0 && h.Pipe->FDs[1] < 0)
            delete h.Pipe;
        UnLock();
        return ESUCCESS;
    }
    UnLock();
    return -EBADF;
}

long Process::Read(int handle, void *buffer, size_t size)
{
    if(!Lock()) return -EBUSY;
    Handle h = Handles.Get(handle);
    long res = -EBADF;
    switch(h.Type)
    {
    case Handle::HandleType::File:
    {
        auto file = h.File;
        UnLock();
        return file->Read(buffer, size);
    }
    case Handle::HandleType::Stream:
    {
        auto stream = h.Stream;
        UnLock();
        return stream->Read(buffer, size);
    }
    case Handle::HandleType::Pipe:
    {
        auto data = h.Pipe->Data;
        UnLock();
        for(res = 0; res < size; ++res)
        {
            if(data->Read(res + (uint8_t *)buffer, res <= 0 ? -1 : 0) < 0)
                break;
        }
        return res;
    }
    default:
        break;
    }
    UnLock();
    return res;
}

long Process::Write(int handle, const void *buffer, size_t size)
{
    if(!Lock()) return -EBUSY;
    Handle h = Handles.Get(handle);
    long res = -EBADF;
    switch(h.Type)
    {
    case Handle::HandleType::File:
    {
        auto file = h.File;
        UnLock();
        return file->Write(buffer, size);
    }
    case Handle::HandleType::Stream:
    {
        auto stream = h.Stream;
        UnLock();
        return stream->Write(buffer, size);
    }
    case Handle::HandleType::Pipe:
    {
        auto data = h.Pipe->Data;
        UnLock();
        for(res = 0; res < size; ++res)
            data->Write(*(res + (uint8_t *)buffer), -1);
        return res;
    }
    default:
        break;
    }
    UnLock();
    return res;
}

void *Process::GetHandleData(int handle, Process::Handle::HandleType type)
{
    if(!Lock()) return nullptr;
    Handle h = Handles.Get(handle);
    if(h.Type != type)
    {
        errno = EBADF;
        UnLock();
        return nullptr;
    }
    UnLock();
    return h.Unknown;
}

int Process::DuplicateFileDescriptor(int fd)
{
    if(!Lock()) return -EBUSY;
    Handle h = Handles.Get(fd);
    if(h.Type == Handle::HandleType::Free)
    {
        UnLock();
        return -EBADF;
    }
    int newFd = allocHandleSlot(h);
    UnLock();
    return newFd;
}

int Process::DuplicateFileDescriptor(int oldfd, int newfd)
{
    if(!Lock()) return -EBUSY;
    Handle h = Handles.Get(oldfd);
    if(h.Type == Handle::HandleType::Free)
    {
        UnLock();
        return -EBADF;
    }
    Handle h2 = Handles.Get(newfd);
    if(h2.Type == Handle::HandleType::Invalid)
    {
        newfd = allocHandleSlot(h);
        UnLock();
        return newfd;
    }
    else if(h2.Type != Handle::HandleType::Free)
        Close(newfd);
    Handles.Set(newfd, h);
    UnLock();
    return newfd;
}

int Process::CreatePipe(int fds[2])
{
    if(!Lock()) return -EBUSY;

    fds[0] = allocHandleSlot(Handle((Pipe *)nullptr));
    if(fds[0] < 0)
    {
        UnLock();
        return fds[0];
    }
    fds[1] = allocHandleSlot(Handle((Pipe *)nullptr));
    if(fds[1] < 0)
    {
        freeHandleSlot(fds[0]);
        UnLock();
        return fds[1];
    }

    Pipe *pipe = new Pipe(16 << 10, fds);
    Handles.Set(fds[0], Handle(pipe));
    Handles.Set(fds[1], Handle(pipe));

    UnLock();
    return ESUCCESS;
}

int Process::NewThread(const char *name, void *entry, uintptr_t arg, int *retVal)
{
    if(!Lock()) return -EBUSY;
    Thread *t = new Thread(name, this, reinterpret_cast<void *>(userThreadEntryPoint), 0, DEFAULT_STACK_SIZE, DEFAULT_USER_STACK_SIZE, retVal, nullptr);
    t->UserEntryPoint = entry;
    t->UserArgument = arg;

    // allocate and initialize new TLS area for new thread    
    Thread *ct = Thread::GetCurrent();
    if(ct->FS)
    {
        t->FS = SBrk(PAGE_SIZE, true);
        Memory::Move(reinterpret_cast<void *>(t->FS),
                     reinterpret_cast<void *>(ct->FS),
                     PAGE_SIZE);
    }

    pid_t res = t->Id;
    t->Enable();
    UnLock();
    return res;
}

int Process::CreateNamedObjectHandle(NamedObject *no)
{
    if(!Lock()) return -EBUSY;
    int res = allocHandleSlot(Handle(no));
    if(res < 0)
        return res;
    UnLock();
    return res;
}

NamedObject *Process::GetNamedObject(int handle)
{
    if(!Lock()) return nullptr;
    NamedObject *no = static_cast<NamedObject *>(GetHandleData(handle, Handle::HandleType::NamedObject));
    UnLock();
    return no;
}

int Process::NewMutex(bool recursive)
{
    Mutex *mutex = new Mutex(recursive, "userspace mutex");
    if(!Lock())
    {
        delete mutex;
        return -EBUSY;
    }
    int res = allocHandleSlot(Handle(mutex));
    if(res < 0) delete mutex;
    UnLock();
    return res;
}

Mutex *Process::GetMutex(int fd)
{
    if(!Lock()) return nullptr;
    Mutex *mtx = static_cast<Mutex *>(GetHandleData(fd, Handle::HandleType::Mutex));
    UnLock();
    return mtx;
}

int Process::NewSemaphore(int initVal)
{
    Semaphore *sem = new Semaphore(initVal, "userspace semaphore");
    if(!Lock())
    {
        delete sem;
        return -EBUSY;
    }
    int res = allocHandleSlot(Handle(sem));
    if(res < 0) delete sem;
    UnLock();
    return res;
}

Semaphore *Process::GetSemaphore(int fd)
{
    if(!Lock()) return nullptr;
    Semaphore *sem = static_cast<Semaphore *>(GetHandleData(fd, Handle::HandleType::Semaphore));
    UnLock();
    return sem;
}

Process::~Process()
{
    Lock();

    for(size_t i = 0; i < Handles.Size(); ++i)
        Close(static_cast<int>(i));

    bool lockAcquired = listLock.Acquire(0, true);
    for(ELF *elf : Images)
        if(elf) delete elf;
    Paging::UnmapRange(AddressSpace, 0, USER_END);
    if(deleteAddressSpace) Paging::FreeFrame(AddressSpace);
    if(CurrentDirectory) FileSystem::PutDEntry(CurrentDirectory);
    processList.Remove(this, nullptr, false);
    if(lockAcquired) listLock.Release();
    if(Name) delete[] Name;
    if(DeleteFinished && Finished) delete Finished;
    if(CommandLine) delete[] CommandLine;
}

Process::Handle::Handle() :
    Type(HandleType::Invalid),
    Unknown(nullptr)
{
}

Process::Handle::Handle(Process::Handle::HandleType type) :
    Type(type),
    Unknown(nullptr)
{
}

Process::Handle::Handle(nullptr_t) :
    Type(HandleType::Unknown),
    Unknown(nullptr)
{
}

Process::Handle::Handle(::Stream *stream) :
    Type(HandleType::Stream),
    Stream(stream)
{
}

Process::Handle::Handle(::File *file) :
    Type(HandleType::File),
    File(file)
{
}

Process::Handle::Handle(::NamedObject *namedObject) :
    Type(HandleType::NamedObject),
    NamedObject(namedObject)
{
}

Process::Handle::Handle(::Mutex *mutex) :
    Type(HandleType::Mutex),
    Mutex(mutex)
{
}

Process::Handle::Handle(::Semaphore *semaphore) :
    Type(HandleType::Semaphore),
    Semaphore(semaphore)
{
}

Process::Handle::Handle(::FrameBuffer *frameBuffer) :
    Type(HandleType::FrameBuffer),
    FrameBuffer(frameBuffer)
{
}

Process::Handle::Handle(::InputDevice *inputDevice) :
    Type(HandleType::InputDevice),
    InputDevice(inputDevice)
{
}

Process::Handle::Handle(Process::Pipe *pipe) :
    Type(HandleType::Pipe),
    Pipe(pipe)
{
}

Process::Pipe::Pipe(size_t size, int *fds) :
    Data(new MessageQueue<uint8_t>(size)),
    FDs { fds[0], fds[1] }
{
}

Process::Pipe::~Pipe()
{
    if(Data) delete Data;
    Data = nullptr;
}
