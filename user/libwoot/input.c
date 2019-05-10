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

int inpTranslateKey(int vKey, int modifiers)
{
    static const char *digits = "0123456789";
    static const char *shiftDigits = ")!@#$%^&*(";
    static const char *lowerLetters = "abcdefghijklmnopqrstuvwxyz";
    static const char *upperLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int shift = modifiers & INP_MOD_SHIFT ? 1 : 0;
    int caps = modifiers & INP_MOD_CAPS ? 1 : 0;
    int num = modifiers & INP_MOD_NUM ? 1 : 0;

    if(vKey >= VK_NUMPAD0 && vKey <= VK_NUMPAD9)
    {
        if(num)
            return digits[vKey - VK_NUMPAD0];
    }
    else if(vKey >= VK_KEY0 && vKey <= VK_KEY9)
    {
        unsigned dig = vKey - VK_KEY0;
        return shift ? shiftDigits[dig] : digits[dig];
    }
    else if(vKey >= VK_KEYA && vKey <= VK_KEYZ)
    {
        unsigned let = vKey - VK_KEYA;
        return (shift != caps) ? upperLetters[let] : lowerLetters[let];
    }
    else if(vKey == VK_SPACE)
        return ' ';
    else if(vKey == VK_RETURN)
        return '\n';
    else if(vKey == VK_OEMMINUS)
        return (shift ? '_' : '-');
    else if(vKey == VK_OEMPLUS)
        return (shift ? '+' : '=');
    else if(vKey == VK_OEMCOMMA)
        return (shift ? '<' : ',');
    else if(vKey == VK_OEMPERIOD)
        return (shift ? '>' : '.');
    else if(vKey == VK_OEM1)
        return (shift ? ':' : ';');
    else if(vKey == VK_OEM2)
        return (shift ? '?' : '/');
    else if(vKey == VK_OEM3)
        return (shift ? '~' : '`');
    else if(vKey == VK_OEM4)
        return (shift ? '{' : '[');
    else if(vKey == VK_OEM5)
        return (shift ? '|' : '\\');
    else if(vKey == VK_OEM6)
        return (shift ? '}' : ']');
    else if(vKey == VK_OEM7)
        return (shift ? '"' : '\'');
    else if(vKey == VK_SUBTRACT)
        return '-';
    else if(vKey == VK_ADD)
        return '+';
    else if(vKey == VK_MULTIPLY)
        return '*';
    else if(vKey == VK_DIVIDE)
        return '/';
    else if(vKey == VK_DECIMAL)
        return '.';
    else if(vKey == VK_BACK)
        return '\b';
    else if(vKey == VK_ESCAPE)
        return 0x1B;
    return 0;
}
