#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <woot/ipc.h>
#include <woot/pixmap.h>
#include <woot/process.h>
#include <woot/thread.h>
#include <woot/ui.h>
#include <woot/vkeys.h>
#include <woot/wm.h>

#include "font.hpp"

extern "C" char **environ;

static pmPixMap *conPixMap = nullptr;
static unsigned conX = 0;
static unsigned conY = 0;
static unsigned conWidth = 80;
static unsigned conHeight = 25;
static unsigned conTabSize = 8;
static pmColor_t conTextColor = pmColorGray;
static pmColor_t conBackColor = pmColorBlue;
static char *conCmdBuf = nullptr;
static char *conCmdBuf2 = nullptr;
static char *conOldCmd = nullptr;
static char **conCmdArgs = nullptr;
static unsigned conCmdArgC = 0;
static unsigned conCmdBufSize = 1024;
static char *conCWD = nullptr;
static unsigned conCWDSize = 1024;
static wmWindow_t *conWindow = nullptr;
static rcRectangle_t conDirtyRect = rcRectangleEmpty;
static int conLastUpdate = 512;

static void updateConsole();

static inline void putChar(uint x, uint y, int chr)
{
    unsigned char *glyph = font[chr & 0xFF];
    for(int iy = 0; iy < FONT_SCANLINES; ++iy)
    {
        for(int ix = 0; ix < 8; ++ix)
            pmSetPixel(conPixMap, x + ix, y + iy, (*glyph << ix) & 0x80 ? conTextColor : conBackColor);
        ++glyph;
    }
    rcRectangle_t dirty = { (int)x, (int)y, FONT_BITS, FONT_SCANLINES };
    conDirtyRect = rcAddP(&conDirtyRect, &dirty);
    if(--conLastUpdate < 0)
        updateConsole();
}

static void putChar(int chr)
{
    int chrX = conX * FONT_BITS;
    int chrY = conY * FONT_SCANLINES;

    putChar(chrX, chrY, ' ');

    if(chr == '\n')
    {
        conX = 0;
        ++conY;
    }
    else if(chr == '\r')
        conX = 0;
    else if(chr == '\b')
    {
        if(conX > 0)
            --conX;
        else
        {
            if(conY > 0)
            {
                conX = conWidth - 1;
                --conY;
            }
        }
        chrX = conX * FONT_BITS;
        chrY = conY * FONT_SCANLINES;
        putChar(chrX, chrY, ' ');
    }
    else if(chr == '\t')
        conX = conTabSize * (conX + conTabSize - 1);
    else
    {   // regular character
        putChar(chrX, chrY, chr);
        ++conX;
    }

    if(conX >= conWidth)
    {   // end of line
        conX = 0;
        ++conY;
    }

    if(conY >= conHeight)
    {   // end of screen; scroll
        pmBlit(conPixMap, conPixMap, 0, FONT_SCANLINES, 0, 0, -1, conPixMap->Contents.Height - FONT_SCANLINES);
        pmFillRectangle(conPixMap, 0, conPixMap->Contents.Height - FONT_SCANLINES, conPixMap->Contents.Width, FONT_SCANLINES, conBackColor);
        conDirtyRect = conPixMap->Contents;
        conX = 0;
        --conY;
    }

    chrX = conX * FONT_BITS;
    chrY = conY * FONT_SCANLINES;
}

static void putStr(const char *str)
{
    while(*str)
        putChar(*str++);
}

static void updateConsole()
{
    if(rcIsEmptyP(&conDirtyRect))
        return;
    wmRedrawRect(conWindow, &conDirtyRect);
    conDirtyRect = rcRectangleEmpty;
    conLastUpdate = 512;
}

int stdListener(uintptr_t arg)
{
    int fd = (int)arg;
    char buf[1024];
    for(int i = 0;; ++i)
    {
        ssize_t br = read(fd, buf, sizeof(buf));
        if(br < 0) return br;
        for(int i = 0; i < br; ++i)
            putChar(buf[i]);
        if(br > 0) updateConsole();
    }
    return 0;
}

static bool isError(long value)
{
    return value < 0 && value > -4096;
}

