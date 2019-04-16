#include <stdio.h>
#include <woot/process.h>
#include <woot/thread.h>

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    printf("[init] Starting %s\n", argv[0]);

    //int inputProc = processCreate("/bin/inputhandler");
    //processWait(inputProc, 5000); // wait for daemonization of input handler
    int wmProc = processCreate("/bin/windowmanager 1024 768 32");
    processWait(wmProc, 5000); // wait for demonetization of window manager

    int testProc = processCreate("/bin/usertest");
    processWait(testProc, -1);
    processDelete(testProc);

    processWait(wmProc, -1);
    processDelete(wmProc);
    //processWait(inputProc, -1);
    //processDelete(inputProc);

    printf("[init] Exiting %s\n", argv[0]);
    return 0;
}
