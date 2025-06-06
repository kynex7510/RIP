#include <RIP/Swap.h>

#include <string.h> // memcpy

typedef void(*SwapBytes_t)(u8* buffer, size_t size);

static void swapBytes4(u8* buffer, size_t size) {
    u32* p = (u32*)buffer;
    for (size_t i = 0; i < (size >> 2); ++i) {
        const u32 v = p[i];
        p[i] = ((v >> 24) | ((v >> 8) & 0xFF00) | ((v << 8) & 0xFF0000) | (v << 24));
    }
}

static void swapBytes3(u8* buffer, size_t size) {
    for (size_t i = 0; i < size; i += 3) {
        const u8 tmp = buffer[i];
        buffer[i] = buffer[i + 2];
        buffer[i + 2] = tmp;
    }
}

static void swapBytes2(u8* buffer, size_t size) {
    u16* p = (u16*)buffer;
    for (size_t i = 0; i < (size >> 1); ++i) {
        const u16 v = p[i];
        p[i] = ((v >> 8) | (v << 8));
    }
}

static SwapBytes_t getSwapBytesFn(size_t bpp) {
    switch (bpp >> 3) {
        case 4:
            return swapBytes4;
        case 3:
            return swapBytes3;
        case 2:
            return swapBytes2;
        default:
            return NULL;
    }
}

void ripSwapBytes(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    const size_t bpp = ripGetPixelFormatBPP(pixelFormat);
    const SwapBytes_t swapBytesFn = getSwapBytesFn(bpp);

    if (flip) {
        const size_t rowSize = (width * bpp) >> 3;
        for (size_t r = 0; r < height; ++r) {
            const size_t srcIndex = r * rowSize;
            const size_t dstIndex = (height - r - 1) * rowSize;
            memcpy(&dst[dstIndex], &src[srcIndex], rowSize);

            if (swapBytesFn)
                swapBytesFn(&dst[dstIndex], rowSize);
        }
    } else {
        const size_t size = (width * height * bpp) >> 3;
        memcpy(dst, src, size);

        if (swapBytesFn)
            swapBytesFn(dst, size);
    }
}

void ripSwapBytesInPlace(u8* p, u16 width, u16 height, RIPPixelFormat pixelFormat) {
    const size_t bpp = ripGetPixelFormatBPP(pixelFormat);
    const SwapBytes_t swapBytesFn = getSwapBytesFn(bpp);

    if (swapBytesFn)
        swapBytesFn(p, (width * height * bpp) >> 3);
}