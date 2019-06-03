#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct fntFont fntFont_t;
typedef struct pmPixMap pmPixMap_t;
typedef union pmColor pmColor_t;

fntFont_t *fntLoad(const char *filename);
int fntSetPixelSize(fntFont_t *font, int size);
int fntSetPointSize(fntFont_t *font, double size, int dpi);
float fntDrawCharacter(fntFont_t *font, pmPixMap_t *pixMap, int x, int y, int chr, pmColor_t color);
float fntMeasureCharacter(fntFont_t *font, int chr);
float fntDrawString(fntFont_t *font, pmPixMap_t *pixMap, int x, int y, const char *str, pmColor_t color);
float fntMeasureString(fntFont_t *font, const char *str, int n);
float fntGetPixelAscender(fntFont_t *font);
float fntGetPixelDescender(fntFont_t *font);
float fntGetPixelHeight(fntFont_t *font);
void fntDelete(fntFont_t *font);

#ifdef __cplusplus
}
#endif // __cplusplus
