/**
 * @file Convert.h
 * @brief Conversion utilities.
 */
#ifndef _RIP_CONVERT_H
#define _RIP_CONVERT_H

#include <RIP/Format.h>

/**
 * @brief Convert raw image data into a format that the 3DS GPU understands.
 * @param[in] src Source address.
 * @param[in] dst Destination address.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @param[in] flip Whether to flip the image vertically.
 * @result True if the conversion completed successfully, false otherwise.
 * @note When compiling for 3DS with an hardware backend, the destination buffer
 * must be physically contiguous and readable by the GPU (eg. FCRAM/VRAM/QTMRAM memory).
 */
RIP_EXTERN bool ripConvertToNative(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip);

/**
 * @brief This is the opposite of @ripConvertToNative.
 * @param[in] src Source address.
 * @param[in] dst Destination address.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @param[in] flip Whether to flip the image vertically.
 * @result True if the conversion completed successfully, false otherwise.
 * @note When compiling for 3DS with an hardware backend, the source buffer must
 * be physically contigous and readable by the GPU (eg. FCRAM/VRAM/QTMRAM memory).
 */
RIP_EXTERN bool ripConvertFromNative(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip);

/**
 * @brief Like @ripConvertToNative, but the result is applied on the same buffer.
 * @param[in, out] p Image buffer.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @result True if the conversion completed successfully, false otherwise.
 */
RIP_EXTERN bool ripConvertInPlaceToNative(u8* p, u16 width, u16 height, RIPPixelFormat pixelFormat);

/**
 * @brief Like @ripConvertFromNative, but the result is applied on the same buffer.
 * @param[in, out] p Image buffer.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @result True if the conversion completed successfully, false otherwise.
 */
RIP_EXTERN bool ripConvertInPlaceFromNative(u8* p, u16 width, u16 height, RIPPixelFormat pixelFormat);

#endif /* _RIP_CONVERT_H */