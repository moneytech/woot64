#include <errno.h>
#include <freetype2/ft2build.h>
#include <woot/font.h>
#include <woot/pixmap.h>
#include <woot/vector.h>

#include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

typedef struct cacheGlyph
{
    FT_Bitmap *bitmap;
    float advance;
    int bitmapLeft;
    int bitmapTop;
} cacheGlyph_t;

static cacheGlyph_t invalidGlyph = { NULL, 0.0f };

// TODO: implement lru
typedef struct cacheEntry
{
    unsigned idx;
    cacheGlyph_t glyph;
} cacheEntry_t;

struct fntFont
{
    FT_Face face;
    vecVector_t *glyphCache;
    int hits, misses;
};

static FT_Library library;

static cacheGlyph_t getGlyph(fntFont_t *font, unsigned idx)
{
    // do binary search
    vecVector_t *cache = font->glyphCache;
    int cacheSize = vecSize(font->glyphCache);

    int m = 0;
    cacheEntry_t *a = NULL;
    if(cacheSize)
    {
        int l = 0, r = cacheSize - 1;
        while(l <= r)
        {
            m = (l + r) >> 1;
            a = vecGet(cache, m);
            if(!a) break;
            if(a->idx < idx) l = m + 1;
            else if(a->idx > idx) r = m - 1;
            else return a->glyph;
        }
    }

    // not in cache so include
    FT_Error error = FT_Load_Glyph(font->face, idx, FT_LOAD_DEFAULT);
    if(error) return invalidGlyph;
    error = FT_Render_Glyph(font->face->glyph, FT_RENDER_MODE_LCD);
    if(error) return invalidGlyph;

    FT_Bitmap *bmp = (FT_Bitmap *)calloc(1, sizeof(FT_Bitmap));
    FT_Bitmap_Copy(library, &font->face->glyph->bitmap, bmp);

    cacheGlyph_t glyph =
    {
        bmp,
        font->face->glyph->advance.x / 64.0f,
        font->face->glyph->bitmap_left,
        font->face->glyph->bitmap_top
    };
    cacheEntry_t entry = { idx, glyph };
    int res = !cacheSize || !a || idx >= a->idx ? vecInsertAfter(cache, m, &entry) : vecInsertBefore(cache, m, &entry);
    if(res < 0)
    {
        // TODO: add lru algorithm
    }
    return entry.glyph;
}

static int fntInitialize()
{
    if(!library)
    {
        FT_Error error = FT_Init_FreeType(&library);
        if(error)
        {
            library = NULL;
            return -EINVAL;
        }
    }
    return 0;
}

fntFont_t *fntLoad(const char *filename)
{
    fntInitialize();
    struct fntFont *font = (struct fntFont *)calloc(1, sizeof(struct fntFont));
    if(!font) return NULL;
    FT_Error error = FT_New_Face(library, filename, 0, &font->face);
    if(error)
    {
        font->face = NULL; // make sure it's NULL
        errno = EINVAL;
        fntDelete(font);
        return NULL;
    }
    font->glyphCache = vecCreate(sizeof(cacheEntry_t), 256, 16, 4096);
    if(!font->glyphCache)
    {
        fntDelete(font);
        return NULL;
    }
    return font;
}

int fntSetPixelSize(fntFont_t *font, int size)
{
    FT_Error error = FT_Set_Pixel_Sizes(font->face, 0, size);
    return error ? -EINVAL : 0;
}

int fntSetPointSize(fntFont_t *font, double size, int dpi)
{
    FT_Error error = FT_Set_Char_Size(font->face, 0, size * 64, 0, dpi);
    return error ? -EINVAL : 0;
}

float fntDrawCharacter(fntFont_t *font, pmPixMap_t *pixMap, int x, int y, int chr, pmColor_t color)
{
    cacheGlyph_t glyph = getGlyph(font, FT_Get_Char_Index(font->face, chr));
    if(!glyph.bitmap) return 8.0f;
    y += font->face->size->metrics.ascender >> 6;
    for(int i = 0; i < glyph.bitmap->rows; ++i)
    {
        unsigned char *line = (unsigned char *)(glyph.bitmap->buffer + i * glyph.bitmap->pitch);
        int _y = y + i - glyph.bitmapTop;
        if(glyph.bitmap->pixel_mode == FT_PIXEL_MODE_GRAY)
        {   // regular alpha blending
            for(int j = 0; j < glyph.bitmap->width; ++j)
            {
                int _x = x + j + glyph.bitmapLeft;
                pmSetPixel(pixMap, _x, _y, pmBlendPixel(pmGetPixel(pixMap, _x, _y), pmColorFromARGB(line[j], color.R, color.G, color.B)));
            }
        }
        else
        {
            for(int j = 0; j < glyph.bitmap->width / 3; ++j)
            {   // subpixel blending
                int _x = x + j + glyph.bitmapLeft;

                int px = 3 * j;
                int r = line[px];
                int g = line[px + 1];
                int b = line[px + 2];

                pmColor_t bcl = pmGetPixel(pixMap, _x, _y);

                r = (r * color.R + (255 - r) * bcl.R) >> 8;
                g = (g * color.G + (255 - g) * bcl.G) >> 8;
                b = (b * color.B + (255 - b) * bcl.B) >> 8;

                pmSetPixel(pixMap, _x, _y, pmColorFromRGB(r, g, b));
            }
        }
    }
    return glyph.advance;
}

float fntMeasureCharacter(fntFont_t *font, int chr)
{
    cacheGlyph_t glyph = getGlyph(font, FT_Get_Char_Index(font->face, chr));
    if(!glyph.bitmap) return 8.0f;
    return glyph.advance;
}

float fntDrawString(fntFont_t *font, pmPixMap_t *pixMap, int x, int y, const char *str, pmColor_t color)
{
    float a = x;
    while(*str)
        a += fntDrawCharacter(font, pixMap, a, y, *str++, color);
    return a;
}

float fntMeasureString(fntFont_t *font, const char *str, int n)
{
    float x = 0;
    int len = strlen(str);
    for(int i = 0; (n < 0 || i < n) && *str; ++i)
    {
        int dx = fntMeasureCharacter(font, *str++);
        if(dx < 0) return dx;
        x += dx;
    }
    return x;
}

float fntGetPixelAscender(fntFont_t *font)
{
    return font->face->size->metrics.ascender / 64.0f;
}

float fntGetPixelDescender(fntFont_t *font)
{
    return font->face->size->metrics.descender / 64.0f;
}

float fntGetPixelHeight(fntFont_t *font)
{
    return font->face->size->metrics.height / 64.0f;
}

void fntDelete(fntFont_t *font)
{
    if(!font) return;
    if(font->glyphCache)
    {
        unsigned cacheSize = vecSize(font->glyphCache);
        for(int i = 0; i < cacheSize; ++i)
        {
            cacheEntry_t *glyph = vecGet(font->glyphCache, i);
            if(!glyph) continue;
            if(glyph->glyph.bitmap) FT_Bitmap_Done(library, glyph->glyph.bitmap);
        }
        vecDelete(font->glyphCache);
    }
    if(font->face) FT_Done_Face(font->face);
}
