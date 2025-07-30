/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <RIP/Convert.h>
#include <RIP/Pixels.h>
#include <RIP/Tiling.h>

#include "Allocator.h"

bool ripConvertToNative(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    bool ret = false;

    if (src && dst) {
        if (flip) {
            const size_t size = (width * height * ripGetPixelFormatBPP(pixelFormat)) >> 3;
            void* tmp = ripLinearAlloc(size);
            RIP_ASSERT(tmp);

            if (ripSwapPixelBytes(src, tmp, width, height, pixelFormat, true))
                ret = ripTile(tmp, dst, width, height, pixelFormat);

            ripLinearFree(tmp);
        } else {
            if (ripTile(src, dst, width, height, pixelFormat)) {
                ret = ripSwapPixelBytes(dst, dst, width, height, pixelFormat, false);
            }
        }
    }

    return ret;
}

bool ripConvertFromNative(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    bool ret = false;

    if (src && dst) {
        if (ripUntile(src, dst, width, height, pixelFormat))
            ret = ripSwapPixelBytes(dst, dst, width, height, pixelFormat, flip);
    }

    return ret;
}

bool ripConvertInPlaceToNative(u8* p, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    bool ret = false;

    if (p) {
        const size_t size = (width * height * ripGetPixelFormatBPP(pixelFormat)) >> 3;
        void* tmp = ripLinearAlloc(size);
        RIP_ASSERT(tmp);

        if (ripSwapPixelBytes(p, tmp, width, height, pixelFormat, flip))
            ret = ripTile(tmp, p, width, height, pixelFormat);
            
        ripLinearFree(tmp);
    }

    return ret;
}

bool ripConvertInPlaceFromNative(u8* p, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    bool ret = false;

    if (p) {    
        const size_t size = (width * height * ripGetPixelFormatBPP(pixelFormat)) >> 3;
        void* tmp = ripLinearAlloc(size);
        RIP_ASSERT(tmp);

        if (ripUntile(p, tmp, width, height, pixelFormat))
            ret = ripSwapPixelBytes(tmp, p, width, height, pixelFormat, flip);

        ripLinearFree(tmp);
    }

    return ret;
}