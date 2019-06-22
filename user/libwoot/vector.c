#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <woot/vector.h>

typedef struct vecVector
{
    unsigned char *data;
    unsigned elementSize;
    unsigned maxSize;
    unsigned currentSize;
    unsigned allocatedSize;
    unsigned allocIncr;
} vecVector_t;

static int upSize(vecVector_t *vec, unsigned n)
{
    unsigned newSize = vec->currentSize + n;
    if(newSize > vec->maxSize) return -ENOMEM;
    if(newSize > vec->allocatedSize)
    {
        unsigned newAllocSize = vec->allocatedSize + vec->allocIncr;
        if(newAllocSize > vec->maxSize) newAllocSize = vec->maxSize;
        errno = 0;
        unsigned char *newData = (unsigned char *)realloc(vec->data, vec->elementSize * newAllocSize);
        if(newAllocSize && !newData) return -errno;
        vec->data = newData;
        vec->allocatedSize = newAllocSize;
    }
    return 0;
}

static void memSwap(unsigned char *a, unsigned char *b, unsigned n)
{
    while(n--)
    {
        char tmp = *a;
        *a++ = *b;
        *b++ = tmp;
    }
}

vecVector_t *vecCreate(unsigned elementSize, unsigned preAlloc, unsigned allocIncr, unsigned maxSize)
{
    vecVector_t *vec = calloc(1, sizeof(vecVector_t));
    if(!vec) return NULL;

    vec->elementSize = elementSize;
    vec->maxSize = maxSize ? maxSize : __INT_MAX__;
    vec->allocIncr = allocIncr;
    vec->data = (unsigned char *)malloc(preAlloc * elementSize);
    if(preAlloc && !vec->data)
    {
        vecDelete(vec);
        return NULL;
    }
    vec->allocatedSize = preAlloc;
    return vec;
}

void vecDelete(vecVector_t *vec)
{
    if(!vec) return;
    if(vec->data) free(vec->data);
    free(vec);
}

void vecClear(vecVector_t *vec)
{
    if(!vec) return;
    vec->currentSize = 0;
}

unsigned vecSize(vecVector_t *vec)
{
    if(!vec) return 0;
    return vec->currentSize;
}

unsigned vecAllocSize(vecVector_t *vec)
{
    if(!vec) return 0;
    return vec->allocatedSize;
}

void *vecGet(vecVector_t *vec, unsigned idx)
{
    if(!vec || idx >= vec->currentSize) return NULL;
    return vec->data + vec->elementSize * idx;
}

void vecSet(vecVector_t *vec, unsigned idx, void *value)
{
    if(!vec || idx >= vec->currentSize) return;
    memmove(vec->data + vec->elementSize * idx, value, vec->elementSize);
}

int vecAppend(vecVector_t *vec, void *value)
{
    if(!vec) return -EINVAL;
    int res = upSize(vec, 1);
    if(res < 0) return res;
    memmove(vec->data + vec->elementSize * vec->currentSize++, value, vec->elementSize);
    return 0;
}

int vecPrepend(vecVector_t *vec, void *value)
{
    if(!vec) return -EINVAL;
    int res = upSize(vec, 1);
    if(res < 0) return res;
    memmove(vec->data + vec->elementSize, vec->data, vec->currentSize * vec->elementSize);
    memmove(vec->data, value, vec->elementSize);
    ++vec->currentSize;
    return 0;
}


int vecInsertBefore(vecVector_t *vec, unsigned idx, void *value)
{
    if(!vec) return -EINVAL;
    if(idx > vec->currentSize) idx = vec->currentSize;
    int res = upSize(vec, 1);
    if(res < 0) return res;
    memmove(vec->data + (1 + idx) * vec->elementSize,
            vec->data + idx * vec->elementSize,
            (vec->currentSize - idx) * vec->elementSize);
    memmove(vec->data + idx * vec->elementSize, value, vec->elementSize);
    ++vec->currentSize;
    return 0;
}

int vecInsertAfter(vecVector_t *vec, unsigned idx, void *value)
{
    if(!vec) return -EINVAL;
    if(idx >= vec->currentSize) idx = vec->currentSize - 1;
    int res = upSize(vec, 1);
    if(res < 0) return res;
    memmove(vec->data + (2 + idx) * vec->elementSize,
            vec->data + (1 + idx) * vec->elementSize,
            (vec->currentSize - idx - 1) * vec->elementSize);
    memmove(vec->data + (1 + idx) * vec->elementSize, value, vec->elementSize);
    ++vec->currentSize;
    return 0;
}

int vecRemoveAt(vecVector_t *vec, unsigned idx)
{
    if(!vec) return -EINVAL;
    if(idx >= vec->currentSize) return -ENOENT;
    memmove(vec->data + idx * vec->elementSize,
            vec->data + (1 + idx) * vec->elementSize,
            (vec->currentSize - idx - 1) * vec->elementSize);
    --vec->currentSize;
    return 0;
}

void vecTrim(vecVector_t *vec)
{
    if(!vec) return;
    if(vec->currentSize >= vec->allocatedSize)
        return;
    unsigned char *newData = (unsigned char *)realloc(vec->data, vec->currentSize * vec->elementSize);
    if(!newData) return;
    vec->data = newData;
    vec->allocatedSize = vec->currentSize;
}

int vecSwap(vecVector_t *vec, unsigned idx1, unsigned idx2)
{
    if(!vec) return -EINVAL;
    if(idx1 >= vec->currentSize || idx2 >= vec->currentSize)
        return -ENOENT;
    if(idx1 == idx2) return 0;
    memSwap(vec->data + idx1 * vec->elementSize, vec->data + idx2 * vec->elementSize, vec->elementSize);
    return 0;
}
