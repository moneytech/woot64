#include <errno.h>
#include <png.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <woot/pixmap.h>

#define swap(type, a, b) { type tmp = a; a = b; b = tmp; }
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#pragma pack(push, 1)
typedef struct BMPImageHeader
{
    uint32_t Size;
    uint32_t Width;
    int32_t Height;
    uint16_t Planes;
    uint16_t BitCount;
    uint32_t Compression;
    uint32_t SizeImage;
    uint32_t XPelsPerMeter;
    uint32_t YPelsPerMeter;
    uint32_t ClrUsed;
    uint32_t ClrImportant;
} BMPImageHeader_t;

typedef struct BMPFileHeader
{
    uint16_t Type;
    uint32_t Size;
    uint16_t Reserved[2];
    uint32_t OffBits;
    BMPImageHeader_t Image;
} BMPFileHeader_t;

typedef struct IconDir
{
    uint16_t Reserved;
    uint16_t Type;
    uint16_t ImageCount;
} IconDir_t;

typedef struct IconDirEntry
{
    uint8_t Width;
    uint8_t Height;
    uint8_t PaletteColors;
    uint8_t Reserved;
    union
    {
        uint16_t ColorPlanes;
        uint16_t HotspotX;
    };
    union
    {
        uint16_t BPP;
        uint16_t HotspotY;
    };
    uint32_t DataSize;
    uint32_t DataOffset;
} IconDirEntry_t;
#pragma pack(pop)

#define BMP_MAGIC 0x4D42

