/**
 * @file Texture.h
 * @brief Texture utilities.
 */
#ifndef _RIP_TEXTURE_H
#define _RIP_TEXTURE_H

#include <RIP/Format.h>

#define RIP_MAX_TEX_LEVELS 8 ///< Max possible number of texture levels.

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Get offset for texture data at the specified level.
 * @param[in] width Texture width.
 * @param[in] height Texture height.
 * @param[in] pixelFormat Texture pixel format.
 * @param[in] level Texture level.
 * @return Texture data offset.
 */
RIP_INLINE size_t ripGetTextureDataOffset(u16 width, u16 height, RIPPixelFormat pixelFormat, size_t level) {
    if (level > 0) {
        // Basically rewrite the offset in terms of the previous level dimensions:
        // B * W(L-1) * H(L-1) * (2^2(L-1) + 2^2(L-2) + ... + 2^2(L-L)) / 8
        const u16 prevWidth = (width >> (level - 1));
        const u16 prevHeight = (height >> (level - 1));
        const size_t bpp = ripGetPixelFormatBPP(pixelFormat);
        return ((bpp * prevWidth * prevHeight * (((1u << (level << 1)) - 1) & 0x55555)) >> 3);
    }

    return 0;
}

/**
 * @brief Get max possible number of texture levels for the specified dimensions.
 * @param[in] width Texture width.
 * @param[in] height Texture height.
 * @return Max number of texture levels.
 */
RIP_INLINE size_t ripGetNumTextureLevels(u16 width, u16 height) {
    return 29 - __builtin_clz(width > height ? width : height);
}

/**
 * @brief Get texture data size.
 * @param[in] width Texture width.
 * @param[in] height Texture height.
 * @param[in] pixelFormat Texture pixel format.
 * @param[in] level Number of levels to consider.
 * @return Texture data size.
 */
RIP_INLINE size_t ripGetTextureDataSize(u16 width, u16 height, RIPPixelFormat pixelFormat, size_t levels) {
    return ripGetTextureDataOffset(width, height, pixelFormat, ripGetNumTextureLevels(width, height));
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _RIP_TEXTURE_H */