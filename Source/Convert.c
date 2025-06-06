#include <RIP/Convert.h>
#include <RIP/Swap.h>
#include <RIP/Tiling.h>
#include <RIP/Cache.h>

#if RIP_BACKEND == RIP_BACKEND_KYGX
#include <GX/Allocator.h>
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU
#include <3ds.h>
#elif RIP_BACKEND == RIP_BACKEND_CITRO3D
#include <citro3d.h>
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS
// TODO
#else
#include <stdlib.h> // malloc, free
#endif

#include <string.h> // memcpy

static void* allocTmpBuffer(size_t size) {
#if RIP_BACKEND == RIP_BACKEND_KYGX
    return kygxAlloc(GX_MEM_LINEAR, size);
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU || RIP_BACKEND == RIP_BACKEND_CITRO3D
    return linearAlloc(size);
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS
// TODO
#error "Unimplemented!"
#else
    return malloc(size);
#endif
}

static void freeTmpBuffer(void* p) {
#if RIP_BACKEND == RIP_BACKEND_KYGX
    return kygxFree(p);
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU || RIP_BACKEND == RIP_BACKEND_CITRO3D
    return linearFree(p);
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS
// TODO
#error "Unimplemented!"
#else // None (software)
    return free(p);
#endif
}

bool ripConvertToNative(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    const size_t size = (width * height * ripGetPixelFormatBPP(pixelFormat)) >> 3;
    void* tmp = allocTmpBuffer(size);
    if (!tmp)
        return false;

    ripSwapBytes(src, tmp, width, height, pixelFormat, flip);
    ripFlushCache(tmp, size);
    const bool ret = ripTile(tmp, dst, width, height, pixelFormat);

    freeTmpBuffer(tmp);
    return ret;
}

bool ripConvertFromNative(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    const size_t size = (width * height * ripGetPixelFormatBPP(pixelFormat)) >> 3;
    void* tmp = allocTmpBuffer(size);
    if (!tmp)
        return false;

    bool ret = false;
    if (ripTile(src, tmp, width, height, pixelFormat)) {
        ripInvalidateCache(tmp, size);
        ripSwapBytes(tmp, dst, width, height, pixelFormat, flip);
        ret = true;
    }

    freeTmpBuffer(tmp);
    return ret;
}

bool ripConvertInPlaceToNative(u8* p, u16 width, u16 height, RIPPixelFormat pixelFormat) {
    const size_t size = (width * height * ripGetPixelFormatBPP(pixelFormat)) >> 3;
    void* tmp = allocTmpBuffer(size);
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

    freeTmpBuffer(tmp);
    return ret;
}

bool ripConvertInPlaceFromNative(u8* p, u16 width, u16 height, RIPPixelFormat pixelFormat) {
    const size_t size = (width * height * ripGetPixelFormatBPP(pixelFormat)) >> 3;
    void* tmp = allocTmpBuffer(size);
    if (!tmp)
        return false;

    bool ret = false;
    if (ripUntile(p, tmp, width, height, pixelFormat)) {
        ripInvalidateCache(tmp, size);
        memcpy(p, tmp, size);
        ripSwapBytesInPlace(p, width, height, pixelFormat);
        ret = true;
    }

    freeTmpBuffer(tmp);
    return ret;
}