#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <sys/types.h>
#include <woot/rectangle.h>
#include <woot/video.h>

typedef struct pmPixelFormat
{
    unsigned BPP;
    unsigned AlphaBits, RedBits, GreenBits, BlueBits;
    unsigned AlphaShift, RedShift, GreenShift, BlueShift;
} pmPixelFormat_t;

typedef union pmColor
{
    struct
    {
        unsigned char A;
        unsigned char R;
        unsigned char G;
        unsigned char B;
    };
    unsigned Value;
} pmColor_t;

typedef struct pmColorHSL
{
    float A, H, S, L;
} pmColorHSL_t;

typedef struct pmColorHSB
{
    float A, H, S, B;
} pmColorHSB_t;

typedef struct pmPixMap
{
    struct pmPixMap *Parent;
    rcRectangle_t Contents;
    rcRectangle_t Dirty;
    unsigned Pitch;
    pmPixelFormat_t Format;
    pmColor_t *Palette;
    unsigned ReleasePixels;
    unsigned char *Pixels;
} pmPixMap_t;

extern pmColor_t pmColorAliceBlue;
extern pmColor_t pmColorAntiqueWhite;
extern pmColor_t pmColorAqua;
extern pmColor_t pmColorAquamarine;
extern pmColor_t pmColorAzure;
extern pmColor_t pmColorBeige;
extern pmColor_t pmColorBisque;
extern pmColor_t pmColorBlack;
extern pmColor_t pmColorBlanchedAlmond;
extern pmColor_t pmColorBlue;
extern pmColor_t pmColorBlueViolet;
extern pmColor_t pmColorBrown;
extern pmColor_t pmColorBurlyWood;
extern pmColor_t pmColorCadetBlue;
extern pmColor_t pmColorChartReuse;
extern pmColor_t pmColorChocolate;
extern pmColor_t pmColorCoral;
extern pmColor_t pmColorCornflowerBlue;
extern pmColor_t pmColorCornsilk;
extern pmColor_t pmColorCrimson;
extern pmColor_t pmColorCyan;
extern pmColor_t pmColorDarkBlue;
extern pmColor_t pmColorDarkCyan;
extern pmColor_t pmColorDarkGoldenrod;
extern pmColor_t pmColorDarkGray;
extern pmColor_t pmColorDarkGreen;
extern pmColor_t pmColorDarkKhaki;
extern pmColor_t pmColorDarkMagenta;
extern pmColor_t pmColorDarkOliveGreen;
extern pmColor_t pmColorDarkOrange;
extern pmColor_t pmColorDarkOrchid;
extern pmColor_t pmColorDarkRed;
extern pmColor_t pmColorDarkSalmon;
extern pmColor_t pmColorDarkSeaGreen;
extern pmColor_t pmColorDarkSlateBlue;
extern pmColor_t pmColorDarkSlateGray;
extern pmColor_t pmColorDarkTurquoise;
extern pmColor_t pmColorDarkViolet;
extern pmColor_t pmColorDeepPink;
extern pmColor_t pmColorDeepSkyBlue;
extern pmColor_t pmColorDimGray;
extern pmColor_t pmColorDodgerBlue;
extern pmColor_t pmColorFirebrick;
extern pmColor_t pmColorFloralWhite;
extern pmColor_t pmColorForestGreen;
extern pmColor_t pmColorFuchsia;
extern pmColor_t pmColorGainsboro;
extern pmColor_t pmColorGhostWhite;
extern pmColor_t pmColorGold;
extern pmColor_t pmColorGoldenrod;
extern pmColor_t pmColorGray;
extern pmColor_t pmColorGreen;
extern pmColor_t pmColorGreenYellow;
extern pmColor_t pmColorHoneydew;
extern pmColor_t pmColorHotPink;
extern pmColor_t pmColorIndianRed;
extern pmColor_t pmColorIndigo;
extern pmColor_t pmColorIvory;
extern pmColor_t pmColorKhaki;
extern pmColor_t pmColorLavender;
extern pmColor_t pmColorLavenderBlush;
extern pmColor_t pmColorLawnGreen;
extern pmColor_t pmColorLemonChiffon;
extern pmColor_t pmColorLightBlue;
extern pmColor_t pmColorLightCoral;
extern pmColor_t pmColorLightCyan;
extern pmColor_t pmColorLightGoldenrodYellow;
extern pmColor_t pmColorLightGray;
extern pmColor_t pmColorLightGreen;
extern pmColor_t pmColorLightPink;
extern pmColor_t pmColorLightSalmon;
extern pmColor_t pmColorLightSeaGreen;
extern pmColor_t pmColorLightSkyBlue;
extern pmColor_t pmColorLightSlateGray;
extern pmColor_t pmColorLightSteelBlue;
extern pmColor_t pmColorLightYellow;
extern pmColor_t pmColorLime;
extern pmColor_t pmColorLimeGreen;
extern pmColor_t pmColorLinen;
extern pmColor_t pmColorMagenta;
extern pmColor_t pmColorMaroon;
extern pmColor_t pmColorMediumAquamarine;
extern pmColor_t pmColorMediumBlue;
extern pmColor_t pmColorMediumOrchid;
extern pmColor_t pmColorMediumPurple;
extern pmColor_t pmColorMediumSeaGreen;
extern pmColor_t pmColorMediumSlateBlue;
extern pmColor_t pmColorMediumSpringGreen;
extern pmColor_t pmColorMediumTurquioise;
extern pmColor_t pmColorMediumVioletRed;
extern pmColor_t pmColorMidnightBlue;
extern pmColor_t pmColorMintCream;
extern pmColor_t pmColorMistyRose;
extern pmColor_t pmColorMoccasin;
extern pmColor_t pmColorNovajoWhite;
extern pmColor_t pmColorNavy;
extern pmColor_t pmColorOldLace;
extern pmColor_t pmColorOlive;
extern pmColor_t pmColorOliveDrab;
extern pmColor_t pmColorOrange;
extern pmColor_t pmColorOrangeRed;
extern pmColor_t pmColorOrchid;
extern pmColor_t pmColorPaleGoldenrod;
extern pmColor_t pmColorPaleGreen;
extern pmColor_t pmColorPaleTurquoise;
extern pmColor_t pmColorPaleVioletRed;
extern pmColor_t pmColorPapayaWhip;
extern pmColor_t pmColorPeachPuff;
extern pmColor_t pmColorPeru;
extern pmColor_t pmColorPink;
extern pmColor_t pmColorPlum;
extern pmColor_t pmColorPowderBlue;
extern pmColor_t pmColorPurple;
extern pmColor_t pmColorRed;
extern pmColor_t pmColorRosyBrown;
extern pmColor_t pmColorRoyalBlue;
extern pmColor_t pmColorSaddleBrown;
extern pmColor_t pmColorSalmon;
extern pmColor_t pmColorSandyBrown;
extern pmColor_t pmColorSeaGreen;
extern pmColor_t pmColorSeaShell;
extern pmColor_t pmColorSienna;
extern pmColor_t pmColorSilver;
extern pmColor_t pmColorSkyBlue;
extern pmColor_t pmColorSlateBlue;
extern pmColor_t pmColorSlateGray;
extern pmColor_t pmColorSnow;
extern pmColor_t pmColorSpringGreen;
extern pmColor_t pmColorSteelBlue;
extern pmColor_t pmColorTan;
extern pmColor_t pmColorTeal;
extern pmColor_t pmColorThistle;
extern pmColor_t pmColorTomato;
extern pmColor_t pmColorTurquoise;
extern pmColor_t pmColorViolet;
extern pmColor_t pmColorWheat;
extern pmColor_t pmColorWhite;
extern pmColor_t pmColorWhiteSmoke;
extern pmColor_t pmColorYellow;
extern pmColor_t pmColorYellowGreen;