pmColor_t pmColorAliceBlue = { 255, 240, 248, 255 };
pmColor_t pmColorAntiqueWhite = { 255, 250, 235, 215 };
pmColor_t pmColorAqua = { 255, 0, 255, 255 };
pmColor_t pmColorAquamarine = { 255, 127, 255, 212 };
pmColor_t pmColorAzure = { 255, 240, 255, 255 };
pmColor_t pmColorBeige = { 255, 245, 245, 220 };
pmColor_t pmColorBisque = { 255, 255, 228, 196 };
pmColor_t pmColorBlack = { 255, 0, 0, 0 };
pmColor_t pmColorBlanchedAlmond = { 255, 255, 235, 205 };
pmColor_t pmColorBlue = { 255, 0, 0, 255 };
pmColor_t pmColorBlueViolet = { 255, 138, 43, 226 };
pmColor_t pmColorBrown = { 255, 165, 42, 42 };
pmColor_t pmColorBurlyWood = { 255, 222, 184, 135 };
pmColor_t pmColorCadetBlue = { 255, 95, 158, 160 };
pmColor_t pmColorChartReuse = { 255, 127, 255, 0 };
pmColor_t pmColorChocolate = { 255, 210, 105, 30 };
pmColor_t pmColorCoral = { 255, 255, 127, 80 };
pmColor_t pmColorCornflowerBlue = { 255, 100, 149, 237 };
pmColor_t pmColorCornsilk = { 255, 255, 248, 220 };
pmColor_t pmColorCrimson = { 255, 220, 20, 60 };
pmColor_t pmColorCyan = { 255, 0, 255, 255 };
pmColor_t pmColorDarkBlue = { 255, 0, 0, 139 };
pmColor_t pmColorDarkCyan = { 255, 0, 139, 139 };
pmColor_t pmColorDarkGoldenrod = { 255, 184, 134, 11 };
pmColor_t pmColorDarkGray = { 255, 169, 169, 169 };
pmColor_t pmColorDarkGreen = { 255, 0, 100, 0 };
pmColor_t pmColorDarkKhaki = { 255, 189, 183, 107 };
pmColor_t pmColorDarkMagenta = { 255, 139, 0, 139 };
pmColor_t pmColorDarkOliveGreen = { 255, 85, 107, 47 };
pmColor_t pmColorDarkOrange = { 255, 255, 140, 0 };
pmColor_t pmColorDarkOrchid = { 255, 153, 50, 204 };
pmColor_t pmColorDarkRed = { 255, 139, 0, 0 };
pmColor_t pmColorDarkSalmon = { 255, 233, 150, 122 };
pmColor_t pmColorDarkSeaGreen = { 255, 143, 188, 139 };
pmColor_t pmColorDarkSlateBlue = { 255, 72, 61, 139 };
pmColor_t pmColorDarkSlateGray = { 255, 47, 79, 79 };
pmColor_t pmColorDarkTurquoise = { 255, 0, 206, 209 };
pmColor_t pmColorDarkViolet = { 255, 148, 0, 211 };
pmColor_t pmColorDeepPink = { 255, 255, 20, 147 };
pmColor_t pmColorDeepSkyBlue = { 255, 0, 191, 255 };
pmColor_t pmColorDimGray = { 255, 105, 105, 105 };
pmColor_t pmColorDodgerBlue = { 255, 30, 144, 255 };
pmColor_t pmColorFirebrick = { 255, 178, 34, 34 };
pmColor_t pmColorFloralWhite = { 255, 255, 250, 240 };
pmColor_t pmColorForestGreen = { 255, 34, 139, 34 };
pmColor_t pmColorFuchsia = { 255, 255, 0, 255 };
pmColor_t pmColorGainsboro = { 255, 220, 220, 220 };
pmColor_t pmColorGhostWhite = { 255, 248, 248, 255 };
pmColor_t pmColorGold = { 255, 255, 215, 0 };
pmColor_t pmColorGoldenrod = { 255, 218, 165, 32 };
pmColor_t pmColorGray = { 255, 128, 128, 128 };
pmColor_t pmColorGreen = { 255, 0, 128, 0 };
pmColor_t pmColorGreenYellow = { 255, 173, 255, 47 };
pmColor_t pmColorHoneydew = { 255, 240, 255, 240 };
pmColor_t pmColorHotPink = { 255, 255, 105, 180 };
pmColor_t pmColorIndianRed = { 255, 205, 92, 92 };
pmColor_t pmColorIndigo = { 255, 75, 0, 130 };
pmColor_t pmColorIvory = { 255, 255, 255, 240 };
pmColor_t pmColorKhaki = { 255, 240, 230, 140 };
pmColor_t pmColorLavender = { 255, 230, 230, 250 };
pmColor_t pmColorLavenderBlush = { 255, 255, 240, 245 };
pmColor_t pmColorLawnGreen = { 255, 124, 252, 0 };
pmColor_t pmColorLemonChiffon = { 255, 255, 250, 205 };
pmColor_t pmColorLightBlue = { 255, 173, 216, 230 };
pmColor_t pmColorLightCoral = { 255, 240, 128, 128 };
pmColor_t pmColorLightCyan = { 255, 224, 255, 255 };
pmColor_t pmColorLightGoldenrodYellow = { 255, 250, 250, 210 };
pmColor_t pmColorLightGray = { 255, 211, 211, 211 };
pmColor_t pmColorLightGreen = { 255, 144, 238, 144 };
pmColor_t pmColorLightPink = { 255, 255, 182, 193 };
pmColor_t pmColorLightSalmon = { 255, 255, 160, 122 };
pmColor_t pmColorLightSeaGreen = { 255, 32, 178, 170 };
pmColor_t pmColorLightSkyBlue = { 255, 135, 206, 250 };
pmColor_t pmColorLightSlateGray = { 255, 119, 136, 153 };
pmColor_t pmColorLightSteelBlue = { 255, 176, 196, 222 };
pmColor_t pmColorLightYellow = { 255, 255, 255, 224 };
pmColor_t pmColorLime = { 255, 0, 255, 0 };
pmColor_t pmColorLimeGreen = { 255, 50, 205, 50 };
pmColor_t pmColorLinen = { 255, 250, 240, 230 };
pmColor_t pmColorMagenta = { 255, 255, 0, 255 };
pmColor_t pmColorMaroon = { 255, 128, 0, 0 };
pmColor_t pmColorMediumAquamarine = { 255, 102, 205, 170 };
pmColor_t pmColorMediumBlue = { 255, 0, 0, 205 };
pmColor_t pmColorMediumOrchid = { 255, 186, 85, 211 };
pmColor_t pmColorMediumPurple = { 255, 147, 112, 219 };
pmColor_t pmColorMediumSeaGreen = { 255, 60, 179, 113 };
pmColor_t pmColorMediumSlateBlue = { 255, 123, 104, 238 };
pmColor_t pmColorMediumSpringGreen = { 255, 0, 250, 154 };
pmColor_t pmColorMediumTurquioise = { 255, 72, 209, 204 };
pmColor_t pmColorMediumVioletRed = { 255, 199, 21, 133 };
pmColor_t pmColorMidnightBlue = { 255, 25, 25, 112 };
pmColor_t pmColorMintCream = { 255, 245, 255, 250 };
pmColor_t pmColorMistyRose = { 255, 255, 228, 225 };
pmColor_t pmColorMoccasin = { 255, 255, 228, 181 };
pmColor_t pmColorNovajoWhite = { 255, 255, 222, 173 };
pmColor_t pmColorNavy = { 255, 0, 0, 128 };
pmColor_t pmColorOldLace = { 255, 253, 245, 230 };
pmColor_t pmColorOlive = { 255, 128, 128, 0 };
pmColor_t pmColorOliveDrab = { 255, 107, 142, 35 };
pmColor_t pmColorOrange = { 255, 255, 165, 0 };
pmColor_t pmColorOrangeRed = { 255, 255, 69, 0 };
pmColor_t pmColorOrchid = { 255, 218, 112, 214 };
pmColor_t pmColorPaleGoldenrod = { 255, 238, 232, 170 };
pmColor_t pmColorPaleGreen = { 255, 152, 251, 152 };
pmColor_t pmColorPaleTurquoise = { 255, 175, 238, 238 };
pmColor_t pmColorPaleVioletRed = { 255, 219, 112, 147 };
pmColor_t pmColorPapayaWhip = { 255, 255, 239, 213 };
pmColor_t pmColorPeachPuff = { 255, 255, 218, 185 };
pmColor_t pmColorPeru = { 255, 205, 133, 63 };
pmColor_t pmColorPink = { 255, 255, 192, 203 };
pmColor_t pmColorPlum = { 255, 221, 160, 221 };
pmColor_t pmColorPowderBlue = { 255, 176, 224, 230 };
pmColor_t pmColorPurple = { 255, 128, 0, 128 };
pmColor_t pmColorRed = { 255, 255, 0, 0 };
pmColor_t pmColorRosyBrown = { 255, 188, 143, 143 };
pmColor_t pmColorRoyalBlue = { 255, 65, 105, 225 };
pmColor_t pmColorSaddleBrown = { 255, 139, 69, 19 };
pmColor_t pmColorSalmon = { 255, 250, 128, 114 };
pmColor_t pmColorSandyBrown = { 255, 244, 164, 96 };
pmColor_t pmColorSeaGreen = { 255, 46, 139, 87 };
pmColor_t pmColorSeaShell = { 255, 255, 245, 238 };
pmColor_t pmColorSienna = { 255, 160, 82, 45 };
pmColor_t pmColorSilver = { 255, 192, 192, 192 };
pmColor_t pmColorSkyBlue = { 255, 135, 206, 235 };
pmColor_t pmColorSlateBlue = { 255, 106, 90, 205 };
pmColor_t pmColorSlateGray = { 255, 112, 128, 144 };
pmColor_t pmColorSnow = { 255, 255, 250, 250 };
pmColor_t pmColorSpringGreen = { 255, 0, 255, 127 };
pmColor_t pmColorSteelBlue = { 255, 70, 130, 180 };
pmColor_t pmColorTan = { 255, 210, 180, 140 };
pmColor_t pmColorTeal = { 255, 0, 128, 128 };
pmColor_t pmColorThistle = { 255, 216, 191, 216 };
pmColor_t pmColorTomato = { 255, 255, 99, 71 };
pmColor_t pmColorTurquoise = { 255, 64, 224, 208 };
pmColor_t pmColorViolet = { 255, 238, 130, 238 };
pmColor_t pmColorWheat = { 255, 245, 222, 179 };
pmColor_t pmColorWhite = { 255, 255, 255, 255 };
pmColor_t pmColorWhiteSmoke = { 255, 245, 245, 245 };
pmColor_t pmColorYellow = { 255, 255, 255, 0 };
pmColor_t pmColorYellowGreen = { 255, 154, 205, 50 };

pmColor_t pmColorTransparent = { 0x00, 0x00, 0x00, 0x00 };

pmPixelFormat_t pmFormatInvalid = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
pmPixelFormat_t pmFormatA8R8G8B8 = { 32, 8, 8, 8, 8, 24, 16, 8, 0 };
pmPixelFormat_t pmFormatA0R8B8G8 = { 32, 0, 8, 8, 8, 0, 16, 8, 0 };

static void wmemset(void *ptr, unsigned value, unsigned long num);
static void lmemset(void *ptr, unsigned value, unsigned long num);
static void *bltmove(void *dst, const void *src, size_t bpl, size_t dstride, size_t sstride, size_t lines);
static int mapValue(int imin, int imax, int omin, int omax, int val);
static unsigned isDepthSupported(unsigned depth);

