/**
 * @file Tiling.h
 * @brief Tiling utilities.
 */
#ifndef _RIP_TILING_H
#define _RIP_TILING_H

#include <RIP/Format.h>

/**
 * @brief Applies tiling to the specified image.
 * @param[in] src Source address.
 * @param[in] dst Destination address.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @result True if tiling completed successfully, false otherwise.
 * @note When compiling for 3DS with an hardware backend, the buffers must be
 * physically contiguous and readable by the GPU (eg. FCRAM/VRAM/QTMRAM memory).
 */
RIP_EXTERN bool ripTile(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat);

/**
 * @brief Reverts tiling in the specified image.
 * @param[in] src Source address.
 * @param[in] dst Destination address.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @result True if untiling completed successfully, false otherwise.
 * @note When compiling for 3DS with an hardware backend, the buffers must be
 * physically contiguous and readable by the GPU (eg. FCRAM/VRAM/QTMRAM memory).
 */
RIP_EXTERN bool ripUntile(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat);

#endif /* _RIP_TILING_H */