/**
 * @file Convert.h
 * @brief Conversion utilities.
 */
#ifndef _RIP_CONVERT_H
#define _RIP_CONVERT_H

#include <RIP/Format.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Convert raw image data into a format that the 3DS GPU understands.
 * @param[in] src Source address.
 * @param[in] dst Destination address.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @param[in] flip Whether to flip the image vertically.
 * @result True if the conversion completed successfully, false otherwise.
 * @note The buffers must be in linear memory/QTMRAM.
 */
bool ripConvertToNative(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip);

/**
 * @brief This is the opposite of @ripConvertToNative.
 * @param[in] src Source address.
 * @param[in] dst Destination address.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @param[in] flip Whether to flip the image vertically.
 * @result True if the conversion completed successfully, false otherwise.
 * @note The buffers must be in linear memory/QTMRAM.
 */
bool ripConvertFromNative(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip);

/**
 * @brief Like @ripConvertToNative, but the result is applied on the same buffer.
 * @param[in, out] p Image buffer.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @param[in] flip Whether to flip the image vertically.
 * @result True if the conversion completed successfully, false otherwise.
 * @note The buffer must be in linear memory/QTMRAM.
 */
bool ripConvertInPlaceToNative(u8* p, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip);

/**
 * @brief Like @ripConvertFromNative, but the result is applied on the same buffer.
 * @param[in, out] p Image buffer.
 * @param[in] width Image width.
 * @param[in] height Image height.
 * @param[in] pixelFormat Image pixel format.
 * @param[in] flip Whether to flip the image vertically.
 * @result True if the conversion completed successfully, false otherwise.
 * @note The buffer must be in linear memory/QTMRAM.
 */
bool ripConvertInPlaceFromNative(u8* p, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _RIP_CONVERT_H */