#pragma once

#include <elf.hpp>
#include <ipc.hpp>
#include <list.hpp>
#include <messagequeue.hpp>
#include <mutex.hpp>
#include <sequencer.hpp>
#include <types.h>
#include <vector.hpp>

class DEntry;
class ELF;
class File;
class FrameBuffer;
class NamedMutex;
class NamedObject;
class Semaphore;
class Thread;

#define MAX_HANDLES 1024

typedef int locale_t;

struct pthread {
	/* Part 1 -- these fields may be external or
	 * internal (accessed via asm) ABI. Do not change. */
	struct pthread *self;
	uintptr_t *dtv;
	struct pthread *prev, *next; /* non-ABI */
	uintptr_t sysinfo;
	uintptr_t canary, canary2;

	/* Part 2 -- implementation details, non-ABI. */
	int tid;
	int errno_val;
	volatile int detach_state;
	volatile int cancel;
	volatile unsigned char canceldisable, cancelasync;
	unsigned char tsd_used:1;
	unsigned char dlerror_flag:1;
	unsigned char *map_base;
	size_t map_size;
	void *stack;
	size_t stack_size;
	size_t guard_size;
	void *result;
	struct __ptcb *cancelbuf;
	void **tsd;
	struct {
		volatile void *volatile head;
		long off;
		volatile void *volatile pending;
	} robust_list;
	volatile int timer_id;
	locale_t locale;
	volatile int killlock[1];
	char *dlerror_buf;
	void *stdio_locks;

	/* Part 3 -- the positions of these fields relative to
	 * the end of the structure is external and internal ABI. */
	uintptr_t canary_at_end;
	uintptr_t *dtv_copy;
};

class Process
{
public:
    struct Handle
    {
        enum class HandleType
        {
            Free = 0,
            Unknown,
            File,
            Thread,
            Process,
            NamedObject,
            Mutex,
            Semaphore,
            FrameBuffer,
            InputDevice
        } Type;
        union
        {
            void *Unknown;
            ::File *File;
            ::Thread *Thread;
            ::Process *Process;
            ::NamedObject *NamedObject;
            ::Mutex *Mutex;
            ::Semaphore *Semaphore;
            ::FrameBuffer *FrameBuffer;
            ::InputDevice *InputDevice;
        };
        Handle();
        Handle(nullptr_t);
        Handle(void *obj);
        Handle(::File *file);
        Handle(::Thread *thread);
        Handle(::Process *process);
        Handle(::NamedObject *namedObject);
        Handle(::FrameBuffer *frameBuffer);
        Handle(::InputDevice *inputDevice);
    };
private:
    static Sequencer<pid_t> id;
    static List<Process *> processList;
    static Mutex listLock;
    static uintptr_t kernelAddressSpace;

    Mutex lock;
    bool noAutoRelocs;
    bool deleteAddressSpace;

    static uintptr_t buildUserStack(uintptr_t stackPtr, const char *cmdLine, int envCount, const char *envVars[], ELF *elf, uintptr_t retAddr, uintptr_t basePointer);
    static int processEntryPoint(const char *cmdline);
    static int userThreadEntryPoint(void *arg);

    int allocHandleSlot(Handle handle);
    void freeHandleSlot(int handle);

    uintptr_t brk(uintptr_t brk, bool allocPages);
public:
    pid_t Id;
    Process *Parent;
    char *Name;
    uintptr_t AddressSpace;
    List<ELF *> Images;
    uid_t UID, EUID;
    gid_t GID, EGID;
    DEntry *CurrentDirectory;
    uintptr_t UserStackPtr;
    Vector<Handle> Handles;
    uintptr_t V86PageZeroPhAddr;
    MessageQueue<ipcMessage> Messages;

    // used for brk() syscall
    Mutex MemoryLock;
    uintptr_t MinBrk;
    uintptr_t MaxBrk;
    uintptr_t CurrentBrk;
    uintptr_t CurrentMMapBrk;
    uintptr_t MappedBrk;

    List<Thread *> Threads;
    bool SelfDestruct;
    Semaphore *Finished;
    bool DeleteFinished;
    char *CommandLine;

    static void Initialize();
    static Process *GetByID(pid_t pid);
    static Process *Create(const char *filename, Semaphore *finished, bool noAutoRelocs, int *retVal);
    static Process *GetCurrent();
    static DEntry *GetCurrentDir();
    static bool Finalize(pid_t pid, int retVal);
    static void Dump();
    static int ForEach(bool (*handler)(Process *proc, void *arg), void *arg);

    Process(const char *name, Thread *mainThread, uintptr_t addressSpace, bool SelfDestruct);
    bool Lock();
    void UnLock();
    bool Start();
    bool AddThread(Thread *thread);
    bool RemoveThread(Thread *thread);
    bool AddELF(ELF *elf);
    ELF *GetELF(const char *name);
    bool RemoveELF(ELF *elf);
    Elf_Sym *FindSymbol(const char *name, ELF *skip, ELF **elf);
    const char *GetSymbolName_nolock(uintptr_t addr, ptrdiff_t *delta);
    const char *GetSymbolName(uintptr_t addr, ptrdiff_t *delta);
    bool ApplyRelocations();
    uintptr_t Brk(uintptr_t brk, bool allocPages);
    uintptr_t SBrk(intptr_t incr, bool allocPages);
    uintptr_t MMapSBrk(intptr_t incr, bool allocPages);

    int Open(const char *filename, int flags);
    int Close(int handle);
    template<class T> int NewHandle(T obj)
    {
        if(!Lock()) return -EBUSY;
        int res = allocHandleSlot(Handle(obj));
        UnLock();
        return res;
    }
    void *GetHandleData(int handle, Handle::HandleType type);

    // thread syscall support routines
    int NewThread(const char *name, void *entry, uintptr_t arg, int *retVal);
    int DeleteThread(int handle);
    Thread *GetThread(int handle);
    int ResumeThread(int handle);
    int SuspendThread(int handle);
    int SleepThread(int handle, int ms);
    int WaitThread(int handle, int timeout);
    int AbortThread(int handle, int retVal);

    // process syscall support routines
    int NewProcess(const char *cmdline);
    int DeleteProcess(int handle);
    Process *GetProcess(int handle);
    int WaitProcess(int handle, int timeout);
    int AbortProcess(int handle, int result);

    // named object support routines
    int CreateNamedObjectHandle(NamedObject *no);
    NamedObject *GetNamedObject(int handle);

    int NewMutex();
    Mutex *GetMutex(int idx);
    int DeleteMutex(int idx);

    int NewSemaphore(int initVal);
    Semaphore *GetSemaphore(int idx);
    int DeleteSemaphore(int idx);

    ~Process();
};
