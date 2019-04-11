#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syscalls/syscalls.h>
#include <unistd.h>

#define offsetof __builtin_offsetof

int main(int argc, char *argv[])
{
    struct stat st;
    stat(argv[0], &st);
    printf("%zd\n", st.st_size);
    for(int i = 0; i < 3; ++i)
        printf("I like trains. %d\n", i);
    return 9001;
}
