#include <errno.h>
#include <woot/input.h>
#include <sys/syscall.h>
#include <unistd.h>

int inpGetDeviceCount()
{
    //return syscall(SYS_INDEV_GET_COUNT);
    return -ENOSYS;
}

int inpDeviceList(char *buf, size_t bufSize)
{
    //return syscall(SYS_INDEV_LIST, buf, bufSize);
    return -ENOSYS;
}

int inpOpenDevice(const char *name)
{
    //return syscall(SYS_INDEV_OPEN, name);
    return -ENOSYS;
}

int inpCloseDevice(int handle)
{
    //return syscall(SYS_INDEV_CLOSE, handle);
    return -ENOSYS;
}

int inpGetDeviceType(int handle)
{
    //return syscall(SYS_INDEV_GET_TYPE, handle);
    return -ENOSYS;
}

int inpGetDeviceName(int handle, char *buf, size_t bufSize)
{
    //return syscall(SYS_INDEV_GET_NAME, handle, buf, bufSize);
    return -ENOSYS;
}

int inpGetEvent(int handle, int timeout, void *buf)
{
    //return syscall(SYS_INDEV_GET_EVENT, handle, timeout, buf);
    return -ENOSYS;
}
