#pragma once

#include <inputdevice.hpp>
#include <sequencer.hpp>
#include <types.h>
#include <vararg.h>

#define MSG_PAYLOAD_SIZE            112

struct ipcMessage
{
    int Number, Flags, ID, Source;
    unsigned char Data[MSG_PAYLOAD_SIZE];
};

class IPC
{
public:
    static bool waitMessageComparer(ipcMessage *a, ipcMessage *b, VarArgs args);
private:
    static Sequencer<int> ids;
public:
    static int SendMessage(pid_t dst, int number, int flags, void *payload, size_t payloadSize);
    static int GetMessage(ipcMessage *msg, int timeout);
    static int PeekMessage(ipcMessage *msg, unsigned offset);
    static int WaitMessage(ipcMessage *msg, int number, int source, int rangeStart, int rangeSize, int timeout);
};
