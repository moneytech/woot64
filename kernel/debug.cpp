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

static Mutex DebugStreamLock(true, "DebugStreamLock");

static bool framebufferDisabled = false;

#define USE_VGA_TEXT        0
#define USE_SERIAL          1
#define USE_FRAMEBUFFER     1
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

#if USE_FRAMEBUFFER

#include <fbfont.h>
#include <multiboot.h>
#include <paging.hpp>

extern "C" multiboot_info_t *__multibootInfo;
static multiboot_info_t *multibootInfo;

static uint8_t *fbPixels;
static size_t fbSize;
static uint32_t backColor = 0x000000;
static uint32_t foreColor = 0xFFFFFF;
static const uint fbCharWidth = 8;
static const uint fbCharHeight = FONT_SCANLINES;
static uint fbConsoleWidth = 80;
static uint fbConsoleHeight = 25;
static uint fbX = 0;
static uint fbY = 0;

#if defined(__x86_64__) || defined(__amd64__)
extern "C" void __qmemcpy(void *dst, void *src, size_t n);
extern "C" void __lmemset(void *dst, unsigned val, size_t n);

asm(INLINE_ASM_SYNTAX
"__qmemcpy:\n"
"cld\n"
"mov rcx, rdx\n"
"rep movsq\n"
"ret\n"
NORMAL_ASM_SYNTAX);

asm(INLINE_ASM_SYNTAX
"__lmemset:\n"
"cld\n"
"mov rax, rsi\n"
"mov rcx, rdx\n"
"rep stosd\n"
"ret\n"
NORMAL_ASM_SYNTAX);

#endif // defined(__x86_64__) || defined(__amd64__)

static inline uint32_t fbMakeColor(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t color = 0;
    color |= static_cast<uint32_t>(r) << multibootInfo->framebuffer_red_field_position;
    color |= static_cast<uint32_t>(g) << multibootInfo->framebuffer_green_field_position;
    color |= static_cast<uint32_t>(b) << multibootInfo->framebuffer_blue_field_position;
    return color;
}

static inline void fbSetPixel(uint x, uint y, uint32_t color)
{
    *(reinterpret_cast<uint32_t *>(fbPixels + multibootInfo->framebuffer_pitch * y + 4 * x)) = color;
}

static inline void fbPutChar(uint x, uint y, int chr, uint32_t color)
{
    uint8_t *glyph = fbFont[chr];
    for(uint iy = 0; iy < FONT_SCANLINES; ++iy)
    {
        for(uint ix = 0; ix < 8; ++ix)
        {
            if((*glyph << ix) & 0x80)
                fbSetPixel(x + ix, y + iy, color);
        }
        ++glyph;
    }
}

static inline void fbClearScreen(uint32_t color)
{
    Memory::Set32(fbPixels, color, fbSize / 4);
}

#endif // USE_FRAMEBUFFER

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
    DebugStreamLock.Acquire(5000, false);
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
#if USE_FRAMEBUFFER
        if(!framebufferDisabled)
        {
            uint chrX = fbX * fbCharWidth;
            uint chrY = fbY * fbCharHeight;
            if(c == '\n')
            {
                fbX = 0;
                ++fbY;
            }
            else if(c == '\t')
                fbX = align(fbX + 1, 8);
            else
            {
                fbPutChar(chrX, chrY, c, foreColor);
                ++fbX;
            }
            if(fbX >= fbConsoleWidth)
            {
                fbX = 0;
                ++fbY;
            }
            if(fbY >= fbConsoleHeight)
            {
#if defined(__x86_64__) || defined(__amd64__)
                __qmemcpy(fbPixels, fbPixels + multibootInfo->framebuffer_pitch * fbCharHeight,
                          (multibootInfo->framebuffer_pitch * ((fbConsoleHeight - 1) * fbCharHeight)) / 8);
                __lmemset(fbPixels + multibootInfo->framebuffer_pitch * ((fbConsoleHeight - 1) * fbCharHeight),
                          backColor, multibootInfo->framebuffer_pitch * fbCharHeight / 4);
#else
                Memory::Move(fbPixels, fbPixels + multibootInfo->framebuffer_pitch * fbCharHeight,
                             multibootInfo->framebuffer_pitch * ((fbConsoleHeight - 1) * fbCharHeight));
                Memory::Set32(fbPixels + multibootInfo->framebuffer_pitch * ((fbConsoleHeight - 1) * fbCharHeight),
                              backColor, multibootInfo->framebuffer_pitch * fbCharHeight / 4);
#endif // defined(__x86_64__) || defined(__amd64__)
                fbX = 0;
                --fbY;
            }
        }
#endif // USE_FRAMEBUFFER

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
    DebugStreamLock.Release();
    return n;
}

CallBackStream Debug::DebugStream(debugRead, debugWrite);

void Debug::Initialize()
{
#if USE_VGA_TEXT
    vgaClearScreen();
    vgaSetCursorSize(13, 14);
#endif // USE_VGA_TEXT
#if USE_FRAMEBUFFER
    multibootInfo = (multiboot_info_t *)(KERNEL_BASE + (uintptr_t)__multibootInfo);
    fbPixels = (uint8_t *)(multibootInfo->framebuffer_addr + KERNEL_BASE);
    fbSize = multibootInfo->framebuffer_pitch * multibootInfo->framebuffer_height;
    size_t pageCount = (fbSize + PAGE_SIZE - 1) >> PAGE_SHIFT;
    Paging::ReserveFrames(multibootInfo->framebuffer_addr, pageCount);
    Paging::MapPages(PG_CURRENT_ADDR_SPC, (uintptr_t)fbPixels, multibootInfo->framebuffer_addr, false, true, pageCount);
    backColor = fbMakeColor(24, 32, 64);
    foreColor = fbMakeColor(128, 168, 224);
    fbConsoleWidth = multibootInfo->framebuffer_width / fbCharWidth;
    fbConsoleHeight = multibootInfo->framebuffer_height / fbCharHeight;
    fbClearScreen(backColor);
#endif // USE_FRAMEBUFFER
}

void Debug::DebugFmt(const char *fmt, ...)
{
    VarArgs args;
    VarArgStart(args, fmt);
    DebugStreamLock.Acquire(5000, false);
    DebugStream.VWriteFmt(fmt, args);
    DebugStreamLock.Release();
    VarArgEnd(args);
}

int Debug::DebugIn(void *buffer, size_t bufSize)
{
    DebugStreamLock.Acquire(5000, false);
    int res = DebugStream.ReadLine((char *)buffer, bufSize);
    DebugStreamLock.Release();
    return res;
}

int Debug::DebugRead(void *buffer, size_t count)
{
    DebugStreamLock.Acquire(5000, false);
    int res = DebugStream.Read(buffer, count);
    DebugStreamLock.Release();
    return res;
}

int Debug::DebugWrite(const void *buffer, size_t count)
{
    DebugStreamLock.Acquire(5000, false);
    int res = DebugStream.Write(buffer, count);
    DebugStreamLock.Release();
    return res;
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

void Debug::DisableFramebuffer()
{
    framebufferDisabled = true;
}

void Debug::EnableFramebuffer()
{
    fbX = 0;
    fbY = 0;
    fbClearScreen(backColor);
    framebufferDisabled = false;
}
