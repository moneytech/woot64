#pragma once

#include <types.h>

class Random
{
    static uint rsx;
    static uint rsy;
    static uint rsz;
    static uint rsw;
public:
    static const uint MaxValue;
    static void SetSeed(uint seed);
    static uint GetValue();
};
