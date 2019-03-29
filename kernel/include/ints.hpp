#pragma once

#include <types.h>

class Ints
{
public:
#pragma pack(push, 1)
    struct State
    {
        uint64_t SwitchRSP;
        uint16_t GS;
        uint16_t paddingGS[3];
        uint16_t FS;
        uint16_t paddingFS[3];
        uint16_t ES;
        uint16_t paddingES[3];
        uint16_t DS;
        uint16_t paddingDS[3];
        uint64_t R15;
        uint64_t R14;
        uint64_t R13;
        uint64_t R12;
        uint64_t R11;
        uint64_t R10;
        uint64_t R9;
        uint64_t R8;
        uint64_t RDI;
        uint64_t RSI;
        uint64_t RBP;
        uint64_t RBX;
        uint64_t RDX;
        uint64_t RCX;
        uint64_t RAX;
        uint64_t InterruptNumber;
        uint64_t ErrorCode;
        uint64_t RIP;
        uint16_t CS;
        uint16_t paddingCS[3];
        uint64_t RFLAGS;
        uint64_t RSP;
        uint16_t SS;
        uint16_t paddingSS[3];
    };
#pragma pack(pop)
    typedef bool (*HandlerCallback)(State *state, void *context);
    struct Handler
    {
        Handler *Next; // set to 0
        HandlerCallback Callback;
        void *Context;
    };
private:
    static Handler *Handlers[];
public:
    static void CommonHandler(State *state);
    static void RegisterHandler(uint intNo, Handler *handler);
    static void UnRegisterHandler(uint intNo, Handler *handler);
    static uint HandlerCount(uint intNo);
    static void DumpState(Ints::State *state);
};
