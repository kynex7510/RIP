/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <RIP/Format.h>

#define INVALID_PIXEL_FORMAT (RIPPixelFormat)(-1)

RIP_INLINE RIPPixelFormat getPFAlpha(RIPType type) {
    switch (type) {
        case RIP_TYPE_UNSIGNED_BYTE:
            return RIP_PIXELFORMAT_A8;
        case RIP_TYPE_UNSIGNED_NIBBLE:
            return RIP_PIXELFORMAT_A4;
        default:
            return INVALID_PIXEL_FORMAT;
    }
}

RIP_INLINE RIPPixelFormat getPFLuminance(RIPType type) {
    switch (type) {
        case RIP_TYPE_UNSIGNED_BYTE:
            return RIP_PIXELFORMAT_L8;
        case RIP_TYPE_UNSIGNED_NIBBLE:
            return RIP_PIXELFORMAT_L4;
        default:
            return INVALID_PIXEL_FORMAT;
    }
}

RIP_INLINE RIPPixelFormat getPFLuminanceAlpha(RIPType type) {
    switch (type) {
        case RIP_TYPE_UNSIGNED_BYTE:
            return RIP_PIXELFORMAT_LA8;
        case RIP_TYPE_UNSIGNED_BYTE_4_4:
            return RIP_PIXELFORMAT_LA4;
        default:
            return INVALID_PIXEL_FORMAT;
    }
}

RIP_INLINE RIPPixelFormat getPFRGB(RIPType type) {
    switch (type) {
        case RIP_TYPE_UNSIGNED_BYTE:
            return RIP_PIXELFORMAT_RGB8;
        case RIP_TYPE_UNSIGNED_SHORT_5_6_5:
            return RIP_PIXELFORMAT_RGB565;
        default:
            return INVALID_PIXEL_FORMAT;
    }
}

RIP_INLINE RIPPixelFormat getPFRGBA(RIPType type) {
    switch (type) {
        case RIP_TYPE_UNSIGNED_BYTE:
            return RIP_PIXELFORMAT_RGBA8;
        case RIP_TYPE_UNSIGNED_SHORT_5_5_5_1:
            return RIP_PIXELFORMAT_RGB5A1;
        case RIP_TYPE_UNSIGNED_BYTE_4_4:
            return RIP_PIXELFORMAT_RGBA4;
        default:
            return INVALID_PIXEL_FORMAT;
    }
}

bool ripGetPixelFormat(RIPFormat format, RIPType type, RIPPixelFormat* out) {
    RIPPixelFormat pixelFormat = INVALID_PIXEL_FORMAT;

    switch (format) {
        case RIP_FORMAT_ALPHA:
            pixelFormat = getPFAlpha(type);
            break;
        case RIP_FORMAT_LUMINANCE:
            pixelFormat = getPFLuminance(type);
            break;
        case RIP_FORMAT_LUMINANCE_ALPHA:
            pixelFormat = getPFLuminanceAlpha(type);
            break;
        case RIP_FORMAT_RGB:
            pixelFormat = getPFRGB(type);
            break;
        case RIP_FORMAT_RGBA:
            pixelFormat = getPFRGBA(type);
        case RIP_FORMAT_HILO:
            if (type == RIP_TYPE_UNSIGNED_BYTE)
                pixelFormat = RIP_PIXELFORMAT_HILO8;
            break;
        case RIP_FORMAT_ETC1:
            if (type == RIP_TYPE_UNSIGNED_NIBBLE)
                pixelFormat = RIP_PIXELFORMAT_ETC1;
            break;
        case RIP_FORMAT_ETC1A4:
            if (type == RIP_TYPE_UNSIGNED_BYTE_4_4)
                pixelFormat = RIP_PIXELFORMAT_ETC1A4;
            break;
    }

    if (pixelFormat != INVALID_PIXEL_FORMAT) {
        if (out)
            *out = pixelFormat;

        return true;
    }

    return false;
}

