#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void *signalGetHandler(unsigned signum);
int signalSetHandler(unsigned signum, void *handler);
int signalIsEnabled(unsigned signum);
int signalEnable(unsigned signum);
int signalDisable(unsigned signum);
int signalRaise(int tid, unsigned signum);
int signalReturn(void);
int signalGetCurrent(void);

#ifdef __cplusplus
}
#endif // __cplusplus
