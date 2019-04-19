#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syscalls/syscalls.h>
#include <unistd.h>
#include <woot/input.h>
#include <woot/ipc.h>
#include <woot/pixmap.h>
#include <woot/thread.h>
#include <woot/video.h>
#include <woot/wm.h>
#include <zlib.h>

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    printf("[usertest] zlib version: %s\n", zlibVersion());

    int res = wmInitialize();
    const char *wmServer = wmGetServer();
    if(res < 0)
    {
        printf("[usertest] wmInitialize() failed\n");
        return -1;
    }
    else printf("[usertest] Window manager server: '%s'\n", wmServer);

    wmWindow_t *window = wmCreateWindow(100, 200, 300, 200, WM_CWF_DEFAULT);
    if(!window) return -errno;

    pmPixMap_t *pm = wmGetPixMap(window);
    if(!pm)
    {
        wmDeleteWindow(window);
        return -errno;
    }

    ipcMessage_t msg;
    for(int i = 0;; ++i)
    {
        int quit = 0;
        while(ipcGetMessage(&msg, 0) >= 0)
        {
            ipcProcessMessage(&msg);
            if(msg.Number == MSG_QUIT)
            {
                quit = 1;
                break;
            }
        }
        if(quit) break;

        pmColor_t color = pmColorFromRGB(rand(), rand(), rand());
        pmClear(pm, color);
        pmRectangleRect(pm, &pm->Contents, pmColorWhite);
        pmLine(pm, 0, 0, pm->Contents.Width - 1, pm->Contents.Height - 1, pmColorWhite);
        pmLine(pm, 0, pm->Contents.Height - 1, pm->Contents.Width - 1, 0, pmColorWhite);
        wmRedrawWindow(window);
        threadSleep(THREAD_SELF, 250);
    }

    printf("[usertest] Closing usertest\n");

    wmDeleteWindow(window);

    return 0;
}
