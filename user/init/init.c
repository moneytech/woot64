#include <stdio.h>

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    printf("[init] Starting %s\n", argv[0]);
    printf("[init] Exiting %s\n", argv[0]);
    return 0;
}
