#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct vecVector vecVector_t;

vecVector_t *vecCreate(unsigned elementSize, unsigned preAlloc, unsigned allocIncr, unsigned maxSize);
void vecDelete(vecVector_t *vec);
void vecClear(vecVector_t *vec);
unsigned vecSize(vecVector_t *vec);
unsigned vecAllocSize(vecVector_t *vec);
void *vecGet(vecVector_t *vec, unsigned idx);
void vecSet(vecVector_t *vec, unsigned idx, void *value);
int vecAppend(vecVector_t *vec, void *value);
int vecPrepend(vecVector_t *vec, void *value);
int vecInsertBefore(vecVector_t *vec, unsigned idx, void *value);
int vecInsertAfter(vecVector_t *vec, unsigned idx, void *value);
int vecRemoveAt(vecVector_t *vec, unsigned idx);
void vecTrim(vecVector_t *vec);
int vecSwap(vecVector_t *vec, unsigned idx1, unsigned idx2);

#ifdef __cplusplus
}
#endif // __cplusplus
