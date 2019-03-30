#pragma once

#include <types.h>

class Stream;

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define clamp(_min, _max, val) (max((_min), min((_max), (val))))
#define align(val, alignment) ((alignment) * (((val) + ((alignment) - 1)) / (alignment)))
#define swap(T, a, b) { T t = (a); (a) = (b); (b) = (t); }
#define offsetof __builtin_offsetof

#define c_func_alias(al, name) extern "C" void al() __attribute__((alias(#name)))

#define IS_ERROR(val) ((intptr_t)(val) < 0 && (intptr_t)(val) >= -4096)

class Misc
{
public:
    static void InitializeDebugStream();
    static uint64_t PowMax(uint64_t base, uint64_t exp);
};
