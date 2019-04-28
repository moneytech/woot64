#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syscalls/syscalls.h>
#include <time.h>
#include <unistd.h>
#include <woot/font.h>
#include <woot/input.h>
#include <woot/ipc.h>
#include <woot/pixmap.h>
#include <woot/thread.h>
#include <woot/video.h>
#include <woot/wm.h>

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

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
    wmSetWindowTitle(window, "Test window");

    pmPixMap_t *pm = wmGetPixMap(window);
    if(!pm)
    {
        wmDeleteWindow(window);
        return -errno;
    }

    fntFont_t *fnt = fntLoad("/default.ttf");
    if(!fnt)
    {
        wmDeleteWindow(window);
        return -errno;
    }
    fntSetPointSize(fnt, 14, 96);

    srand(time(NULL));
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

        if((i % 10) == 9)
            wmSetWindowPos(window, rand() % 500, rand() % 500);

        char buf[64];
        time_t ct = time(NULL);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", gmtime(&ct));

        int strW = fntMeasureString(fnt, buf);

        pmColor_t color = pmColorFromRGB(rand(), rand(), rand());
        pmClear(pm, color);
        fntDrawString(fnt, pm, (pm->Contents.Width - strW) / 2, 0, buf, pmColorWhite);
        pmRectangleRect(pm, &pm->Contents, pmColorWhite);
        pmLine(pm, 0, 0, pm->Contents.Width - 1, pm->Contents.Height - 1, pmColorWhite);
        pmLine(pm, 0, pm->Contents.Height - 1, pm->Contents.Width - 1, 0, pmColorWhite);
        wmRedrawWindow(window);
        threadSleep(THREAD_SELF, 250);
    }

    printf("[usertest] Closing usertest\n");

    fntDelete(fnt);
    wmDeleteWindow(window);

    return 0;
}
