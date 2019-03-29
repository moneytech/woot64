#pragma once

typedef __INT8_TYPE__ int8_t;
typedef __INT16_TYPE__ int16_t;
typedef __INT32_TYPE__ int32_t;
typedef __INT64_TYPE__ int64_t;

typedef __UINT8_TYPE__ uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;

typedef __UINTPTR_TYPE__ uintptr_t;
typedef __INTPTR_TYPE__ intptr_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __SIZE_TYPE__ size_t;

// define natural integer size for this architecture
#if(__SIZE_WIDTH__ == 64)
typedef int64_t intn;
typedef uint64_t uintn;
#elif(__SIZE_WIDTH__ == 32)
typedef int32_t intn;
typedef uint32_t uintn;
#else
#error Can compile only for 32 or 64 bit architectures
#endif // __SIZE_WIDTH__

// these should go to their respective headers
typedef int pid_t;

typedef unsigned int uint;
