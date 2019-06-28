#include <libgen.h>
#include <stdio.h>
#include <woot/thread.h>

#define PROG_NAME "timekeeper"

extern "C" int main(int argc, char *argv[])
{
    fprintf(stderr, "[" PROG_NAME "] Time keeper started\n");
    threadDaemonize();
    return 0;
}
