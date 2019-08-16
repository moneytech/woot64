#pragma once

#include <ints.hpp>
#include <types.h>

class Stream;

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define clamp(_min, _max, val) (max((_min), min((_max), (val))))
#define align(val, alignment) ((alignment) * (((val) + ((alignment) - 1)) / (alignment)))
#define swap(T, a, b) { T t = (a); (a) = (b); (b) = (t); }
#define offsetof __builtin_offsetof

#define c_func_alias(al, name) extern "C" void al() __attribute__((alias(#name)))

#define IS_ERROR(val) (intptr_t(val) < 0 && intptr_t(val) >= -4096)

class Misc
{
    static Ints::Handler handler;
    static bool int3Handler(Ints::State *state, void *context);
public:
    static void InitializeDebugStream();
    static void InitializeBreakpointInt();
    static uint64_t PowMax(uint64_t base, uint64_t exp);

    template<class T>
    static T TestAndSet(T *lock, T val)
    {
        return __sync_lock_test_and_set(lock, val);
    }

    template<class T>
    static void Release(T *lock)
    {
        return __sync_lock_release(lock);
    }
};
