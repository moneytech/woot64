#pragma once

#include <inputdevice.hpp>
#include <sequencer.hpp>
#include <types.h>

#define MSG_PAYLOAD_SIZE            112

struct ipcMessage
{
    int Number, Flags, ID, Source;
    unsigned char Data[MSG_PAYLOAD_SIZE];
};

class IPC
{
public:
private:
    static Sequencer<int> ids;
public:
    static int SendMessage(pid_t dst, int number, int flags, void *payload, size_t payloadSize);
    static int GetMessage(ipcMessage *msg, int timeout);
};
