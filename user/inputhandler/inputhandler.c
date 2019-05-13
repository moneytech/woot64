#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <woot/input.h>
#include <woot/ipc.h>
#include <woot/thread.h>

static volatile int done = 0;
static int keyboardOwner = -1;
static int mouseOwner = -1;

static int kbdThread(int arg)
{
    threadDaemonize();
    int handle = arg;
    inpKeyboardEvent_t event;
    for(int i = 0; !done; ++i)
    {
        int res = inpGetEvent(handle, 1000, &event);
        if(res < 0)
        {
            /*if(res == -EBADF)
            {
                printf("[inputhandler] Keyboard event read problem. (handle: %d)\n", handle);
                break;
            }*/
            continue;
        }
        //printf("key: %d %s\n", event.Key, event.Flags & INP_KBD_EVENT_FLAG_RELEASE ? "released" : "pressed");

        if(keyboardOwner >= 0)
            ipcSendMessage(keyboardOwner, MSG_KEYBOARD_EVENT, MSG_FLAG_NONE, &event, sizeof(event));
    }
    inpCloseDevice(handle);
    return 0;
}

static int mouseThread(int arg)
{
    threadDaemonize();
    int handle = arg;
    inpMouseEvent_t event;
    for(int i = 0; !done; ++i)
    {
        int res = inpGetEvent(handle, 1000, &event);
        if(res < 0)
        {
            /*if(res == -EBADF)
            {
                printf("[inputhandler] Mouse event read problem. (handle: %d)\n", handle);
                break;
            }*/
            continue;
        }
        //printf("mouse delta: %d %d, buttons: pressed: %d held: %d released: %d\n",
        //       event.Delta[0], event.Delta[1],
        //       event.ButtonsPressed, event.ButtonsHeld, event.ButtonsReleased);

        if(mouseOwner >= 0)
            ipcSendMessage(mouseOwner, MSG_MOUSE_EVENT, MSG_FLAG_NONE, &event, sizeof(event));

    }
    inpCloseDevice(handle);
    return 0;
}

int main()
{
    setbuf(stdout, NULL);

    int devCount = inpGetDeviceCount();
    printf("[inputhandler] Found %d input devices\n", devCount);

    int *devIds = (int *)calloc(devCount, sizeof(int));
    int *threads = (int *)calloc(devCount, sizeof(int));

    inpDeviceListIds(devIds, devCount);
    for(int i = 0; i < devCount; ++i)
    {
        int id = devIds[i];
        int devType = inpGetDeviceType(id);
        int devHandle = inpOpenDevice(id);
        if(devHandle < 0)
        {
            printf("[inputhandler] Couldn't open device %d\n", id);
            continue;
        }
        switch(devType)
        {
        default:
            inpCloseDevice(devHandle);
            printf("[inputhandler] I don't know how to handle device %d\n", id);
            threads[i] = -1;
            break;
        case INP_DEV_TYPE_KEYBOARD:
            printf("[inputhandler] Creating handler for keyboard %d(%d)\n", id, devHandle);
            threads[i] = threadCreate("keyboard thread", kbdThread, devHandle, NULL);
            break;
        case INP_DEV_TYPE_MOUSE:
            printf("[inputhandler] Creating handler for mouse %d(%d)\n", id, devHandle);
            threads[i] = threadCreate("mouse thread", mouseThread, devHandle, NULL);
            break;
        }
    }

    for(int i = 0; i < devCount; ++i)
    {
        if(threads[i] < 0)
            continue;
        threadResume(threads[i]);
        threadWait(threads[i], -1);
    }

    threadDaemonize();

    ipcMessage_t msg;
    for(;;)
    {
        if(ipcGetMessage(&msg, -1) < 0)
            break;
        ipcProcessMessage(&msg);
        if(msg.Number == MSG_QUIT)
        {
            for(int i = 0; i < devCount; ++i)
                threadAbort(threads[i], 0);
            break;
        }
        else if(msg.Number == MSG_ACQUIRE_KEYBOARD)
        {
            if(keyboardOwner < 0)
            {
                keyboardOwner = msg.Source;
                printf("[inputhandler] keyboard acquired by process %d\n", keyboardOwner);
            }
        }
        else if(msg.Number == MSG_ACQUIRE_MOUSE)
        {
            if(mouseOwner < 0)
            {
                mouseOwner = msg.Source;
                printf("[inputhandler] mouse acquired by process %d\n", mouseOwner);
            }
        }
        else if(msg.Number == MSG_RELEASE_KEYBOARD)
            keyboardOwner = -1;
        else if(msg.Number == MSG_RELEASE_MOUSE)
            mouseOwner = -1;
    }

    printf("[inputhandler] Closing inputhandler\n");

    for(int i = 0; i < devCount; ++i)
    {
        if(threads[i] < 0)
            continue;
        threadWait(threads[i], -1);
        threadDelete(threads[i]);
    }

    free(threads);
    free(devIds);
    return 0;
}
