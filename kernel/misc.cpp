#include <debug.hpp>
#include <ints.hpp>
#include <misc.hpp>

static int __errno_value = 0;

Ints::Handler Misc::handler = { nullptr, int3Handler, nullptr };

extern "C" int *__errno_location()
{
    return &__errno_value;
}

bool Misc::int3Handler(Ints::State *state, void *context)
{
    (void)context;
    DEBUG("BREAKPOINT! at %p\n", state->RIP);
    Ints::DumpState(state);
    return true;
}

void Misc::InitializeBreakpointInt()
{
    Ints::RegisterHandler(3, &handler);
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
