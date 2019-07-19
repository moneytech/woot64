#include "libunwind_i.h"

int tdep_get_elf_image (struct elf_image *ei, pid_t pid, unw_word_t ip,
                        unsigned long *segbase, unsigned long *mapoff,
                        char *path, size_t pathlen)
{
    return -1;
}

#ifndef UNW_REMOTE_ONLY

void tdep_get_exe_image_path(char *path)
{
    strcpy(path, "/proc/self/exe");
}

#endif /* !UNW_REMOTE_ONLY */
