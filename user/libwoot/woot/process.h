#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int processCreate(const char *cmdline);
int processDelete(int handle);
int processWait(int handle, int timeout);
int processAbort(int handle, int result);
int processListIds(int *buf, unsigned bufSize);
int processGetName(int pid, char *buf, unsigned bufSize);

#ifdef __cplusplus
}
#endif // __cplusplus
