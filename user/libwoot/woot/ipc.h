#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <sys/types.h>

#define MSG_PAYLOAD_SIZE            112

#define MSG_FLAG_NONE               0
#define MSG_FLAG_ACK_REQ            1

#define MSG_NULL                    0
#define MSG_ACK                     1
#define MSG_NACK                    2
#define MSG_PING                    3
#define MSG_PONG                    4
#define MSG_QUIT                    5

#define IPC_SHMEM_WRITE 1

typedef struct ipcMessage
{
    int Number, Flags, ID, Source;
    unsigned char Data[MSG_PAYLOAD_SIZE];
} ipcMessage_t;

int ipcSendMessage(pid_t dst, int num, int flags, void *payload, size_t payloadSize);
int ipcAckMessage(ipcMessage_t *msg);
int ipcGetMessage(ipcMessage_t *msg, int timeout);
int ipcProcessMessage(ipcMessage_t *msg);

int ipcCreateSharedMem(const char *name, size_t size);
int ipcOpenSharedMem(const char *name);
int ipcCloseSharedMem(int handle);
size_t ipcGetSharedMemSize(int handle);
void *ipcMapSharedMem(int handle, void *hint, unsigned flags);
int ipcUnMapSharedMem(int handle, void *addr);

#ifdef __cplusplus
}
#endif // __cplusplus
