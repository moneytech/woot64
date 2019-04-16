#include <errno.h>
#include <ipc.hpp>
#include <memory.hpp>
#include <misc.hpp>
#include <process.hpp>

Sequencer<int> IPC::ids(0);

int IPC::SendMessage(pid_t dst, int number, int flags, void *payload, size_t payloadSize)
{
    if(!dst)
    {   // broadcast message
        ipcMessage msg = { number, flags, ids.GetNext(), Process::GetCurrent()->Id };
        Memory::Zero(msg.Data, sizeof(msg.Data));
        if(payload) Memory::Move(msg.Data, payload, min(sizeof(msg.Data), payloadSize));
        Process::ForEach([](Process *proc, void *arg) -> bool
        {
            proc->Messages.Write(*(ipcMessage *)arg, 0);
            return true;
        }, &msg);
        return msg.ID;
    }

    Process *dstProc = Process::GetByID(dst);
    if(!dstProc) return -ESRCH;
    ipcMessage msg = { number, flags, ids.GetNext(), Process::GetCurrent()->Id };
    Memory::Zero(msg.Data, sizeof(msg.Data));
    if(payload) Memory::Move(msg.Data, payload, min(sizeof(msg.Data), payloadSize));
    int res = dstProc->Messages.Write(msg, 100);
    if(res < 0) return res;
    return msg.ID;
}

int IPC::GetMessage(ipcMessage *msg, int timeout)
{
    return Process::GetCurrent()->Messages.Read(msg, timeout);
}
