#include <stdio.h>

#include "libunwind_i.h"

int tdep_get_elf_image(struct elf_image *ei, pid_t pid, unw_word_t ip,
                       unsigned long *segbase, unsigned long *mapoff,
                       char *path, size_t pathlen)
{
    fprintf(stderr, "tdep_get_elf_image(%p, %d, %p, %p, %p, %s, %d)\n",
            ei, pid, ip, *segbase, *mapoff, path, pathlen);
    int rc = elf_map_image(ei, "/bin/usertest");
    return rc;
}

#ifndef UNW_REMOTE_ONLY

void tdep_get_exe_image_path(char *path)
{
    fprintf(stderr, "tdep_get_exe_image_path called\n");
    strcpy(path, "/proc/self/exe");
}

#endif /* !UNW_REMOTE_ONLY */
