#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>
#include <sys/types.h>
#include <woot/ipc.h>

#define MSG_RPC_RESP_PAYLOAD_SIZE (MSG_PAYLOAD_SIZE - sizeof(int))

typedef struct ipcRPCResponse
{
    int RequestMessageID;
    unsigned char Results[MSG_RPC_RESP_PAYLOAD_SIZE];
} ipcRPCResponse_t;

int rpcFindServer(const char *type, char *addr, size_t addrBufSize, int timeout);
int rpcIPCFindServerRespond(pid_t dst, int reqMsgId);
int rpcCall(const char *addr, const char *proc, void *args, size_t argsSize, void *respBuf, size_t respBufSize, int timeout);
int rpcIPCReturn(pid_t dst, int reqMsgId, void *resp, size_t respSize);

#ifdef __cplusplus
}
#endif // __cplusplus