extern pmColor_t pmColorTransparent;

extern pmPixelFormat_t pmFormatInvalid;
extern pmPixelFormat_t pmFormatA8R8G8B8;
extern pmPixelFormat_t pmFormatA0R8B8G8;

int pmFormatEqual(pmPixelFormat_t a, pmPixelFormat_t b);
int pmFormatEqualP(pmPixelFormat_t *a, pmPixelFormat_t *b);
unsigned pmFormatPixelsToBytes(pmPixelFormat_t format, unsigned pixels);
unsigned pmFormatPixelsToBytesP(pmPixelFormat_t *format, unsigned pixels);
pmPixelFormat_t pmFormatFromModeInfo(vidModeInfo_t *modeInfo);

unsigned char pmColorGetLuma(pmColor_t color);
pmColor_t pmColorFromRGB(unsigned char r, unsigned char g, unsigned char b);
pmColor_t pmColorFromARGB(unsigned char a, unsigned char r, unsigned char g, unsigned char b);
unsigned pmColorToValue(pmPixelFormat_t *format, pmColor_t color);
pmColor_t pmColorFromValue(pmPixelFormat_t *format, unsigned value);
pmColor_t pmColorFromIndex(pmPixMap_t *pixMap, unsigned index);
void pmColorRGB2HSL(pmColorHSL_t *dst, pmColor_t src);
pmColor_t pmColorHSL2RGB(pmColorHSL_t *src);
pmColor_t pmColorBrighten(pmColor_t color, float amount);
pmColor_t pmColorInvert(pmColor_t color);
pmColor_t *pmPaletteCreate(pmPixelFormat_t *format);
void pmPaletteDelete(pmColor_t *palette);

