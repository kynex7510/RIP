/**
 * @file C3D.h
 * @brief Citro3D utilities.
 */
#ifndef _RIP_C3D_H
#define _RIP_C3D_H

#include <RIP/Defs.h>

#if RIP_BACKEND == RIP_BACKEND_CITRO3D

#include <citro3d.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Convert raw image data, then load it into the specified citro3d texture object.
 * @param[in, out] tex Texture object.
 * @param[in] data Raw texture data.
 * @param[in] face Specifies texture face.
 * @param[in] level Specifies mipmap level.
 * @result True if the conversion completed successfully, false otherwise.
 * @note The buffer must be in linear memory/QTMRAM.
 */
bool ripConvertAndLoadC3DTexImage(C3D_Tex* tex, const void* data, GPU_TEXFACE face, int level);

/**
 * @brief Like @ripConvertAndLoadC3DTexImage, but the result is applied to the buffer.
 * @param[in, out] tex Texture object.
 * @param[in, out] data Raw texture data.
 * @param[in] face Specifies texture face.
 * @param[in] level Specifies mipmap level.
 * @result True if the conversion completed successfully, false otherwise.
 * @note The buffer must be in linear memory/QTMRAM.
 */
bool ripConvertInPlaceAndLoadC3DTexImage(C3D_Tex* tex, void* data, GPU_TEXFACE face, int level);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // RIP_BACKEND == RIP_BACKEND_CITRO3D

#endif /* _RIP_C3D_H */