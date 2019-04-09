#include <syscalls/syscalls.h>

void _start()
{
    for(int i = 0; i < 3; ++i)
        sys_write(1, "I like trains.\n", 15);
    sys_exit(9001);
}