pmPixMap_t *pmCreate(int width, int height, pmPixelFormat_t *format);
void pmDelete(pmPixMap_t *pixMap);
void pmInvalidate(pmPixMap_t *pixMap, int x, int y, int w, int h);
void pmInvalidateRect(pmPixMap_t *pixMap, rcRectangle_t rect);
void pmInvalidateRectP(pmPixMap_t *pixMap, rcRectangle_t *rect);
void pmInvalidateWhole(pmPixMap_t *pixMap);
rcRectangle_t pmGetDirtyRectangle(pmPixMap_t *pixMap);
rcRectangle_t pmGetAndClearDirtyRectangle(pmPixMap_t *pixMap);
void pmClearDirty(pmPixMap_t *pixMap);
void pmClearParentDirty(pmPixMap_t *pixMap);
pmPixMap_t *pmFromMemory(int width, int height, int pitch, pmPixelFormat_t *format, void *pixels, unsigned releasePixels);
pmPixMap_t *pmFromPixMap(pmPixMap_t *src, pmPixelFormat_t *format);
pmPixMap_t *pmSubPixMap(pmPixMap_t *src, int x, int y, int w, int h);
pmPixMap_t *pmLoadPNG(const char *filename);
pmPixMap_t *pmLoadCUR(const char *filename, unsigned idx, int *hotX, int *hotY);
void pmSetPaletteEntry(pmPixMap_t *pixMap, unsigned idx, pmColor_t color);
rcRectangle_t pmGetRectangle(pmPixMap_t *pixMap);
unsigned pmGetPitch(pmPixMap_t *pixMap);
void *pmGetPixels(pmPixMap_t *pixMap);
void pmSetPixel(pmPixMap_t *pixMap, int x, int y, pmColor_t color);
pmColor_t pmGetPixel(pmPixMap_t *pixMap, int x, int y);
pmColor_t pmBlendPixel(pmColor_t a, pmColor_t b);
void pmHLine(pmPixMap_t *pixMap, int x1, int y, int x2, pmColor_t c);
void pmHLinePattern(pmPixMap_t *pixMap, int x1, int y, int x2, unsigned pattern, pmColor_t c);
void pmVLine(pmPixMap_t *pixMap, int x, int y1, int y2, pmColor_t c);
void pmVLinePattern(pmPixMap_t *pixMap, int x, int y1, int y2, unsigned pattern, pmColor_t c);
void pmVFlip(pmPixMap_t *pixMap);
void pmLine(pmPixMap_t *pixMap, int x1, int y1, int x2, int y2, pmColor_t c);
void pmRectangle(pmPixMap_t *pixMap, int x, int y, int w, int h, pmColor_t c);
void pmRectangleRect(pmPixMap_t *pixMap, rcRectangle_t *rect, pmColor_t c);
void pmRectanglePattern(pmPixMap_t *pixMap, int x, int y, int w, int h, unsigned pattern, pmColor_t c);
void pmRectangleRectPattern(pmPixMap_t *pixMap, rcRectangle_t *rect, unsigned pattern, pmColor_t c);
void pmFillRectangle(pmPixMap_t *pixMap, int x, int y, int w, int h, pmColor_t c);
void pmFillRectangleRect(pmPixMap_t *pixMap, rcRectangle_t *rect, pmColor_t c);
void pmAlphaRectangle(pmPixMap_t *pixMap, int x, int y, int w, int h, pmColor_t c);
void pmDrawFrame(pmPixMap_t *pixMap, int x, int y, int w, int h, int sunken, pmColor_t color);
void pmDrawFrameRect(pmPixMap_t *pixMap, rcRectangle_t *rect, int sunken, pmColor_t color);
void pmClear(pmPixMap_t *pixMap, pmColor_t color);
void pmBlit(pmPixMap_t *dst, pmPixMap_t *src, int sx, int sy, int x, int y, int w, int h);
void pmAlphaBlit(pmPixMap_t *dst, pmPixMap_t *src, int sx, int sy, int x, int y, int w, int h);

#ifdef __cplusplus
}
#endif // __cplusplus
