#include <misc.hpp>

static int __errno_value = 0;

extern "C" int *__errno_location()
{
    return &__errno_value;
}

uint64_t Misc::PowMax(uint64_t base, uint64_t exp)
{
    decltype(base) result = 1;
    for(;;)
    {
        if(exp & 1) result *= base;
        exp >>= 1;
        if(!exp) break;
        base *= base;
    }
    return result;
}
