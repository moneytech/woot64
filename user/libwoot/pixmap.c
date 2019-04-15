#include <errno.h>
//#include <png.h>
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

pmColor_t pmColorBlack = { 0xFF, 0x00, 0x00, 0x00 };
pmColor_t pmColorBlue = { 0xFF, 0x00, 0x00, 0xAA };
pmColor_t pmColorGreen = { 0xFF, 0x00, 0xAA, 0x00 };
pmColor_t pmColorCyan = { 0xFF, 0x00, 0xAA, 0xAA };
pmColor_t pmColorRed = { 0xFF, 0xAA, 0x00, 0x00 };
pmColor_t pmColorMagenta = { 0xFF, 0xAA, 0x00, 0xAA };
pmColor_t pmColorBrown = { 0xFF, 0xAA, 0x55, 0x00 };
pmColor_t pmColorGray = { 0xFF, 0xAA, 0xAA, 0xAA };
pmColor_t pmColorDarkGray = { 0xFF, 0x55, 0x55, 0x55 };
pmColor_t pmColorBrightBlue = { 0xFF, 0x55, 0x55, 0xFF };
pmColor_t pmColorBrightGreen = { 0xFF, 0x55, 0xFF, 0x55 };
pmColor_t pmColorBrightCyan = { 0xFF, 0x55, 0xFF, 0xFF };
pmColor_t pmColorBrightRed = { 0xFF, 0xFF, 0x55, 0x55 };
pmColor_t pmColorBrightMagenta = { 0xFF, 0xFF, 0x55, 0xFF };
pmColor_t pmColorYellow = { 0xFF, 0xFF, 0xFF, 0x55 };
pmColor_t pmColorWhite = { 0xFF, 0xFF, 0xFF, 0xFF };
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
    pixMap->Dirty = pixMap->Contents;
}

rcRectangle_t pmGetDirtyRectangle(pmPixMap_t *pixMap)
{
    if(!pixMap) return rcRectangleEmpty;
    return pixMap->Dirty;
}

rcRectangle_t pmGetAndClearDirtyRectangle(pmPixMap_t *pixMap)
{
    rcRectangle_t dirty = pmGetDirtyRectangle(pixMap);
    pmClearDirty(pixMap);
    return dirty;
}

void pmClearDirty(pmPixMap_t *pixMap)
{
    if(!pixMap) return;
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

/*pmPixMap_t *pmLoadPNG(const char *filename)
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
}*/

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
    {   // palettized immages not supported
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

void pmVLine(pmPixMap_t *pixMap, int x, int y1, int y2, pmColor_t c)
{
    // just naive and slow implementation using SetPixel()
    if(y1 > y2) swap(int, y1, y2);
    if(x < 0 || x >= pixMap->Contents.Width || (y1 < 0 && y2 < 0) || (y1 >= pixMap->Contents.Height && y2 >= pixMap->Contents.Height))
        return;
    for(int Y = y1; Y <= y2; ++Y)
        pmSetPixel(pixMap, x, Y, c);
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
