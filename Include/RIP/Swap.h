/**
 * @file Convert.h
 * @brief Pixel byte swapping utilities.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _RIP_SWAP_H
#define _RIP_SWAP_H

#include <RIP/Format.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Swap image pixels bytes. Both buffers can start at the same address (in-place operation).
 * @param[in] src Source address.
 * @param[in] dst Destination address.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @param[in] flip Whether to flip the image vertically.
 * @note The buffers must not be in VRAM.
 */
void ripSwapBytes(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _RIP_SWAP_H */