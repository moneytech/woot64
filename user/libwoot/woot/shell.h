#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int shellInitialize();
int shellCleanup();
const char *shellGetEditor(const char *filename, int idx);

#ifdef __cplusplus
}
#endif // __cplusplus
