#include <stdlib.h>
#include <stdint.h>
#include "libc.h"

static void dummy()
{
}

/* atexit.c and __stdio_exit.c override these. the latter is linked
 * as a consequence of linking either __toread.c or __towrite.c. */
weak_alias(dummy, __funcs_on_exit);
weak_alias(dummy, __stdio_exit);
weak_alias(dummy, _fini);

extern weak hidden void (*const __fini_array_start)(void), (*const __fini_array_end)(void);

#ifdef __WOOT__
typedef void (*init_fini_func)(void);
extern init_fini_func *___fini_array_start_;
extern init_fini_func *___fini_array_end_;
#endif // __WOOT__

static void libc_exit_fini(void)
{
#ifdef __WOOT__
    if(___fini_array_start_)
    {
        int fini_count = ___fini_array_end_ - ___fini_array_start_;
        for(int i = fini_count - 1; i >= 0; --i)
            ___fini_array_start_[i]();
    }
#else
	uintptr_t a = (uintptr_t)&__fini_array_end;
	for (; a>(uintptr_t)&__fini_array_start; a-=sizeof(void(*)()))
		(*(void (**)())(a-sizeof(void(*)())))();
#endif // __WOOT__
    _fini();
}

weak_alias(libc_exit_fini, __libc_exit_fini);

_Noreturn void exit(int code)
{
	__funcs_on_exit();
	__libc_exit_fini();
	__stdio_exit();
	_Exit(code);
}
