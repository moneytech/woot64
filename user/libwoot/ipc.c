#include <errno.h>
#include <syscalls/syscalls.h>
#include <unistd.h>
#include <woot/ipc.h>

int ipcSendMessage(pid_t dst, int num, int flags, void *payload, size_t payloadSize)
{
    return sysIPCSendMessage(dst, num, flags, payload, payloadSize);
}

int ipcGetMessage(ipcMessage_t *msg, int timeout)
{
    return syscall(SYS_IPC_GET_MESSAGE, msg, timeout);
}

int ipcProcessMessage(ipcMessage_t *msg)
{
    if(!msg) return -EINVAL;
    if(msg->Flags & MSG_FLAG_ACK_REQ)
        ipcSendMessage(msg->Source, MSG_ACK, MSG_FLAG_NONE, NULL, 0);
    if(msg->Number == MSG_PING)
        ipcSendMessage(msg->Source, MSG_PONG, MSG_FLAG_NONE, NULL, 0);
    return 0;
}

int ipcCreateSharedMem(const char *name, size_t size)
{
    return sysIPCCreateSharedMem(name, size);
}

int ipcOpenSharedMem(const char *name)
{
    return sysIPCOpenSharedMem(name);
}

int ipcCloseSharedMem(int handle)
{
    return sysIPCCloseSharedMem(handle);
}

size_t ipcGetSharedMemSize(int handle)
{
    return sysIPCGetSharedMemSize(handle);
}

void *ipcMapSharedMem(int handle, void *hint, unsigned flags)
{
    return sysIPCMapSharedMem(handle, hint, flags);
}

int ipcUnMapSharedMem(int handle, void *addr)
{
    return sysIPCUnMapSharedMem(handle, addr);
}
