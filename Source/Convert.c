#include <RIP/Convert.h>
#include <RIP/Swap.h>
#include <RIP/Tiling.h>
#include <RIP/Cache.h>

#include <string.h> // memcpy

#include "Allocator.h"

bool ripConvertToNative(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    const size_t size = (width * height * ripGetPixelFormatBPP(pixelFormat)) >> 3;
    void* tmp = ripLinearAlloc(size);
    if (!tmp)
        return false;

    ripSwapBytes(src, tmp, width, height, pixelFormat, flip);
    ripFlushCache(tmp, size);
    const bool ret = ripTile(tmp, dst, width, height, pixelFormat);

    ripLinearFree(tmp);
    return ret;
}

bool ripConvertFromNative(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    const size_t size = (width * height * ripGetPixelFormatBPP(pixelFormat)) >> 3;
    void* tmp = ripLinearAlloc(size);
    if (!tmp)
        return false;

    bool ret = false;
    if (ripTile(src, tmp, width, height, pixelFormat)) {
        ripInvalidateCache(tmp, size);
        ripSwapBytes(tmp, dst, width, height, pixelFormat, flip);
        ret = true;
    }

    ripLinearFree(tmp);
    return ret;
}

bool ripConvertInPlaceToNative(u8* p, u16 width, u16 height, RIPPixelFormat pixelFormat) {
    const size_t size = (width * height * ripGetPixelFormatBPP(pixelFormat)) >> 3;
    void* tmp = ripLinearAlloc(size);
    if (!tmp)
        return false;

    ripSwapBytesInPlace(p, width, height, pixelFormat);
    ripFlushCache(p, size);

    bool ret = false;
    if (ripTile(p, tmp, width, height, pixelFormat)) {
        ripInvalidateCache(tmp, size);
        memcpy(p, tmp, size);
        ret = true;
    }

    ripLinearFree(tmp);
    return ret;
}

bool ripConvertInPlaceFromNative(u8* p, u16 width, u16 height, RIPPixelFormat pixelFormat) {
    const size_t size = (width * height * ripGetPixelFormatBPP(pixelFormat)) >> 3;
    void* tmp = ripLinearAlloc(size);
    if (!tmp)
        return false;

    bool ret = false;
    if (ripUntile(p, tmp, width, height, pixelFormat)) {
        ripInvalidateCache(tmp, size);
        memcpy(p, tmp, size);
        ripSwapBytesInPlace(p, width, height, pixelFormat);
        ret = true;
    }

    ripLinearFree(tmp);
    return ret;
}