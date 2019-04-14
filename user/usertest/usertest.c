#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syscalls/syscalls.h>
#include <unistd.h>
#include <woot/video.h>

#define offsetof __builtin_offsetof

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
            printf("  %d. %ux%u %d bpp @%dHz\n", i + 1, mi.Width, mi.Height, mi.BitsPerPixel, mi.RefreshRate);
        }
    }

    for(int i = 0; i < 3; ++i)
        printf("I like trains. %d\n", i);

    vidCloseDisplay(displFd);
    return 9001;
}
