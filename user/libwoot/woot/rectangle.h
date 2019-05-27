#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct rcRectangle
{
    int X, Y;
    int Width, Height;
} rcRectangle_t;

extern rcRectangle_t rcRectangleEmpty;

rcRectangle_t rcCreate(int x, int y, int w, int h);
rcRectangle_t rcAdd(rcRectangle_t a, rcRectangle_t b);
rcRectangle_t rcAddP(rcRectangle_t *a, rcRectangle_t *b);
rcRectangle_t rcIntersect(rcRectangle_t a, rcRectangle_t b);
rcRectangle_t rcIntersectP(rcRectangle_t *a, rcRectangle_t *b);
int rcContainsPoint(rcRectangle_t rect, int x, int y);
int rcContainsPointP(rcRectangle_t *rect, int x, int y);
int rcIsEmpty(rcRectangle_t rect);
int rcIsEmptyP(rcRectangle_t *rect);

#ifdef __cplusplus
}
#endif // __cplusplus
