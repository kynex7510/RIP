/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <RIP/Pixels.h>
#include <RIP/Texture.h>

#include <string.h> // memcpy

static u8 g_TmpBuffer[RIP_MAX_TEX_DIMS * 4];

// Defined in PixelsImpl.s
extern void rip_memcpy(u8* dst, const u8* src, size_t size);
extern void rip_swapPixelBytesImpl(u8* dst, const u8* src, size_t size, size_t bytesPerPixel);

// To RGBA8.
extern void rip_RGB8ToRGBA8(u8* dst, const u8* src, size_t numPixels);
extern void rip_RGB565ToRGBA8(u8* dst, const u8* src, size_t numPixels);
extern void rip_RGB5A1ToRGBA8(u8* dst, const u8* src, size_t numPixels);
extern void rip_RGBA4ToRGBA8(u8* dst, const u8* src, size_t numPixels);

bool ripSwapPixelBytes(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    if (!src || !dst || width > RIP_MAX_TEX_DIMS || height > RIP_MAX_TEX_DIMS)
        return false;

    if (!width || !height)
        return true;

    const size_t bytesPerPixel = ripGetPixelFormatBPP(pixelFormat) >> 3;
    RIP_ASSERT(bytesPerPixel > 0);

    if (flip) {
        const size_t rowSize = width * bytesPerPixel;
        
        for (size_t r = 0; r < (height >> 1); ++r) {
            const size_t index0 = r * rowSize;
            const size_t index1 = (height - r - 1) * rowSize;

            rip_memcpy(g_TmpBuffer, &src[index0], rowSize);
            rip_swapPixelBytesImpl(&dst[index0], &src[index1], rowSize, bytesPerPixel);
            rip_swapPixelBytesImpl(&dst[index1], g_TmpBuffer, rowSize, bytesPerPixel);
        }
    } else {
        const size_t size = width * height * bytesPerPixel;
        rip_swapPixelBytesImpl(dst, src, size, bytesPerPixel);
    }

    return true;
}

static inline bool convertPixelsToRGBA8(const u8* src, u8* dst, size_t numPixels, RIPPixelFormat srcPixelFormat) {
    if (!src || !dst)
        return false;

    switch (srcPixelFormat) {
        case RIP_PIXELFORMAT_RGBA8:
            if (src != dst)
                rip_memcpy(dst, src, numPixels);
            return true;
        case RIP_PIXELFORMAT_RGB8:
            rip_RGB8ToRGBA8(dst, src, numPixels);
            return true;
        case RIP_PIXELFORMAT_RGB565:
            rip_RGB565ToRGBA8(dst, src, numPixels);
            return true;
        case RIP_PIXELFORMAT_RGB5A1:
            rip_RGB5A1ToRGBA8(dst, src, numPixels);
            return true;
        case RIP_PIXELFORMAT_RGBA4:
            rip_RGBA4ToRGBA8(dst, src, numPixels);
            return true;
        default:
            return false;
    }
}

bool ripConvertPixels(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat srcPixelFormat, RIPPixelFormat dstPixelFormat) {
    if (!src || !dst)
        return false;
    
    if (!width || !height)
        return true;

    const size_t numPixels = width * height;

    switch (dstPixelFormat) {
        case RIP_PIXELFORMAT_RGBA8:
            return convertPixelsToRGBA8(src, dst, numPixels, srcPixelFormat);
        default:
            return false;
    }
}