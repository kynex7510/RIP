/**
 * @file Format.h
 * @brief Image format utilities.
 */
#ifndef _RIP_FORMAT_H
#define _RIP_FORMAT_H

#include <RIP/Defs.h>

/// @brief Image formats.
typedef enum {
    RIP_FORMAT_ALPHA,           ///< ALPHA
    RIP_FORMAT_LUMINANCE,       ///< LUMINANCE
    RIP_FORMAT_LUMINANCE_ALPHA, ///< LUMINANCE_ALPHA
    RIP_FORMAT_RGB,             ///< RGB
    RIP_FORMAT_RGBA,            ///< RGBA
    RIP_FORMAT_HILO,            ///< HILO
    RIP_FORMAT_ETC1,            ///< ETC1
    RIP_FORMAT_ETC1A4,          ///< ETC1A4
} RIPFormat;

/// @brief Pixel types.
typedef enum {
    RIP_TYPE_UNSIGNED_NIBBLE,        ///< UNSIGNED_NIBBLE
    RIP_TYPE_UNSIGNED_BYTE,          ///< UNSIGNED_BYTE
    RIP_TYPE_UNSIGNED_BYTE_4_4,      ///< UNSIGNED_BYTE_4_4
    RIP_TYPE_UNSIGNED_SHORT_5_6_5,   ///< UNSIGNED_SHORT_5_6_5
    RIP_TYPE_UNSIGNED_SHORT_5_5_5_1, ///< UNSIGNED_SHORT_5_5_5_1
    RIP_TYPE_UNSIGNED_SHORT_4_4_4_4, ///< UNSIGNED_SHORT_4_4_4_4
} RIPType;

/// @brief Pixel formats.
typedef enum {
    RIP_PIXELFORMAT_RGBA8,  ///< RGBA8
    RIP_PIXELFORMAT_RGB8,   ///< RGB8
    RIP_PIXELFORMAT_RGB565, ///< RGB565
    RIP_PIXELFORMAT_RGB5A1, ///< RGB5A1
    RIP_PIXELFORMAT_RGBA4,  ///< RGBA4
    RIP_PIXELFORMAT_A8,     ///< A8
    RIP_PIXELFORMAT_A4,     ///< A4
    RIP_PIXELFORMAT_L8,     ///< L8
    RIP_PIXELFORMAT_L4,     ///< L4
    RIP_PIXELFORMAT_LA8,    ///< LA8
    RIP_PIXELFORMAT_LA4,    ///< LA4
    RIP_PIXELFORMAT_HILO8,  ///< HILO8
    RIP_PIXELFORMAT_ETC1,   ///< ETC1
    RIP_PIXELFORMAT_ETC1A4, ///< ETC1A4
} RIPPixelFormat;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Get pixel format from image format and pixel type.
 * @param[in] format Image format.
 * @param[in] type Pixel type.
 * @param[out] out Pixel format.
 * @return True if the format-type pair make up a valid pixel format, false otherwise.
 */
bool ripGetPixelFormat(RIPFormat format, RIPType type, RIPPixelFormat* out);

/**
 * @brief Get image format and pixel type from pixel format.
 * @param[in] pixelFormat Pixel format.
 * @param[out] format Image format.
 * @param[out] type Pixel type.
 */
void ripGetFormatType(RIPPixelFormat pixelFormat, RIPFormat* outFormat, RIPType* outType);

/**
 * @brief Get bits per pixel from pixel format.
 * @param[in] pixelFormat Pixel format.
 * @return Pixel format bits per pixel.
 */
size_t ripGetPixelFormatBPP(RIPPixelFormat pixelFormat);

/**
 * @brief Get bits per pixel from image format and pixel type.
 * @param[in] format Image format.
 * @param[in] type Pixel type.
 * @return Pixel format bits per pixel.
 */
RIP_INLINE size_t ripGetFormatTypeBPP(RIPFormat format, RIPType type) {
    RIPPixelFormat pixelFormat;

    if (ripGetPixelFormat(format, type, &pixelFormat))
        return ripGetPixelFormatBPP(pixelFormat);

    return 0;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _RIP_FORMAT_H */