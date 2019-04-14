#pragma once

#include <list.hpp>
#include <mutex.hpp>
#include <queue.hpp>
#include <semaphore.hpp>
#include <sequencer.hpp>
#include <virtualkey.hpp>

// USER_MATCH: libwoot/woot/input.h
#define INP_MAX_RAW_BYTES           64
#define INP_MAX_MOUSE_AXES          13
#define INP_MAX_TABLET_COORDS       7
#define INP_MAX_TABLET_AXES         6
#define INP_MAX_CONTROLLER_COORDS   13

// USER_MATCH: libwoot/woot/input.h
#define INP_KBD_EVENT_FLAG_RELEASE  1

class Process;

class InputDevice
{
    static Sequencer<int> ids;
    static List<InputDevice *> devices;
    static Mutex listLock;

    static bool lockList();
    static void unLockList();
    static bool append(InputDevice *dev);
    static bool remove(InputDevice *dev);
public:
    // USER_MATCH: libwoot/woot/input.h
    enum class Type
    {
        Unknown = 0,
        Other,
        Keyboard,
        Mouse,
        Tablet,
        Controller
    };

    struct Event
    {
        Type DeviceType;
        InputDevice *Device;
        union
        {
            // USER_MATCH: libwoot/woot/input.h struct inpKeyboardEvent
            struct
            {
                VirtualKey Key;
                bool Release;
            } Keyboard;

            // USER_MATCH: libwoot/woot/input.h struct inpMouseEvent
            struct
            {
                uint32_t ButtonsPressed;
                uint32_t ButtonsHeld;
                uint32_t ButtonsReleased;
                int32_t Delta[INP_MAX_MOUSE_AXES];
            } Mouse;

            // USER_MATCH: libwoot/woot/input.h struct inpTabletEvent
            struct
            {
                uint32_t ButtonsPressed;
                uint32_t ButtonsHeld;
                uint32_t ButtonsReleased;
                int32_t Coords[INP_MAX_TABLET_COORDS];
                int32_t Delta[INP_MAX_TABLET_AXES];
            } Tablet;

            // USER_MATCH: libwoot/woot/input.h struct inpControllerEvent
            struct
            {
                uint32_t ButtonsPressed;
                uint32_t ButtonsHeld;
                uint32_t ButtonsReleased;
                int32_t Coords[INP_MAX_CONTROLLER_COORDS];
            } Controller;

            uint8_t RawData[INP_MAX_RAW_BYTES];
        };

        Event();
        Event(InputDevice *device, VirtualKey key, bool release); // keyboard event constructor
        Event(InputDevice *device, int axes, int32_t *movement, uint32_t pressed, uint32_t released, uint32_t held); // mouse event constructor
    };
protected:
    int id;
    Type type;
    Mutex mutex;
    Semaphore eventSem;
    Queue<Event> events;
    Process *owner;

    InputDevice(Type type);
public:
    static size_t GetCount();
    static InputDevice *GetDefault(InputDevice::Type type);
    static InputDevice *GetById(int id);
    static bool ForEach(bool (*callback)(InputDevice *type, void *arg), void *arg);
    static int ListIds(int *buf, size_t bufSize);

    InputDevice::Type GetType() const;
    int GetEvent(InputDevice::Event *event, int timeout);

    virtual int Open();
    virtual int Close();
    virtual const char *GetName();
    virtual ~InputDevice();
};
