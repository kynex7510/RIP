/**
 * @file Convert.h
 * @brief Pixel byte swapping utilities.
 */
#ifndef _RIP_SWAP_H
#define _RIP_SWAP_H

#include <RIP/Format.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Swap image pixels bytes.
 * @param[in] src Source address.
 * @param[in] dst Destination address.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @param[in] flip Whether to flip the image vertically.
 * @note This function is always implemented in software, as such when compiling
 * for HOS none of the buffers can be in VRAM. 
 */
void ripSwapBytes(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip);

/**
 * @brief Like @ripSwapBytes, but the result is applied on the same buffer.
 * @param[in, out] p Image buffer.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @note This function is always implemented in software, as such when compiling
 * for HOS none of the buffers can be in VRAM. 
 */
void ripSwapBytesInPlace(u8* p, u16 width, u16 height, RIPPixelFormat pixelFormat);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _RIP_SWAP_H */