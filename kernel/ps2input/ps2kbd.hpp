#pragma once

#include <inputdevice.hpp>
#include <ints.hpp>
#include <types.h>

class PS2Keyboard : public InputDevice
{
    bool ex = false;
    Ints::Handler interruptHandler;

    static bool interrupt(Ints::State *state, void *context);
public:
    PS2Keyboard();
    virtual const char *GetName();
    virtual ~PS2Keyboard();
};
