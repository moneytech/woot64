#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syscalls/syscalls.h>
#include <unistd.h>
#include <woot/input.h>
#include <woot/pixmap.h>
#include <woot/video.h>

static const char *devTypeNames[] =
{
    "unknown input device",
    "other input device",
    "keyboard",
    "mouse",
    "tablet",
    "game controller"
};

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    int displCount = vidGetDisplayCount();
    int displDefId = vidGetDefaultDisplayId();
    char displName[64];
    vidGetDisplayName(displDefId, displName, sizeof(displName));
    printf("Display count: %d\n", displCount);
    printf("Default display id: %d\n", displDefId);
    printf("Default display name: %s\n", displName);

    int displFd = vidOpenDisplay(displDefId);
    printf("Display handle: %d\n", displFd);

    if(displFd >= 0)
    {
        int modeCount = vidGetModeCount(displFd);
        printf("Display mode count: %d\n", modeCount);
        for(int i = 0; i < modeCount; ++i)
        {
            vidModeInfo_t mi;
            vidGetModeInfo(displFd, i, &mi);
            printf("  (%d) %ux%u %d bpp @%dHz\n", i, mi.Width, mi.Height, mi.BitsPerPixel, mi.RefreshRate);
        }
    }

    int inDevCount = inpGetDeviceCount();
    printf("Input device count: %d\n", inDevCount);
    int inDevIds[64];
    inpDeviceListIds(inDevIds, sizeof(inDevIds) / 4);
    int kbId = -1;
    for(int i = 0; i < inDevCount; ++i)
    {
        char inDevName[64] = { 0 };
        int id = inDevIds[i];
        inpGetDeviceName(id, inDevName, sizeof(inDevName));
        int type = inpGetDeviceType(id);
        if(kbId < 0 && type == INP_DEV_TYPE_KEYBOARD)
            kbId = id;
        printf("  (%d) %s (%s)\n", id, inDevName, devTypeNames[type]);
    }

    int inpFd = inpOpenDevice(kbId);
    if(inpFd >= 0)
    {
        vidModeInfo_t mi;
        vidGetModeInfo(displFd, vidGetCurrentMode(displFd), &mi);
        pmPixelFormat_t pf = pmFormatFromModeInfo(&mi);
        void *pixels = vidMapPixels(displFd, NULL);
        pmPixMap_t *pm = pmFromMemory(mi.Width, mi.Height, mi.Pitch, &pf, pixels, 0);

        printf("Press escape to exit or 1 or 2 to do some testing\n");
        inpKeyboardEvent_t event;
        for(;;)
        {
            if(inpGetEvent(inpFd, INP_TIMEOUT_FOREVER, &event) < 0)
                continue;
            if(!(event.Flags & INP_KBD_EVENT_FLAG_RELEASE))
            {
                if(event.Key == VK_ESCAPE)
                    break;
                else if(event.Key == VK_KEY1)
                    pmClear(pm, pmColorFromRGB(rand(), rand(), rand()));
                else if(event.Key == VK_KEY2)
                {
                    for(int i = 0; i < 100; ++i)
                    {
                        pmColor_t color = pmColorFromRGB(rand(), rand(), rand());
                        pmLine(pm, rand() % mi.Width, rand() % mi.Height, rand() % mi.Width, rand() % mi.Height, color);
                    }
                }
            }
        }
    }

    printf("Exiting %s\n", argv[0]);
    inpCloseDevice(inpFd);
    vidCloseDisplay(displFd);
    return 9001;
}
