/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <RIP/Convert.h>
#include <RIP/Swap.h>
#include <RIP/Tiling.h>

#include "Allocator.h"

bool ripConvertToNative(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    bool ret = false;

    if (flip) {
        const size_t size = (width * height * ripGetPixelFormatBPP(pixelFormat)) >> 3;
        void* tmp = ripLinearAlloc(size);

        if (tmp) {
            ripSwapBytes(src, tmp, width, height, pixelFormat, true);
            if (ripTile(tmp, dst, width, height, pixelFormat))
                ret = true;

            ripLinearFree(tmp);
        }
    } else {
        if (ripTile(src, dst, width, height, pixelFormat)) {
            ripSwapBytes(dst, dst, width, height, pixelFormat, false);
            ret = true;
        }
    }

    return ret;
}

bool ripConvertFromNative(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    if (ripUntile(src, dst, width, height, pixelFormat)) {
        ripSwapBytes(dst, dst, width, height, pixelFormat, flip);
        return true;
    }

    return false;
}

bool ripConvertInPlaceToNative(u8* p, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    const size_t size = (width * height * ripGetPixelFormatBPP(pixelFormat)) >> 3;
    void* tmp = ripLinearAlloc(size);

    if (tmp) {
        ripSwapBytes(p, tmp, width, height, pixelFormat, flip);
        const bool ret = ripTile(tmp, p, width, height, pixelFormat);
        ripLinearFree(tmp);
        return ret;
    }

    return false;
}

bool ripConvertInPlaceFromNative(u8* p, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    bool ret = false;
    const size_t size = (width * height * ripGetPixelFormatBPP(pixelFormat)) >> 3;
    void* tmp = ripLinearAlloc(size);

    if (tmp) {
        if (ripUntile(p, tmp, width, height, pixelFormat)) {
            ripSwapBytes(tmp, p, width, height, pixelFormat, flip);
            ret = true;
        }

        ripLinearFree(tmp);
    }

    return ret;
}