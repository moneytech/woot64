#include <woot/input.h>
#include <syscalls/syscalls.h>

int inpGetDeviceCount()
{
    return sysInDevGetCount();
}

int inpDeviceListIds(int *buf, size_t bufSize)
{
    return sysInDevListIds(buf, bufSize);
}

int inpGetDeviceType(int id)
{
    return sysInDevGetType(id);
}

int inpGetDeviceName(int id, char *buf, size_t bufSize)
{
    return sysInDevGetName(id, buf, bufSize);
}

int inpOpenDevice(int id)
{
    return sysInDevOpen(id);
}

int inpCloseDevice(int fd)
{
    return sysInDevClose(fd);
}

int inpGetEvent(int fd, int timeout, void *buf)
{
    return sysInDevGetEvent(fd, timeout, buf);
}
