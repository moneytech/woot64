#pragma once

#define SIGNAL_COUNT 128

#include <ints.hpp>
#include <types.h>

class Thread;

class Signal
{
public:
    static void ReturnFromSignal(Thread *thread, Ints::State *state);
    static void HandleSignals(Thread *thread, Ints::State *state);
    static void HandleSignals(Thread *thread, uintptr_t *retAddrAddr);
    static void Raise(Thread *thread, unsigned signum);
};
