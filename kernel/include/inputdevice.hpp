#pragma once

#include <list.hpp>
#include <mutex.hpp>
#include <queue.hpp>
#include <semaphore.hpp>
#include <sequencer.hpp>
#include <virtualkey.hpp>

#define INP_MAX_RAW_BYTES           64
#define INP_MAX_MOUSE_AXES          13
#define INP_MAX_TABLET_COORDS       7
#define INP_MAX_TABLET_AXES         6
#define INP_MAX_CONTROLLER_COORDS   13

#define INP_DEV_TYPE_UNKNOWN        0
#define INP_DEV_TYPE_OTHER          1
#define INP_DEV_TYPE_KEYBOARD       2
#define INP_DEV_TYPE_MOUSE          3
#define INP_DEV_TYPE_TABLET         4
#define INP_DEV_TYPE_CONTROLLER     5

#define INP_KBD_EVENT_FLAG_RELEASE  1

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
            union
            {
                struct
                {
                    VirtualKey Key;
                    bool Release;
                } Keyboard;

                struct
                {
                    uint32_t ButtonsPressed;
                    uint32_t ButtonsHeld;
                    uint32_t ButtonsReleased;
                    int32_t Delta[INP_MAX_MOUSE_AXES];
                } Mouse;

                struct
                {
                    uint32_t ButtonsPressed;
                    uint32_t ButtonsHeld;
                    uint32_t ButtonsReleased;
                    int32_t Coords[INP_MAX_TABLET_COORDS];
                    int32_t Delta[INP_MAX_TABLET_AXES];
                } Tablet;

                struct
                {
                    uint32_t ButtonsPressed;
                    uint32_t ButtonsHeld;
                    uint32_t ButtonsReleased;
                    int32_t Coords[INP_MAX_CONTROLLER_COORDS];
                } Controller;

                uint8_t RawData[INP_MAX_RAW_BYTES];
            };
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

    InputDevice(Type type);
public:
    static InputDevice *GetDefault(InputDevice::Type type);
    static InputDevice *GetById(int id);
    static bool ForEach(bool (*callback)(InputDevice *type, void *arg), void *arg);

    InputDevice::Type GetType() const;
    int GetEvent(InputDevice::Event *event, uint timeout);

    virtual ~InputDevice();
};
