#include <RIP/Texture.h>

#include <stdlib.h> // qsort

static int cmpFaces(const void *a, const void *b) {
    _Static_assert(sizeof(u8*) == sizeof(u32), "Invalid size");

    const u32 x = *(const u32*)a;
    const u32 y = *(const u32*)b;

    if (x < y)
        return -1;

    if (x > y)
        return 1;

    return 0;
}

void ripSortTextureFaces(u8** faces) { qsort(faces, 6, sizeof(u8*), cmpFaces); }