#ifdef __i386__
// we have asm versions of wmemset and lmemset for x86
asm(
"wmemset:"
"pushl %edi\n"
"movl 8(%esp), %edi\n"
"movl 12(%esp), %eax\n"
"movl 16(%esp), %ecx\n"
"rep stosw\n"
"popl %edi\n"
"ret\n");

asm(
"lmemset:"
"pushl %edi\n"
"movl 8(%esp), %edi\n"
"movl 12(%esp), %eax\n"
"movl 16(%esp), %ecx\n"
"rep stosl\n"
"popl %edi\n"
"ret\n");
#elif __x86_64__ || __amd64__
// asm memory ops for x86-64
asm(
"wmemset:\n"
"mov %rsi, %rax\n"
"mov %rdx, %rcx\n"
"rep stosw\n"
"ret\n");

asm(
"lmemset:\n"
"mov %rsi, %rax\n"
"mov %rdx, %rcx\n"
"rep stosl\n"
"ret\n");
#else
void wmemset(void *ptr, unsigned value, unsigned long num)
{
    unsigned short *dst = (unsigned short *)ptr;
    while(num--) *dst++ = value;
}

void lmemset(void *ptr, unsigned value, unsigned long num)
{
    unsigned *dst = (unsigned *)ptr;
    while(num--) *dst++ = value;
}
#endif

void *bltmove(void *dst, const void *src, size_t bpl, size_t dstride, size_t sstride, size_t lines)
{
    unsigned char *d = (unsigned char *)dst;
    unsigned char *s = (unsigned char *)src;
    int fwd = d < s;

    if(fwd)
    {
        while(lines--)
        {
            memmove(d, s, bpl);
            d += dstride;
            s += sstride;
        }
    }
    else
    {
        d += dstride * lines;
        s += sstride * lines;
        while(lines--)
        {
            d -= dstride;
            s -= sstride;
            memmove(d, s, bpl);
        }
    }
    return dst;
}

int mapValue(int imin, int imax, int omin, int omax, int val)
{
    return (float)(val - imin) / (imax - imin) * (omax - omin) + omin;
}

unsigned isDepthSupported(unsigned depth)
{
    return depth == 8 || depth == 15 || depth == 16 || depth == 24 || depth == 32;
}

int pmFormatEqual(pmPixelFormat_t a, pmPixelFormat_t b)
{
    return pmFormatEqualP(&a, &b);
}


int pmFormatEqualP(pmPixelFormat_t *a, pmPixelFormat_t *b)
{
    return a->BPP == b->BPP &&
            a->AlphaShift == b->AlphaShift &&
            a->RedShift == b->RedShift &&
            a->GreenShift == b->GreenShift &&
            a->BlueShift == b->BlueShift &&
            a->AlphaBits == b->AlphaBits &&
            a->RedBits == b->RedBits &&
            a->GreenBits == b->GreenBits &&
            a->BlueBits == b->BlueBits;
}

unsigned pmFormatPixelsToBytes(pmPixelFormat_t format, unsigned pixels)
{
    return pmFormatPixelsToBytesP(&format, pixels);
}

unsigned pmFormatPixelsToBytesP(pmPixelFormat_t *format, unsigned pixels)
{
    return (pixels * format->BPP + 7) / 8;
}

pmPixelFormat_t pmFormatFromModeInfo(vidModeInfo_t *modeInfo)
{
    pmPixelFormat_t pf;
    pf.BPP = modeInfo->BitsPerPixel;
    pf.AlphaBits = modeInfo->AlphaBits;
    pf.RedBits = modeInfo->RedBits;
    pf.GreenBits = modeInfo->GreenBits;
    pf.BlueBits = modeInfo->BlueBits;
    pf.AlphaShift = modeInfo->AlphaShift;
    pf.RedShift = modeInfo->RedShift;
    pf.GreenShift = modeInfo->GreenShift;
    pf.BlueShift = modeInfo->BlueShift;
    return pf;
}

unsigned char pmColorGetLuma(pmColor_t color)
{
    return ((unsigned)color.R + (unsigned)color.R + (unsigned)color.G + (unsigned)color.G + (unsigned)color.G + (unsigned)color.B) / 6;
}

pmColor_t pmColorFromRGB(unsigned char r, unsigned char g, unsigned char b)
{
    pmColor_t color;
    color.A = 255;
    color.R = r;
    color.G = g;
    color.B = b;
    return color;
}

