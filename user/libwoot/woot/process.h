#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// KERNEL_MATCH: kernel/syscalls.cpp
#define PROC_MAP_READ   (1 << 0)
#define PROC_MAP_WRITE  (1 << 1)
#define PROC_MAP_EXEC   (1 << 2)
#define PROC_MAP_NAME   (1 << 3)
#define PROC_MAP_STACK  (1 << 4)
#define PROC_MAP_HEAP   (1 << 5)
#define PROC_MAP_LAST   (1 << 30)

// KERNEL_MATCH: kernel/syscalls.cpp
typedef struct processMapEntry
{
    unsigned long EntrySize;    // Size of this entry
    unsigned long Address;      // Start of the mapping
    unsigned long Size;         // Size of the mapping
    unsigned long Offset;       // File offset of the mapping
    unsigned long Flags;        // Flags
    char Name[];                // Optional NULL terminated filename
} processMapEntry_t;

int processCreate(const char *cmdline);
int processDelete(int handle);
int processWait(int handle, int timeout);
int processAbort(int handle, int result);
int processListIds(int *buf, unsigned bufSize);
int processGetName(int pid, char *buf, unsigned bufSize);
int processGetThreadCount(int pid);
unsigned long processGetUsedMemory(int pid);
int processGetExecPath(int pid, char *buf, unsigned bufSize);
int processGetMap(int pid, processMapEntry_t *buf, unsigned bufSize);

#ifdef __cplusplus
}
#endif // __cplusplus
