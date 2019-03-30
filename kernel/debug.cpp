#include <callbackstream.hpp>
#include <cpu.hpp>
#include <debug.hpp>
#include <memory.hpp>
#include <misc.hpp>
#include <mutex.hpp>
#include <sysdefs.h>
#include <vararg.h>

static int64_t debugRead(void *buffer, int64_t n);
static int64_t debugWrite(const void *buffer, int64_t n);

static CallBackStream DebugStream(debugRead, debugWrite);
static Mutex DebugStreamLock(false, "DebugStreamLock");

#define USE_VGA_TEXT        1
#define USE_SERIAL          1
#define DEBUG_SERIAL_BASE   0x03F8

#if USE_SERIAL
extern "C" void debugSerialOut(int chr)
{
    while(!(_inb(DEBUG_SERIAL_BASE + 5) & 0x20));
    _outb(DEBUG_SERIAL_BASE, chr);
}

extern "C" char debugSerialIn()
{
    while(!(_inb(DEBUG_SERIAL_BASE + 5) & 0x01));
    return _inb(DEBUG_SERIAL_BASE);
}
#endif // USE_SERIAL

#if USE_VGA_TEXT
static uint16_t *const vgaTextMem = (uint16_t *)(KERNEL_BASE + 0xB8000);
static int vgaTextWidth = 80;
static int vgaTextHeight = 25;
static uint vgaCurX = 0;
static uint vgaCurY = 0;
static uint16_t vgaAttrib = 0x1700;

static void vgaSetCursorSize(uint8_t start, uint8_t end)
{
    _outb(0x3D4, 0x0A);
    _outb(0x3D5, (_inb(0x3D5) & 0xC0) | start);
    _outb(0x3D4, 0x0B);
    _outb(0x3D5, (_inb(0x3D5) & 0xE0) | end);
}

static void vgaSetCursorPos(uint16_t pos)
{
    bool cs = cpuDisableInterrupts();
    _outb(0x3D4, 0x0F);
    _outb(0x3D5, pos & 0xFF);
    _outb(0x3D4, 0x0E);
    _outb(0x3D5, pos >> 8);
    cpuRestoreInterrupts(cs);
}

static void vgaClearScreen()
{
    Memory::Set16(vgaTextMem, vgaAttrib, vgaTextWidth * vgaTextHeight);
    vgaCurX = vgaCurY = 0;
    vgaSetCursorPos(0);
}
#endif // USE_VGA_TEXT

int64_t debugRead(void *buffer, int64_t n)
{
#if(USE_SERIAL)
    char *buf = (char *)buffer;
    for(int64_t i = 0; i < n; ++i)
    {
        char c = debugSerialIn();
        buf[i] = c;
        debugWrite(&c, 1);
    }
    return n;
#else
    return 0;
#endif // USE_SERIAL
}

int64_t debugWrite(const void *buffer, int64_t n)
{
    uint8_t *buf = (uint8_t *)buffer;
    for(uint i = 0; i < n; ++i)
    {
        uint8_t c = buf[i];

#if USE_VGA_TEXT
        uint vgaOffs = vgaCurY * vgaTextWidth + vgaCurX;
        if(c == '\n')
        {
            vgaCurX = 0;
            ++vgaCurY;
        }
        else if(c == '\t')
            vgaCurX = align(vgaCurX + 1, 8);
        else
        {
            vgaTextMem[vgaOffs] = vgaAttrib | c;
            ++vgaCurX;
        }
        if(vgaCurX >= vgaTextWidth)
        {
            vgaCurX = 0;
            ++vgaCurY;
        }
        if(vgaCurY >= vgaTextHeight)
        {
            Memory::Move(vgaTextMem, vgaTextMem + vgaTextWidth, sizeof(*vgaTextMem) * vgaTextWidth * (vgaTextHeight - 1));
            Memory::Set16(vgaTextMem + vgaTextWidth * (vgaTextHeight - 1), vgaAttrib, vgaTextWidth);
            vgaCurX = 0;
            --vgaCurY;
        }
        vgaOffs = vgaCurY * vgaTextWidth + vgaCurX;
        vgaSetCursorPos(vgaOffs);
#endif // USE_VGA_TEXT

        if(c == '\n')
        {
#if USE_SERIAL
            debugSerialOut('\r');
#endif // USE_SERIAL
            _outb(0xE9, '\r');
        }
#if USE_SERIAL
        debugSerialOut(c);
#endif // USE_SERIAL
        _outb(0xE9, c);
    }
    return n;
}

void Debug::Initialize()
{
#if USE_VGA_TEXT
    vgaClearScreen();
    vgaSetCursorSize(13, 14);
#endif // USE_VGA_TEXT
}

void Debug::DebugFmt(const char *fmt, ...)
{
    VarArgs args;
    VarArgStart(args, fmt);
    DebugStreamLock.Acquire(1000, false);
    DebugStream.VWriteFmt(fmt, args);
    DebugStreamLock.Release();
    VarArgEnd(args);
}

int Debug::DebugIn(void *buffer, size_t bufSize)
{
    return DebugStream.ReadLine((char *)buffer, bufSize);
}

void Debug::BufferDump(void *ptr, size_t n)
{
    uint8_t *buf = (uint8_t *)ptr;
    for(uint j = 0; j < n; j += 16)
    {
        DEBUG("%.8x : ", j);
        for(uint i = 0; i < 16; ++i)
            DEBUG("%.2x ", buf[i + j]);
        DEBUG("| ");
        for(uint i = 0; i < 16; ++i)
        {
            uint8_t b = buf[i + j];
            DEBUG("%c", b >= ' ' && b < '\x7f' ? b : '.');
        }
        DEBUG("\n");
    }
}