pmColor_t pmColorFromARGB(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
{
    pmColor_t color;
    color.A = a;
    color.R = r;
    color.G = g;
    color.B = b;
    return color;
}

unsigned pmColorToValue(pmPixelFormat_t *format, pmColor_t color)
{
    unsigned int a = color.A >> (8 - format->AlphaBits);
    unsigned int r = color.R >> (8 - format->RedBits);
    unsigned int g = color.G >> (8 - format->GreenBits);
    unsigned int b = color.B >> (8 - format->BlueBits);
    return a << format->AlphaShift | r << format->RedShift | g << format->GreenShift | b << format->BlueShift;
}

pmColor_t pmColorFromValue(pmPixelFormat_t *format, unsigned value)
{
    unsigned int A = ((value >> format->AlphaShift) << (8 - format->AlphaBits)) & 0xFF;
    unsigned int R = ((value >> format->RedShift) << (8 - format->RedBits)) & 0xFF;
    unsigned int G = ((value >> format->GreenShift) << (8 - format->GreenBits)) & 0xFF;
    unsigned int B = ((value >> format->BlueShift) << (8 - format->BlueBits)) & 0xFF;
    return pmColorFromARGB(A, R, G, B);
}

pmColor_t pmColorFromIndex(pmPixMap_t *pixMap, unsigned index)
{
    if(!pixMap || !pixMap->Palette || index >= (1 << pixMap->Format.BPP))
        return pmColorBlack;
    return pixMap->Palette[index];
}

unsigned pmIndexFromColor(pmPixMap_t *pixMap, pmColor_t color)
{
    if(!pixMap || !pixMap->Palette)
        return 0;
    unsigned r = color.R;
    unsigned g = color.G;
    unsigned b = color.B;
    unsigned nColors = 1 << pixMap->Format.BPP;
    unsigned mine = ~0;
    unsigned mini = 0;
    for(int i = 0; i < nColors; ++i)
    {
        int dr = pixMap->Palette[i].R - r;
        int dg = pixMap->Palette[i].G - g;
        int db = pixMap->Palette[i].B - b;
        unsigned e = dr * dr + dg * dg + db * db;
        if(e < mine)
        {
            mine = e;
            mini = i;
        }
    }
    return mini;
}

void pmColorRGB2HSL(pmColorHSL_t *dst, pmColor_t src)
{
    if(!dst) return;
    dst->A = src.A / 255.0f;
    float r = src.R / 255.0f;
    float g = src.G / 255.0f;
    float b = src.B / 255.0f;
    //float v = max(r, g); v = max(v, b);
    float v;
    int maxV;
    if(r > g)
    {
        v = r;
        maxV = 0;
    }
    else
    {
        v = g;
        maxV = 1;
    }
    if(b > v)
    {
        v = b;
        maxV = 2;
    }
    //float m = min(r, g); m = min(m, b);
    float m;
    int minM;
    if(r < g)
    {
        m = r;
        minM = 0;
    }
    else
    {
        m = g;
        minM = 1;
    }
    if(b < m)
    {
        m = b;
        minM = 2;
    }
    dst->H = 0;
    dst->S = 0;
    dst->L = 0.5f * (m + v);
    if(dst->L <= 0) return;
    float vm = v - m;
    dst->S = vm;
    if(dst->S <= 0) return;
    dst->S /= dst->L < 0.5f ? v + m : 2.0f - v - m;
    float ivm = 1.0f / vm;
    float R = ivm * (v - r);
    float G = ivm * (v - g);
    float B = ivm * (v - b);
    switch(maxV)
    {
    case 0:
        dst->H = minM == 1 ? 5.0f + B : 1.0f - G;
        break;
    case 1:
        dst->H = minM == 2 ? 1.0f + R : 3.0f - B;
        break;
    default:
        dst->H = minM == 0 ? 3.0f + G : 5.0f - R;
        break;
    }
    dst->H *= 1.0f / 6.0f;
}

pmColor_t pmColorHSL2RGB(pmColorHSL_t *src)
{
    float h = src->H;
    float s = src->S;
    float l = src->L;
    float a = src->A * 255.0f;
    float r = src->L;
    float g = src->L;
    float b = src->L;

    float v = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
    if(v > 0)
    {
        float m = src->L + src->L - v;
        float sv = (v - m) / v;
        h *= 6.0f;
        int sextant = (int)h;
        float fract = h - sextant;
        float vsf = v * sv * fract;
        float mid1 = m + vsf;
        float mid2 = v - vsf;
        switch(sextant)
        {
        case 0:
            r = v;
            g = mid1;
            b = m;
            break;
        case 1:
            r = mid2;
            g = v;
            b = m;
            break;
        case 2:
            r = m;
            g = v;
            b = mid1;
            break;
        case 3:
            r = m;
            g = mid2;
            b = v;
            break;
        case 4:
            r = mid1;
            g = m;
            b = v;
            break;
        case 5:
            r = v;
            g = m;
            b = mid2;
            break;
        }
    }
    return pmColorFromARGB(a, r * 255.0f, g * 255.0f, b * 255.0f);
}

pmColor_t pmColorBrighten(pmColor_t color, float amount)
{
    pmColorHSL_t hsl;
    pmColorRGB2HSL(&hsl, color);
    hsl.L = max(0.0f, min(1.0f, hsl.L + amount));
    return pmColorHSL2RGB(&hsl);
}

pmColor_t pmColorInvert(pmColor_t color)
{
    pmColor_t res = { color.A, ~color.R, ~color.G, ~color.B };
    return res;
}

pmColor_t *pmPaletteCreate(pmPixelFormat_t *format)
{
    if(format->BPP > 8) return NULL;
    int nColors = 1 << format->BPP;
    pmColor_t *palette = (pmColor_t *)malloc(sizeof(pmColor_t) * nColors);
    for(int i = 0; i < nColors; ++i)
    {
        int gray = mapValue(0, nColors, 0, 255, i);
        palette[i] = pmColorFromRGB(gray, gray, gray);
    }
    return palette;
}

void pmPaletteDelete(pmColor_t *palette)
{
    if(palette) free(palette);
}

pmPixMap_t *pmCreate(int width, int height, pmPixelFormat_t *format)
{
    if(width < 1 || height < 1 || !isDepthSupported(format->BPP))
        return NULL;
    pmPixMap_t *pixMap = (struct pmPixMap *)calloc(1, sizeof(struct pmPixMap));
    if(!pixMap) return NULL;
    pixMap->Contents.Width = width;
    pixMap->Contents.Height = height;
    pixMap->Pitch = pmFormatPixelsToBytesP(format, width);
    pixMap->Format = *format;
    if(format->BPP <= 8)
    {
        pixMap->Palette = pmPaletteCreate(format);
        if(!pixMap->Palette)
        {
            pmDelete(pixMap);
            return NULL;
        }
    }
    pixMap->Pixels = calloc(height, pixMap->Pitch);
    pixMap->ReleasePixels = 1;
    if(!pixMap->Pixels)
    {
        pmDelete(pixMap);
        return NULL;
    }
    return pixMap;
}

void pmDelete(pmPixMap_t *pixMap)
{
    if(!pixMap) return;
    if(pixMap->ReleasePixels && pixMap->Pixels)
        free(pixMap->Pixels);
    if(pixMap->Palette)
        pmPaletteDelete(pixMap->Palette);
    free(pixMap);
}

void pmInvalidate(pmPixMap_t *pixMap, int x, int y, int w, int h)
{
    rcRectangle_t rect = { x, y, w, h };
    pmInvalidateRect(pixMap, rect);
}

void pmInvalidateRect(pmPixMap_t *pixMap, rcRectangle_t rect)
{
    if(!pixMap) return;
    if(pixMap->Parent)
    {
        rect.X += pixMap->Parent->Contents.X;
        rect.Y += pixMap->Parent->Contents.Y;
        pmInvalidateRect(pixMap->Parent, rect);
        return;
    }
    pixMap->Dirty = rcAdd(pixMap->Dirty, rect);
}

void pmInvalidateRectP(pmPixMap_t *pixMap, rcRectangle_t *rect)
{
    if(!pixMap) return;
    rcRectangle_t r = *rect;
    if(pixMap->Parent)
    {
        r.X += pixMap->Parent->Contents.X;
        r.Y += pixMap->Parent->Contents.Y;
        pmInvalidateRectP(pixMap->Parent, &r);
        return;
    }
    pixMap->Dirty = rcAdd(pixMap->Dirty, *rect);
}

void pmInvalidateWhole(pmPixMap_t *pixMap)
{
    pmInvalidateRectP(pixMap, &pixMap->Contents);
}

rcRectangle_t pmGetDirtyRectangle(pmPixMap_t *pixMap)
{
    if(!pixMap) return rcRectangleEmpty;
    if(pixMap->Parent)
    {
        rcRectangle_t r = pmGetDirtyRectangle(pixMap->Parent);
        r = rcIntersectP(&pixMap->Contents, &r);
        r.X -= pixMap->Contents.X;
        r.Y -= pixMap->Contents.Y;
        return r;
    }
    return pixMap->Dirty;
}

rcRectangle_t pmGetAndClearDirtyRectangle(pmPixMap_t *pixMap)
{
    rcRectangle_t dirty = pmGetDirtyRectangle(pixMap);
    pmClearParentDirty(pixMap);
    return dirty;
}

void pmClearDirty(pmPixMap_t *pixMap)
{
    if(!pixMap) return;
    pixMap->Dirty = rcRectangleEmpty;
}

void pmClearParentDirty(pmPixMap_t *pixMap)
{
    if(!pixMap) return;
    if(pixMap->Parent)
        pmClearParentDirty(pixMap->Parent);
    pixMap->Dirty = rcRectangleEmpty;
}

pmPixMap_t *pmFromMemory(int width, int height, int pitch, pmPixelFormat_t *format, void *pixels, unsigned releasePixels)
{
    if(width < 1 || height < 1 || !isDepthSupported(format->BPP))
        return NULL;
    struct pmPixMap *pixMap = (struct pmPixMap *)calloc(1, sizeof(struct pmPixMap));
    if(!pixMap) return NULL;
    pixMap->Contents.Width = width;
    pixMap->Contents.Height = height;
    pixMap->Pitch = pitch;
    pixMap->Format = *format;
    if(format->BPP <= 8)
    {
        pixMap->Palette = pmPaletteCreate(format);
        if(!pixMap->Palette)
        {
            pmDelete(pixMap);
            return NULL;
        }
    }
    pixMap->Pixels = pixels;
    pixMap->ReleasePixels = releasePixels;
    return pixMap;
}

pmPixMap_t *pmFromPixMap(pmPixMap_t *src, pmPixelFormat_t *format)
{
    pmPixMap_t *pixMap = pmCreate(src->Contents.Width, src->Contents.Height, format);
    if(!pixMap) return NULL;
    pmBlit(pixMap, src, 0, 0, 0, 0, pixMap->Contents.Width, pixMap->Contents.Height);
    return pixMap;
}

pmPixMap_t *pmSubPixMap(pmPixMap_t *src, int x, int y, int w, int h)
{
    rcRectangle_t srcRect = { 0, 0, src->Contents.Width, src->Contents.Height };
    rcRectangle_t newRect = { x, y, w, h };
    newRect = rcIntersectP(&srcRect, &newRect);
    if(newRect.Width <= 0 || newRect.Height <= 0)
        return NULL;
    pmPixMap_t *pm = pmFromMemory(newRect.Width, newRect.Height,
                                  src->Pitch, &src->Format,
                                  src->Pixels + newRect.Y * src->Pitch +
                                  pmFormatPixelsToBytes(src->Format, newRect.X), 0);
    pm->Parent = src;
    pm->Contents.X = x;
    pm->Contents.Y = y;
    return pm;
}

pmPixMap_t *pmLoadPNG(const char *filename)
{
    FILE *f = fopen(filename, "rb");
    if(!f) return NULL;
    png_byte header[8];
    if(fread(header, sizeof(header), 1, f) != 1 || png_sig_cmp(header, 0, sizeof(header)))
    {
        fclose(f);
        return NULL;
    }
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr)
    {
        fclose(f);
        return NULL;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(f);
        return NULL;
    }
    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, sizeof(header));
    png_read_info(png_ptr, info_ptr);

    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_read_update_info(png_ptr, info_ptr);

    // only 24 and 32 bit pngs are supported for now
    if(bit_depth != 8 || (color_type != PNG_COLOR_TYPE_RGB && color_type != PNG_COLOR_TYPE_RGBA))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        fclose(f);
        return NULL;
    }

    png_bytep *row_pointers = (png_bytep *)calloc(height, sizeof(png_bytep));
    if(!row_pointers)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        fclose(f);
        return NULL;
    }

    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    void *pixels = calloc(height, row_bytes);

    if(!pixels)
    {
        free(row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        fclose(f);
        return NULL;
    }

    int a = color_type == PNG_COLOR_TYPE_RGBA;
    pmPixelFormat_t pf = { 32, (a ? 8 : 0), 8, 8, 8, (a ? 24 : 0), 0, 8, 16 };
    pmPixMap_t *pm = pmFromMemory(width, height, row_bytes, &pf, pixels, 1);
    if(!pm)
    {
        free(pixels);
        free(row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        fclose(f);
        return NULL;
    }

    for(png_uint_32 y = 0; y < height; ++y)
        row_pointers[y] = pm->Pixels + pm->Pitch * y;

    png_read_image(png_ptr, row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    fclose(f);

    free(row_pointers);

    return pm;
}

pmPixMap_t *pmLoadCUR(const char *filename, unsigned idx, int *hotX, int *hotY)
{
    if(!filename) return NULL;
    FILE *f = fopen(filename, "rb");
    if(!f) return NULL;
    IconDir_t id;
    if(fread(&id, sizeof(id), 1, f) != 1)
    {
        fclose(f);
        return NULL;
    }

    if(id.Reserved != 0 || id.Type != 2 || id.ImageCount < 1 || idx >= id.ImageCount)
    {
        fclose(f);
        return NULL;
    }

    IconDirEntry_t ide;
    for(int i = 0; i <= idx; ++i)
    {
        if(fread(&ide, sizeof(ide), 1, f) != 1)
        {
            fclose(f);
            return NULL;
        }
    }

    int w = ide.Width ? ide.Width : 256;
    int h = ide.Height ? ide.Height : 256;
    if(ide.PaletteColors != 0)
    {   // palettized images not supported
        fclose(f);
        return NULL;
    }
    if(hotX) *hotX = ide.HotspotX;
    if(hotY) *hotY = ide.HotspotY;

    if(fseek(f, ide.DataOffset, SEEK_SET))
    {
        fclose(f);
        return NULL;
    }
    BMPImageHeader_t bi;
    if(fread(&bi, sizeof(bi), 1, f) != 1)
    {
        fclose(f);
        return NULL;
    }

    void *pixels = calloc(1, ide.DataSize);
    if(!pixels)
    {
        fclose(f);
        return NULL;
    }

    if(fread(pixels, ide.DataSize - sizeof(bi), 1, f) != 1)
    {
        free(pixels);
        fclose(f);
        return NULL;
    }
    fclose(f);
    pmPixMap_t *pm = pmFromMemory(w, h, 4 * w, &pmFormatA8R8G8B8, pixels, 1);
    if(bi.Height > 0)
        pmVFlip(pm);
    return pm;
}

void pmSetPaletteEntry(pmPixMap_t *pixMap, unsigned idx, pmColor_t color)
{
    if(!pixMap || !pixMap->Palette || idx >= (1 << pixMap->Format.BPP))
        return;
    pixMap->Palette[idx] = color;
}

rcRectangle_t pmGetRectangle(pmPixMap_t *pixMap)
{
    if(!pixMap) return rcRectangleEmpty;
    return pixMap->Contents;
}

unsigned pmGetPitch(pmPixMap_t *pixMap)
{
    if(!pixMap) return -EINVAL;
    return pixMap->Pitch;
}

void *pmGetPixels(pmPixMap_t *pixMap)
{
    if(!pixMap) return NULL;
    return pixMap->Pixels;
}

void pmSetPixel(pmPixMap_t *pixMap, int x, int y, pmColor_t color)
{
    if(x < 0 || x >= pixMap->Contents.Width || y < 0 || y >= pixMap->Contents.Height)
        return;
    if(pixMap->Format.BPP == 8)
    {
        unsigned char *pixel = (unsigned char *)(pixMap->Pixels + pixMap->Pitch * y + x);
        *pixel = pmIndexFromColor(pixMap, color);
    }
    unsigned int col = pmColorToValue(&pixMap->Format, color);
    if(pixMap->Format.BPP == 32)
    {
        unsigned int *pixel = (unsigned int *)(pixMap->Pixels + pixMap->Pitch * y + x * 4);
        *pixel = col;
    }
    else if(pixMap->Format.BPP == 24)
    {
        unsigned int *pixel = (unsigned int *)(pixMap->Pixels + pixMap->Pitch * y + x * 3);
        *pixel = (*pixel & 0xFF000000) | col;
    }
    else if(pixMap->Format.BPP == 16 || pixMap->Format.BPP == 15)
    {
        unsigned short *pixel = (unsigned short *)(pixMap->Pixels + pixMap->Pitch * y + x * 2);
        *pixel = col;
    }
}

pmColor_t pmGetPixel(pmPixMap_t *pixMap, int x, int y)
{
    x = x < 0 ? pixMap->Contents.Width - (-x % pixMap->Contents.Width) : x % pixMap->Contents.Width;
    y = y < 0 ? pixMap->Contents.Height - (-y % pixMap->Contents.Height) : y % pixMap->Contents.Height;

    if(pixMap->Format.BPP == 32)
    {
        unsigned int *pixel = (unsigned int *)(pixMap->Pixels + pixMap->Pitch * y + x * 4);
        return pmColorFromValue(&pixMap->Format, *pixel);
    }
    else if(pixMap->Format.BPP == 24)
    {
        unsigned int *pixel = (unsigned int *)(pixMap->Pixels + pixMap->Pitch * y + x * 3);
        return pmColorFromValue(&pixMap->Format, *pixel & 0xFFFFFF);
    }
    else if(pixMap->Format.BPP == 16 || pixMap->Format.BPP == 15)
    {
        unsigned short *pixel = (unsigned short *)(pixMap->Pixels + pixMap->Pitch * y + x * 2);
        return pmColorFromValue(&pixMap->Format, *pixel);
    }
    else if(pixMap->Format.BPP == 8)
    {
        unsigned char *pixel = (unsigned char *)(pixMap->Pixels + pixMap->Pitch * y + x);
        return pixMap->Palette[*pixel];
    }
    return pmColorBlack;
}

pmColor_t pmBlendPixel(pmColor_t a, pmColor_t b)
{
    unsigned int A = b.A + 1;
    unsigned int iA = 256 - b.A;
    return pmColorFromARGB(a.A, (A * b.R + iA * a.R) >> 8, (A * b.G + iA * a.G) >> 8, (A * b.B + iA * a.B) >> 8);
}

void pmHLine(pmPixMap_t *pixMap, int x1, int y, int x2, pmColor_t c)
{
    if(x1 > x2) swap(int, x1, x2);
    if(y < 0 || y >= pixMap->Contents.Height || (x1 < 0 && x2 < 0) || (x1 >= pixMap->Contents.Width && x2 >= pixMap->Contents.Width))
        return;
    x1 = max(0, x1);
    x2 = min(pixMap->Contents.Width - 1, x2);

    unsigned col = pmColorToValue(&pixMap->Format, c);
    int w = x2 - x1 + 1;

    if(pixMap->Format.BPP == 32)
    {
        unsigned *line = (unsigned *)(pixMap->Pixels + y * pixMap->Pitch);
        lmemset(line + x1, col, w);
    }
    else if(pixMap->Format.BPP == 24)
    {
        for(int X = x1; X < x2; ++X)
        {
            unsigned *pixel = (unsigned *)(pixMap->Pixels + pixMap->Pitch * y + X * 3);
            *pixel = (*pixel & 0xFF000000) | col;
        }
    }
    else if(pixMap->Format.BPP == 16 || pixMap->Format.BPP == 15)
    {
        unsigned short *line = (unsigned short *)(pixMap->Pixels + y * pixMap->Pitch);
        wmemset(line + x1, col, w);
    }
}

void pmHLinePattern(pmPixMap_t *pixMap, int x1, int y, int x2, unsigned pattern, pmColor_t c)
{
    unsigned i = ~0;
    if(x1 > x2) swap(int, x1, x2);
    for(int x = x1; x < x2; ++x, --i)
    {
        if((pattern >> (i & 31)) & 1)
            pmSetPixel(pixMap, x, y, c);
    }
}

void pmVLine(pmPixMap_t *pixMap, int x, int y1, int y2, pmColor_t c)
{
    // just naive and slow implementation using SetPixel()
    if(y1 > y2) swap(int, y1, y2);
    if(x < 0 || x >= pixMap->Contents.Width || (y1 < 0 && y2 < 0) || (y1 >= pixMap->Contents.Height && y2 >= pixMap->Contents.Height))
        return;
    for(int Y = y1; Y <= y2; ++Y)
        pmSetPixel(pixMap, x, Y, c);
}

void pmVLinePattern(pmPixMap_t *pixMap, int x, int y1, int y2, unsigned pattern, pmColor_t c)
{
    unsigned i = ~0;
    if(y1 > y2) swap(int, y1, y2);
    for(int y = y1; y < y2; ++y, --i)
    {
        if((pattern >> (i & 31)) & 1)
            pmSetPixel(pixMap, x, y, c);
    }
}

void pmVFlip(pmPixMap_t *pixMap)
{
    void *lineBuf = malloc(pixMap->Pitch);
    if(!lineBuf) return;
    for(int y = 0; y < pixMap->Contents.Height / 2; ++y)
    {
        memcpy(lineBuf, pixMap->Pixels + pixMap->Pitch * (pixMap->Contents.Height - (y + 1)), pixMap->Pitch);
        memcpy(pixMap->Pixels + pixMap->Pitch * (pixMap->Contents.Height - (y + 1)), pixMap->Pixels + pixMap->Pitch * y, pixMap->Pitch);
        memcpy(pixMap->Pixels + pixMap->Pitch * y, lineBuf, pixMap->Pitch);
    }
    free(lineBuf);
}

void pmLine(pmPixMap_t *pixMap, int x1, int y1, int x2, int y2, pmColor_t c)
{
    int dx = x2 - x1, sx = dx < 0 ? -1 : 1;
    int dy = y2 - y1, sy = dy < 0 ? -1 : 1;
    dx *= sx; dy *= sy;

    int err = (dx > dy ? dx : -dy) >> 1;

    for(;;)
    {
        pmSetPixel(pixMap, x1, y1, c);
        if(x1 == x2 && y1 == y2)
            break;
        int e2 = err;
        if(e2 > -dx)
        {
            err -= dy;
            x1 += sx;
        }
        if(e2 < dy)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void pmRectangle(pmPixMap_t *pixMap, int x, int y, int w, int h, pmColor_t c)
{
    if(w <= 0 || h <= 0) return;
    int x2 = x + w - 1;
    int y2 = y + h - 1;
    pmHLine(pixMap, x, y, x2, c);
    pmHLine(pixMap, x, y2, x2, c);
    pmVLine(pixMap, x, y, y2, c);
    pmVLine(pixMap, x2, y, y2, c);
}

void pmRectangleRect(pmPixMap_t *pixMap, rcRectangle_t *rect, pmColor_t c)
{
    pmRectangle(pixMap, rect->X, rect->Y, rect->Width, rect->Height, c);
}

void pmRectanglePattern(pmPixMap_t *pixMap, int x, int y, int w, int h, unsigned pattern, pmColor_t c)
{
    if(w <= 0 || h <= 0) return;
    int x2 = x + w - 1;
    int y2 = y + h - 1;
    pmHLinePattern(pixMap, x, y, x2, pattern, c);
    pmHLinePattern(pixMap, x, y2, x2, pattern, c);
    pmVLinePattern(pixMap, x, y, y2, pattern, c);
    pmVLinePattern(pixMap, x2, y, y2, pattern, c);
}

void pmRectangleRectPattern(pmPixMap_t *pixMap, rcRectangle_t *rect, unsigned pattern, pmColor_t c)
{
    pmRectanglePattern(pixMap, rect->X, rect->Y, rect->Width, rect->Height, pattern, c);
}

void pmFillRectangle(pmPixMap_t *pixMap, int x, int y, int w, int h, pmColor_t c)
{
    if(w <= 0 || h <= 0)
        return;

    int x2 = x + w - 1;
    int y2 = y + h;

    if(x2 < 0 || x >= pixMap->Contents.Width || y2 < 0 || y >= pixMap->Contents.Height)
        return;

    for(int Y = y; Y < y2; ++Y)
        pmHLine(pixMap, x, Y, x2, c);
}

void pmFillRectangleRect(pmPixMap_t *pixMap, rcRectangle_t *rect, pmColor_t c)
{
    pmFillRectangle(pixMap, rect->X, rect->Y, rect->Width, rect->Height, c);
}

void pmAlphaRectangle(pmPixMap_t *pixMap, int x, int y, int w, int h, pmColor_t c)
{
    if(c.A == 0) return;
    else if(c.A == 255)
    {
        pmFillRectangle(pixMap, x, y, w, h, c);
        return;
    }

    if(w <= 0 || h <= 0)
        return;

    int x2 = x + w - 1;
    int y2 = y + h;

    if(x2 < 0 || x >= pixMap->Contents.Width || y2 < 0 || y >= pixMap->Contents.Height)
        return;

    // slow but works
    for(int Y = y; Y < y2; ++Y)
    {
        for(int X = x; X < x2; ++X)
            pmSetPixel(pixMap, X, Y, pmBlendPixel(pmGetPixel(pixMap, X, Y), c));
    }
}

void pmDrawFrame(pmPixMap_t *pixMap, int x, int y, int w, int h, int sunken, pmColor_t color)
{
    pmColor_t lighter = pmColorBrighten(color, 0.2f);
    pmColor_t darker = pmColorBrighten(color, -0.2f);

    pmColor_t tl = sunken ? darker : lighter;
    pmColor_t br = sunken ? lighter : darker;

    pmHLine(pixMap, x, y + h - 1, x + w - 1, br);
    pmVLine(pixMap, x + w - 1, y, y + h - 1, br);
    pmHLine(pixMap, x, y, x + w - 1, tl);
    pmVLine(pixMap, x, y, y + h - 1, tl);
}

void pmDrawFrameRect(pmPixMap_t *pixMap, rcRectangle_t *rect, int sunken, pmColor_t color)
{
    return pmDrawFrame(pixMap, rect->X, rect->Y, rect->Width, rect->Height, sunken, color);
}

void pmClear(pmPixMap_t *pixMap, pmColor_t color)
{
    pmFillRectangle(pixMap, 0, 0, pixMap->Contents.Width, pixMap->Contents.Height, color);
}

void pmBlit(pmPixMap_t *dst, pmPixMap_t *src, int sx, int sy, int x, int y, int w, int h)
{
    if(x < 0)
    {
        sx -= x;
        w += x;
        x = 0;
    }
    if(y < 0)
    {
        sy -= y;
        h += y;
        y = 0;
    }
    if(sx < 0)
    {
        x -= sx;
        w += sx;
        sx = 0;
    }
    if(sy < 0)
    {
        y -= sy;
        h += sy;
        sy = 0;
    }

    if(w < 0) w = src->Contents.Width;
    if(h < 0) h = src->Contents.Height;

    if(x >= dst->Contents.Width || y >= dst->Contents.Height)
        return;

    int x2 = min(dst->Contents.Width, x + w);
    int y2 = min(dst->Contents.Height, y + h);
    if(x2 <= 0 || y2 <= 0 || sx >= src->Contents.Width || sy >= src->Contents.Height)
        return;

    int sx2 = min(src->Contents.Width, sx + w);
    int sy2 = min(src->Contents.Height, sy + h);

    unsigned char *d = dst->Pixels + y * dst->Pitch + pmFormatPixelsToBytes(dst->Format, x);
    unsigned char *s = src->Pixels + sy * src->Pitch + pmFormatPixelsToBytes(src->Format, sx);
    if(pmFormatEqual(dst->Format, src->Format) && dst->Format.BPP >= 8)
    {   // use fast blit if pixel formats match
        int sw = sx2 - sx;
        int sh = sy2 - sy;
        int W = x2 - x;
        int H = y2 - y;
        if(W < 0 || H < 0)
            return;
        size_t bpl = pmFormatPixelsToBytes(dst->Format, min(sw, W));
        bltmove(d, s, bpl, dst->Pitch, src->Pitch, min(sh, H));
        return;
    }

    // fall back to slow GetPixel/SetPixel implementation
    if(s == d)
        return; // nothing to do
    else if(s > d)
    {   // forward
        if(src->Format.BPP == 8 && dst->Format.BPP == 32 && src->Palette)
        {   // try to make 8 bit to 32 bit blit a little bit faster by skipping pmColor conversions and some checks
            int as = dst->Format.AlphaShift;
            int a = dst->Format.AlphaBits ? 0xFF : 0x00;
            int rs = dst->Format.RedShift;
            int gs = dst->Format.GreenShift;
            int bs = dst->Format.BlueShift;
            for(int Y = y, sY = sy; Y < y2 && sY < sy2; ++Y, ++sY)
            {
                for(int X = x, sX = sx; X < x2 && sX < sx2; ++X, ++sX)
                {
                    unsigned char srcPix = *(src->Pixels + src->Pitch * Y + X);
                    union pmColor col = src->Palette[srcPix];
                    *(unsigned int *)(dst->Pixels + dst->Pitch * Y + 4 * X) = col.R << rs | col.G << gs | col.B << bs | a << as;
                }
            }
        }
        else
        {
            for(int Y = y, sY = sy; Y < y2 && sY < sy2; ++Y, ++sY)
            {
                for(int X = x, sX = sx; X < x2 && sX < sx2; ++X, ++sX)
                    pmSetPixel(dst, X, Y, pmGetPixel(src, sX, sY));
            }
        }
    }
    else
    {   // backward
        if(src->Format.BPP == 8 && dst->Format.BPP == 32 && src->Palette)
        {   // try to make 8 bit to 32 bit blit a little bit faster by skipping pmColor conversions and some checks
            int sx2b = sx + w + (x2 - (x + w));
            int sy2b = sy + h + (y2 - (y + h));
            int as = dst->Format.AlphaShift;
            int a = dst->Format.AlphaBits ? 0xFF : 0x00;
            int rs = dst->Format.RedShift;
            int gs = dst->Format.GreenShift;
            int bs = dst->Format.BlueShift;
            for(int Y = y2 - 1, sY = sy2b - 1; Y >= y && sY >= sy; --Y, --sY)
            {
                for(int X = x2 - 1, sX = sx2b - 1; X >= x && sX >= sx; --X, --sX)
                {
                    unsigned char srcPix = *(src->Pixels + src->Pitch * Y + X);
                    union pmColor col = src->Palette[srcPix];
                    *(unsigned int *)(dst->Pixels + dst->Pitch * Y + 4 * X) = col.R << rs | col.G << gs | col.B << bs | a << as;
                }
            }
        }
        else
        {
            int sx2b = sx + w + (x2 - (x + w));
            int sy2b = sy + h + (y2 - (y + h));
            for(int Y = y2 - 1, sY = sy2b - 1; Y >= y && sY >= sy; --Y, --sY)
            {
                for(int X = x2 - 1, sX = sx2b - 1; X >= x && sX >= sx; --X, --sX)
                    pmSetPixel(dst, X, Y, pmGetPixel(src, sX, sY));
            }
        }
    }
}

void pmAlphaBlit(pmPixMap_t *dst, pmPixMap_t *src, int sx, int sy, int x, int y, int w, int h)
{
    if(!src->Format.AlphaBits)
        return pmBlit(dst, src, sx, sy, x, y, w, h);

    if(x < 0)
    {
        sx -= x;
        w += x;
        x = 0;
    }
    if(y < 0)
    {
        sy -= y;
        h += y;
        y = 0;
    }
    if(sx < 0)
    {
        x -= sx;
        w += sx;
        sx = 0;
    }
    if(sy < 0)
    {
        y -= sy;
        h += sy;
        sy = 0;
    }

    if(w < 0) w = src->Contents.Width;
    if(h < 0) h = src->Contents.Height;

    if(x >= dst->Contents.Width || y >= dst->Contents.Height)
        return;

    int x2 = min(dst->Contents.Width, x + w);
    int y2 = min(dst->Contents.Height, y + h);
    if(x2 <= 0 || y2 <= 0 || sx >= src->Contents.Width || sy >= src->Contents.Height)
        return;

    int sx2 = min(src->Contents.Width, sx + w);
    int sy2 = min(src->Contents.Height, sy + h);

    unsigned char *d = dst->Pixels + y * dst->Pitch + pmFormatPixelsToBytes(dst->Format, x);
    unsigned char *s = src->Pixels + sy * src->Pitch + pmFormatPixelsToBytes(src->Format, sx);

    // slow !!!
    if(s == d)
        return; // nothing to do
    else if(s > d)
    {   // forward
        for(int Y = y, sY = sy; Y < y2 && sY < sy2; ++Y, ++sY)
        {
            for(int X = x, sX = sx; X < x2 && sX < sx2; ++X, ++sX)
                pmSetPixel(dst, X, Y, pmBlendPixel(pmGetPixel(dst, X, Y), pmGetPixel(src, sX, sY)));
        }
    }
    else
    {   // backward
        int sx2b = sx + w + (x2 - (x + w));
        int sy2b = sy + h + (y2 - (y + h));
        for(int Y = y2 - 1, sY = sy2b - 1; Y >= y && sY >= sy; --Y, --sY)
        {
            for(int X = x2 - 1, sX = sx2b - 1; X >= x && sX >= sx; --X, --sX)
                pmSetPixel(dst, X, Y, pmBlendPixel(pmGetPixel(dst, X, Y), pmGetPixel(src, sX, sY)));
        }
    }
}
