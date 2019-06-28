#include <stdio.h>
#include <woot/process.h>
#include <woot/thread.h>

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    printf("[init] Starting %s\n", argv[0]);

    int inputProc = processCreate("/bin/inputhandler");
    processWait(inputProc, 2000); // wait for daemonization of input handler
    int wmProc = processCreate("/bin/windowmanager 1024 768 32");
    processWait(wmProc, 2000); // wait for demonetization of window manager
    int timeProc = processCreate("/bin/timekeeper");
    processWait(timeProc, 2000); // wait for timekeeper

    if(inputProc < 0 || wmProc < 0 || timeProc < 0)
    {
        processDelete(timeProc);
        processDelete(wmProc);
        processDelete(inputProc);
        return -1;
    }

    int fmProc = processCreate("/bin/filemanager /bin");
    int conProc = processCreate("/bin/console");
    if(conProc)
    {
        processWait(conProc, -1);
        processDelete(conProc);
    }
    if(fmProc)
    {
        processWait(fmProc, -1);
        processDelete(fmProc);
    }

    processWait(timeProc, 1000);
    processDelete(timeProc);
    processWait(wmProc, 1000);
    processDelete(wmProc);
    processWait(inputProc, 1000);
    processDelete(inputProc);

    printf("[init] Exiting %s\n", argv[0]);
    return 0;
}
