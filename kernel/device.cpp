#include <device.hpp>

Device *Device::Root;

Device::Device(Device *parent) :
    Parent(parent)
{
}

const char *Device::GetDisplayName()
{
    return "/";
}

Device::~Device()
{
    for(Device *child : Children)
        delete child;
}
