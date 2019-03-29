#pragma once

#include <ints.hpp>
#include <types.h>

class IRQs
{
public:
    static const int Base;
    static const int Count;
    static uint64_t SpuriousIRQCount;

    static void Initialize();
    static void Enable(uint irq);
    static void Disable(uint irq);
    static void TryDisable(uint irq); // disable irq only if there are no more handlers left
    static bool IsEnabled(uint irq);
    static void SendEOI(uint irq);
    static bool IsSpurious(uint irq);
    static void HandleSpurious(uint irq);
    static void RegisterHandler(uint irq, Ints::Handler *handler);
    static void UnRegisterHandler(uint irq, Ints::Handler *handler);
};
