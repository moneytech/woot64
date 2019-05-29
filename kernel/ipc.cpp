#include <errno.h>
#include <ipc.hpp>
#include <memory.hpp>
#include <misc.hpp>
#include <process.hpp>

Sequencer<int> IPC::ids(0);

#include <debug.hpp>

bool IPC::waitMessageComparer(ipcMessage *a, ipcMessage *b, VarArgs args)
{
    if(a == b) return true;

    int number = VarArg(args, int);
    int source = VarArg(args, int);
    int rangeStart = VarArg(args, int);
    int rangeSize = VarArg(args, int);

    if(number >= 0 && number != b->Number) return false;
    if(source >= 0 && source != b->Source) return false;
    if(rangeStart >= 0 && rangeSize >= 0)
    {
        if(Memory::Compare(a->Data + rangeStart, b->Data + rangeStart, rangeSize))
            return false;
    }

    return true;
}

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
    int res = dstProc->Messages.Write(msg, 0);
    if(res < 0) return res;
    return msg.ID;
}

int IPC::GetMessage(ipcMessage *msg, int timeout)
{
    return Process::GetCurrent()->Messages.Read(msg, timeout);
}

int IPC::PeekMessage(ipcMessage *msg, unsigned offset)
{
    return Process::GetCurrent()->Messages.Peek(msg, offset);
}

int IPC::WaitMessage(ipcMessage *msg, int number, int source, int rangeStart, int rangeSize, int timeout)
{
    if((rangeStart + rangeSize) > sizeof(msg->Data))
        return -EINVAL;

    return Process::GetCurrent()->Messages.Wait(msg, waitMessageComparer, timeout, number, source,
                                                rangeStart, rangeSize);
}

