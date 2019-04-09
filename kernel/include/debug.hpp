#pragma once

#include <types.h>

#define DEBUG Debug::DebugFmt
#define DEBUG_IN Debug::DebugIn

class Debug
{
public:
    static void Initialize();
    static void DebugFmt(const char *fmt, ...);
    static int DebugIn(void *buffer, size_t bufSize);
    static int DebugRead(void *buffer, size_t count);
    static int DebugWrite(const void *buffer, size_t count);
    static void BufferDump(void *ptr, size_t n);
};