void ripGetFormatType(RIPPixelFormat pixelFormat, RIPFormat* outFormat, RIPType* outType) {
    RIPFormat format = RIP_FORMAT_RGB;
    RIPType type = RIP_TYPE_UNSIGNED_BYTE;

    switch (pixelFormat) {
        case RIP_PIXELFORMAT_RGBA8:
            format = RIP_FORMAT_RGBA;
            type = RIP_TYPE_UNSIGNED_BYTE;
            break;
        case RIP_PIXELFORMAT_RGB8:
            format = RIP_FORMAT_RGB;
            type = RIP_TYPE_UNSIGNED_BYTE;
            break;
        case RIP_PIXELFORMAT_RGB565:
            format = RIP_FORMAT_RGB;
            type = RIP_TYPE_UNSIGNED_SHORT_5_6_5;
            break;
        case RIP_PIXELFORMAT_RGB5A1:
            format = RIP_FORMAT_RGBA;
            type = RIP_TYPE_UNSIGNED_SHORT_5_5_5_1;
            break;
        case RIP_PIXELFORMAT_RGBA4:
            format = RIP_FORMAT_RGBA;
            type = RIP_TYPE_UNSIGNED_SHORT_4_4_4_4;
            break;
        case RIP_PIXELFORMAT_A8:
            format = RIP_FORMAT_ALPHA;
            type = RIP_TYPE_UNSIGNED_BYTE;
            break;
        case RIP_PIXELFORMAT_A4:
            format = RIP_FORMAT_ALPHA;
            type = RIP_TYPE_UNSIGNED_NIBBLE;
            break;
        case RIP_PIXELFORMAT_L8:
            format = RIP_FORMAT_LUMINANCE;
            type = RIP_TYPE_UNSIGNED_BYTE;
            break;
        case RIP_PIXELFORMAT_L4:
            format = RIP_FORMAT_LUMINANCE;
            type = RIP_TYPE_UNSIGNED_NIBBLE;
            break;
        case RIP_PIXELFORMAT_LA8:
            format = RIP_FORMAT_LUMINANCE_ALPHA;
            type = RIP_TYPE_UNSIGNED_BYTE;
            break;
        case RIP_PIXELFORMAT_LA4:
            format = RIP_FORMAT_LUMINANCE_ALPHA;
            type = RIP_TYPE_UNSIGNED_BYTE_4_4;
            break;
        case RIP_PIXELFORMAT_HILO8:
            format = RIP_FORMAT_HILO;
            type = RIP_TYPE_UNSIGNED_BYTE;
            break;
        case RIP_PIXELFORMAT_ETC1:
            format = RIP_FORMAT_ETC1;
            type = RIP_TYPE_UNSIGNED_NIBBLE;
            break;
        case RIP_PIXELFORMAT_ETC1A4:
            format = RIP_FORMAT_ETC1A4;
            type = RIP_TYPE_UNSIGNED_BYTE_4_4;
            break;
    }

    if (outFormat)
        *outFormat = format;

    if (outType)
        *outType = type;
}

size_t ripGetPixelFormatBPP(RIPPixelFormat pixelFormat) {
    switch (pixelFormat) {
        case RIP_PIXELFORMAT_RGBA8:
            return 32;
        case RIP_PIXELFORMAT_RGB8:
            return 24;
        case RIP_PIXELFORMAT_RGB565:
        case RIP_PIXELFORMAT_RGB5A1:
        case RIP_PIXELFORMAT_RGBA4:
        case RIP_PIXELFORMAT_LA8:
        case RIP_PIXELFORMAT_HILO8:
            return 16;
        case RIP_PIXELFORMAT_L8:
        case RIP_PIXELFORMAT_A8:
        case RIP_PIXELFORMAT_LA4:
        case RIP_PIXELFORMAT_ETC1A4:
            return 8;
        case RIP_PIXELFORMAT_L4:
        case RIP_PIXELFORMAT_A4:
        case RIP_PIXELFORMAT_ETC1:
            return 4;
    }

    return 0;
}