extern "C" int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    int res = 0;
    int pipeFDs[2];
    if((res = pipe(pipeFDs)) < 0)
    {
        fprintf(stderr, "[console] Couldn't create pipe\n");
        return res;
    }

    if((res = dup2(pipeFDs[1], 1)) < 0)
    {
        close(pipeFDs[0]);
        close(pipeFDs[1]);
        fprintf(stderr, "[console] Couldn't redirect stdout\n");
        return res;
    }

    if((res = dup2(pipeFDs[1], 2)) < 0)
    {
        close(pipeFDs[0]);
        close(pipeFDs[1]);
        fprintf(stderr, "[console] Couldn't redirect stderr\n");
        return res;
    }
    close(pipeFDs[1]);

    int listener = threadCreate("stdout/stderr listener", (void *)stdListener, pipeFDs[0], nullptr);
    if(listener < 0)
    {
        fprintf(stderr, "[console] Couldn't create stdout/stderr listener thread\n");
        return listener;
    }

    wmInitialize(WM_INITIALIZE_NONE);

    conWindow = wmCreateWindow(WM_CW_USEDEFAULT, WM_CW_USEDEFAULT, 600, 400, WM_CWF_DEFAULT);
    if(!conWindow)
    {
        close(pipeFDs[0]);
        threadDelete(listener);
        fprintf(stderr, "[console] Couldn't create main window\n");
        return -errno;
    }

    conTextColor = pmColorGray;
    //conBackColor = pmColorBlue;

    uiControl_t *rootControl = wmGetRootControl(conWindow);
    wmSetWindowTitle(conWindow, "Console");
    uiControlSetBorderStyle(rootControl, UI_BORDER_NONE);
    uiControlSetBackColor(rootControl, conBackColor);
    uiControlRedraw(rootControl, UI_TRUE);

    conPixMap = uiControlGetPixMap(rootControl);
    conDirtyRect = conPixMap->Contents;
    conWidth = conPixMap->Contents.Width / FONT_BITS;
    conHeight = conPixMap->Contents.Height / FONT_SCANLINES;
    conCmdBuf = new char[conCmdBufSize];
    conCmdBuf2 = new char[conCmdBufSize];
    conOldCmd = new char[conCmdBufSize];
    int maxCmdArgC = conCmdBufSize / 2;
    conCmdArgs = new char *[maxCmdArgC];
    conCWD = new char[conCWDSize];

    // do that before resuming listener thread
    putStr("WOOT console.\n");
    updateConsole();

    threadResume(listener);

    ipcMessage_t msg;
    unsigned conCmdIdx;
    bool shutdown = false;
    for(;;)
    {
        conCmdIdx = 0;
        memset(conCmdBuf, 0, conCmdBufSize);
        memset(conCmdArgs, 0, sizeof(char *) * maxCmdArgC);
        memset(conCWD, 0, conCWDSize);
        getcwd(conCWD, conCWDSize);
        if(conCWD[0])
        {
            char title[64];
            snprintf(title, sizeof(title), "Console - %s", conCWD);
            wmSetWindowTitle(conWindow, title);
        }
        else
            wmSetWindowTitle(conWindow, "Console");
        printf("%s> ", conCWD[0] ? conCWD : "?");

        bool quit = false;
        for(;;)
        {
            int res = ipcGetMessage(&msg, -1);
            if(res < 0 || msg.Number == MSG_QUIT)
            {
                quit = true;
                break;
            }
            else if(msg.Number == MSG_WM_EVENT)
            {
                wmEvent_t *event = (wmEvent_t *)msg.Data;
                if(event->Type == WM_EVT_CLOSE)
                {
                    quit = true;
                    break;
                }
                else if(event->Type == WM_EVT_CARET_TICK)
                {
                    putChar(conX * FONT_BITS, conY * FONT_SCANLINES, event->CaretTick.Visible ? '_' : ' ');
                    updateConsole();
                }
                /*else if(event->Type == WM_EVT_LOST_FOCUS)
                {
                    putChar(conX * FONT_BITS, conY * FONT_SCANLINES, ' ');
                    updateConsole();
                }*/
                else if(event->Type == WM_EVT_KEYBOARD)
                {
                    int chr = event->Keyboard.Character;
                    if(chr)
                    {
                        if(conCmdIdx > 0 && chr == '\b')
                        {
                            conCmdBuf[--conCmdIdx] = 0;
                            putchar(chr);
                        }
                        else if(chr != '\b' && conCmdIdx < (conCmdBufSize - 1))
                        {
                            if(chr != '\n')
                            {
                                conCmdBuf[conCmdIdx++] = chr;
                                conCmdBuf[conCmdIdx] = 0;
                            }
                            putchar(chr);
                            updateConsole();
                        }
                        if(chr == '\n')
                            break;
                    }

                    if(!(event->Keyboard.Flags & WM_EVT_KB_RELEASED))
                    {
                        if(event->Keyboard.Key == VK_UP)
                        {
                            for(int i = 0; i < conCmdIdx; ++i)
                                putchar('\b');
                            memcpy(conCmdBuf, conOldCmd, conCmdBufSize);
                            conCmdIdx = strlen(conCmdBuf);
                            for(int i = 0; i < conCmdIdx; ++i)
                                putchar(((unsigned)conCmdBuf[i]) & 0xFF);
                            updateConsole();
                        }
                    }
                }
            }
        }
        if(quit) break;

        if(!conCmdBuf[0])
            continue;

        memcpy(conCmdBuf2, conCmdBuf, conCmdBufSize);
        memcpy(conOldCmd, conCmdBuf, conCmdBufSize);
        char *savePtr = nullptr;
        for(conCmdArgC = 0; conCmdArgC < maxCmdArgC; ++conCmdArgC)
        {
            char *arg = strtok_r(conCmdArgC ? NULL : conCmdBuf2, " \t", &savePtr);
            if(!arg) break;
            conCmdArgs[conCmdArgC] = arg;
        }

        if(!strcmp(conCmdArgs[0], "quit") || !strcmp(conCmdArgs[0], "exit"))
            break;
        else if(!strcmp(conCmdArgs[0], "shutdown"))
        {
            shutdown = true;
            break;
        }
        else if(!strcmp(conCmdArgs[0], "pwd"))
            printf("%s\n", conCWD);
        else if(!strcmp(conCmdArgs[0], "env"))
        {
            for(char **env = environ; *env; ++env)
                puts(*env);
        }
        else if(!strcmp(conCmdArgs[0], "cd"))
        {
            if(!conCmdArgs[1])
                puts("missing directory name");
            else
            {
                if(chdir(conCmdArgs[1]) < 0)
                    puts("couldn't change directory");
            }
        }
        else if(!strcmp(conCmdArgs[0], "cat") || !strcmp(conCmdArgs[0], "type"))
        {
            if(!conCmdArgs[1])
                puts("missing filename");
            else
            {
                FILE *f = fopen(conCmdArgs[1], "rb");
                if(!f) printf("couldn't open '%s'\n", conCmdArgs[1]);
                else
                {
                    unsigned char buf[512];
                    int r = 0;
                    unsigned char lastChr = 0;
                    while((r = fread(buf, 1, sizeof(buf), f)) > 0)
                    {
                        for(int i = 0; i < r; ++i)
                        {
                            lastChr = buf[i];
                            putchar(lastChr);
                        }
                    }
                    fclose(f);
                    if(lastChr != '\n')
                        putchar('\n');
                }
            }
        }
        else if(!strcmp(conCmdArgs[0], "ls") || !strcmp(conCmdArgs[0], "dir"))
        {
            char *dirName = conCmdArgs[1] ? conCmdArgs[1] : conCWD;
            DIR *dir = opendir(dirName);
            if(!dir) puts("couldn't open directory");
            else
            {
                struct dirent *de = nullptr;
                while((de = readdir(dir)))
                    printf("%s\n", de->d_name);
                closedir(dir);
            }
        }
        else if(!strcmp(conCmdArgs[0], "ps"))
        {
            int ids[128];
            int procCnt = processListIds(ids, sizeof(ids) / sizeof(*ids));
            printf("%-4s %-32s %-4s %-10s\n", "pid", "name", "thr", "mem(kiB)");
            int threadCount = 0;
            size_t memUsed = 0;
            for(int i = 0; i < procCnt; ++i)
            {
                int pid = ids[i];
                char name[64];
                int threads = -ENOSYS;
                if(processGetName(pid, name, sizeof(name)) < 0) continue;
                if((threads = processGetThreadCount(pid)) < 0) continue;
                size_t mem = processGetUsedMemory(pid);
                if(isError(mem)) continue;
                threadCount += threads;
                memUsed += mem;
                printf("%-4d %-32.32s %-4d %-10zu\n", ids[i], name, threads, mem >> 10);
            }
            printf("Thread count: %d\n", threadCount);
            printf("Userspace memory usage: %zu kiB\n", memUsed >> 10);
        }
        else if(!strcmp(conCmdArgs[0], "sysinfo"))
        {
            struct sysinfo si;
            if(!sysinfo(&si))
            {
                printf("uptime: %ld\n", si.uptime);
                printf("loads: %lu %lu %lu\n", si.loads[0], si.loads[1], si.loads[2]);
                printf("totalram: %lu\n", si.totalram * si.mem_unit);
                printf("freeram: %lu\n", si.freeram * si.mem_unit);
                printf("sharedram: %lu\n", si.sharedram * si.mem_unit);
                printf("bufferram: %lu\n", si.bufferram * si.mem_unit);
                printf("totalswap: %lu\n", si.totalswap * si.mem_unit);
                printf("freeswap: %lu\n", si.freeswap * si.mem_unit);
                printf("procs: %u\n", si.procs);
                printf("totalhigh: %lu\n", si.totalhigh * si.mem_unit);
                printf("freehigh: %lu\n", si.freehigh * si.mem_unit);
            } else puts("sysinfo() failed");
        }
        else if(!strcmp(conCmdArgs[0], "pmap"))
        {
            union
            {
                processMapEntry_t *map;
                uint8_t *mapData;
            };

            int pid = conCmdArgs[1] ? strtol(conCmdArgs[1], nullptr, 0) : getpid();
            static const int bufSize = 16 << 10;
            uint8_t *origData = mapData = new uint8_t[bufSize];
            processGetMap(pid, map, bufSize);
            for(;;)
            {
                printf("%p %p %p %x %s\n", map->Address, map->Size, map->Offset, map->Flags, map->Flags & PROC_MAP_NAME ? map->Name : "<no name>");
                if(map->Flags & PROC_MAP_LAST)
                    break;
                mapData += map->EntrySize;
            }
            delete[] origData;
        }
        else if(!strcmp(conCmdArgs[0], "except"))
        {
            try
            {
                throw("exception test\n");
            }
            catch(const char *msg)
            {
                printf("exceptions work: %s\n", msg);
            }
            catch(...)
            {
                printf("exceptions work\n");
            }
        }
        else
        {
            bool noWait = conCmdBuf[0] == '&';
            char *cmdLine = noWait ? conCmdBuf + 1 : conCmdBuf;
            struct stat st;
            int r = stat(conCmdArgs[0] + (noWait ? 1 : 0), &st);
            int cmdProc = r < 0 ? r : processCreate(cmdLine);
            if(cmdProc < 0)
                printf("unknown command '%s'\n", conCmdArgs[0]);
            else if(!noWait)
            {
                processWait(cmdProc, -1);
                processDelete(cmdProc);
            }
        }
    }

    close(pipeFDs[0]);
    threadDelete(listener);

    if(conCWD) delete[] conCWD;
    if(conCmdArgs) delete[] conCmdArgs;
    if(conOldCmd) delete[] conOldCmd;
    if(conCmdBuf2) delete[] conCmdBuf2;
    if(conCmdBuf) delete[] conCmdBuf;
    if(conWindow) wmDeleteWindow(conWindow);

    wmCleanup();

    if(shutdown) ipcSendMessage(0, MSG_QUIT, MSG_FLAG_NONE, nullptr, 0);

    return 0;
}
