#pragma once

#include <list.hpp>
#include <types.h>

class Device
{
public:
    static Device *Root;

    Device *Parent;
    List<Device *> Children;

    Device(Device *parent);

    virtual const char *GetDisplayName();
    virtual ~Device();
};
