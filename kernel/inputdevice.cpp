#include <cpu.hpp>
#include <errno.h>
#include <inputdevice.hpp>
#include <memory.hpp>
#include <misc.hpp>
#include <process.hpp>
#include <string.hpp>
#include <stringbuilder.hpp>

Sequencer<int> InputDevice::ids(0);
List<InputDevice *> InputDevice::devices;
Mutex InputDevice::listLock(false, "InputDevice::listLock");

bool InputDevice::lockList()
{
    return listLock.Acquire(0, false);
}

void InputDevice::unLockList()
{
    listLock.Release();
}

bool InputDevice::append(InputDevice *dev)
{
    if(!lockList()) return false;
    devices.Append(dev);
    unLockList();
    return true;
}

bool InputDevice::remove(InputDevice *dev)
{
    if(!lockList()) return false;
    uint res = devices.Remove(dev, nullptr, false);
    unLockList();
    return res != 0;
}

InputDevice::InputDevice(InputDevice::Type type) :
    id(ids.GetNext()), type(type), mutex(false, "InputDevice::mutex"),
    eventSem(0, "InputDevice::eventSem"), events(64)
{
    append(this);
}

size_t InputDevice::GetCount()
{
    lockList();
    size_t res = devices.Count();
    unLockList();
    return res;
}

InputDevice *InputDevice::GetDefault(Type type)
{
    if(!lockList()) return nullptr;
    InputDevice *res = nullptr;
    for(InputDevice *dev : devices)
    {
        if(dev->type == type)
        {
            res = dev;
            break;
        }
    }
    unLockList();
    return res;
}

InputDevice *InputDevice::GetById(int id)
{
    if(!lockList()) return nullptr;
    InputDevice *res = nullptr;
    for(InputDevice *dev : devices)
    {
        if(dev->id == id)
        {
            res = dev;
            break;
        }
    }
    unLockList();
    return res;
}

bool InputDevice::ForEach(bool (*callback)(InputDevice *, void *), void *arg)
{
    if(!lockList()) return false;
    for(InputDevice *dev : devices)
    {
        if(callback(dev, arg))
            break;
    }
    unLockList();
    return true;
}

int InputDevice::ListIds(int *buf, size_t bufSize)
{
    if(!buf || !bufSize)
        return -EINVAL;
    if(!lockList()) return -EBUSY;
    int res = 0;
    for(InputDevice *dev : devices)
    {
        if(res >= bufSize)
            break;
        buf[res++] = dev->id;
    }
    unLockList();
    return res;
}

InputDevice::Type InputDevice::GetType() const
{
    return type;
}

int InputDevice::GetEvent(Event *event, int timeout)
{
    if(!mutex.Acquire(timeout >= 0 ? timeout : 0, false))
        return -EBUSY;
    int timeleft = eventSem.Wait(timeout < 0 ? 0 : timeout, timeout == 0, true);
    if(timeleft < 0)
    {
        mutex.Release();
        return -ETIMEOUT;
    }
    bool ok = false;
    if(event) *event = events.Read(&ok);
    else events.Read(&ok);
    cpuEnableInterrupts();
    mutex.Release();
    return ok ? timeleft : -EIO;
}

int InputDevice::Open()
{
    Process *cp = Process::GetCurrent();
    if(Misc::TestAndSet(&owner, cp))
        return -EBUSY;
    return ESUCCESS;
}

int InputDevice::Close()
{
    Misc::Release(&owner);
    return ESUCCESS;
}

const char *InputDevice::GetName()
{
    return "Generic input device";
}

InputDevice::~InputDevice()
{
    remove(this);
}

InputDevice::Event::Event()
{
}

InputDevice::Event::Event(InputDevice *device, VirtualKey key, bool release) :
    DeviceType(device->type), Device(device),
    Keyboard { key, release }
{
}

InputDevice::Event::Event(InputDevice *device, int axes, int32_t *movement, uint32_t pressed, uint32_t released, uint32_t held) :
    DeviceType(device->type), Device(device),
    Mouse { pressed, held, released }
{
    Memory::Move(Mouse.Delta, movement, sizeof(int32_t) * min(axes, INP_MAX_MOUSE_AXES));
}
