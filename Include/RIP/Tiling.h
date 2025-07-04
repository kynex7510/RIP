/**
 * @file Tiling.h
 * @brief Tiling utilities.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _RIP_TILING_H
#define _RIP_TILING_H

#include <RIP/Format.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Applies tiling to the specified image.
 * @param[in] src Source address.
 * @param[in] dst Destination address.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @result True if tiling completed successfully, false otherwise.
 * @note The buffers must be in linear memory/QTMRAM.
 */
bool ripTile(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat);

/**
 * @brief Reverts tiling in the specified image.
 * @param[in] src Source address.
 * @param[in] dst Destination address.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @result True if untiling completed successfully, false otherwise.
 * @note The buffers must be in linear memory/QTMRAM.
 */
bool ripUntile(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _RIP_TILING_H */