#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <sys/types.h>
#include <woot/vkeys.h>

#define INP_TIMEOUT_NOWAIT  0
#define INP_TIMEOUT_FOREVER (-1)

// KERNEL_MATCH: inputdevice.hpp
#define INP_MAX_RAW_BYTES           64
#define INP_MAX_MOUSE_AXES          13
#define INP_MAX_TABLET_COORDS       7
#define INP_MAX_TABLET_AXES         6
#define INP_MAX_CONTROLLER_COORDS   13

// KERNEL_MATCH: inputdevice.hpp enum InputDevice::Type
#define INP_DEV_TYPE_UNKNOWN        0
#define INP_DEV_TYPE_OTHER          1
#define INP_DEV_TYPE_KEYBOARD       2
#define INP_DEV_TYPE_MOUSE          3
#define INP_DEV_TYPE_TABLET         4
#define INP_DEV_TYPE_CONTROLLER     5

// KERNEL_MATCH: inputdevice.hpp
#define INP_KBD_EVENT_FLAG_RELEASE  1

// KERNEL_MATCH: inputdevice.cpp InputDevice::Event::Keyboard
typedef struct inpKeyboardEvent
{
    unsigned Key;
    unsigned Flags;
    unsigned Padding[14];
} inpKeyboardEvent_t;

// KERNEL_MATCH: inputdevice.cpp InputDevice::Event::Mouse
typedef struct inpMouseEvent
{
    unsigned ButtonsPressed;
    unsigned ButtonsHeld;
    unsigned ButtonsReleased;
    int Delta[INP_MAX_MOUSE_AXES];
} inpMouseEvent_t;

// KERNEL_MATCH: inputdevice.cpp InputDevice::Event::Tablet
typedef struct inpTabletEvent
{
    unsigned ButtonsPressed;
    unsigned ButtonsHeld;
    unsigned ButtonsReleased;
    int Coords[INP_MAX_TABLET_COORDS];
    int Delta[INP_MAX_TABLET_AXES];
} inpTabletEvent_t;

// KERNEL_MATCH: inputdevice.cpp InputDevice::Event::Controller
typedef struct inpControllerEvent
{
    unsigned ButtonsPressed;
    unsigned ButtonsHeld;
    unsigned ButtonsReleased;
    int Coords[INP_MAX_CONTROLLER_COORDS];
} inpControllerEvent_t;

#define INP_MOD_NONE    0
#define INP_MOD_LSHIFT  (1 << 0)
#define INP_MOD_RSHIFT  (1 << 1)
#define INP_MOD_SHIFT   (3 << 0)
#define INP_MOD_LCTRL   (1 << 2)
#define INP_MOD_RCTRL   (1 << 3)
#define INP_MOD_CTRL    (3 << 2)
#define INP_MOD_LALT    (1 << 4)
#define INP_MOD_RALT    (1 << 5)
#define INP_MOD_ALT     (3 << 4)
#define INP_MOD_LSUPER  (1 << 6)
#define INP_MOD_RSUPER  (1 << 7)
#define INP_MOD_SUPER   (3 << 6)
#define INP_MOD_CAPS    (1 << 8)
#define INP_MOD_NUM     (1 << 9)
#define INP_MOD_SCROLL  (1 << 10)

int inpGetDeviceCount();
int inpDeviceListIds(int *buf, size_t bufSize);
int inpGetDeviceType(int id);
int inpGetDeviceName(int id, char *buf, size_t bufSize);
int inpOpenDevice(int id);
int inpCloseDevice(int fd);
int inpGetEvent(int fd, int timeout, void *buf);
int inpProcessKeyboardEvent(inpKeyboardEvent_t *event, int *modifiers);
int inpTranslateKey(int vKey, int modifiers);

#ifdef __cplusplus
}
#endif // __cplusplus
