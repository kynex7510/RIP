/**
 * @file Tex3DS.h
 * @brief Tex3DS texture reader.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _RIP_TEX3DS_H
#define _RIP_TEX3DS_H

#include <RIP/Format.h>
#include <stdio.h>

/// @brief Sub-tex3DS object.
typedef struct {
    float xFactor; ///< Sub-texture X factor.
    float yFactor; ///< Sub-texture Y factor.
    u16 width;     ///< Sub-texture width.
    u16 height;    ///< Sub-texture height.
} RIPSubTex3DS;

/// @brief Tex3DS object.
typedef struct {
    u8* faces[6];               ///< Internal texture data.
    bool isCubeMap;             ///< Whether this texture is a cube map.
    u16 width;                  ///< Texture width.
    u16 height;                 ///< Texture height.
    RIPPixelFormat pixelFormat; ///< Pixel format.
    size_t levels;              ///< MipMap levels.
    size_t numOfSubTextures;    ///< Number of sub-textures.
    RIPSubTex3DS* subTextures;  ///< Sub-texture info.
} RIPTex3DS;

bool ripLoadTex3DS(const u8* data, size_t size, RIPTex3DS* out);
bool ripLoadTex3DSFromFile(FILE* f, RIPTex3DS* out);
bool ripLoadTex3DSFromPath(const char* path, RIPTex3DS* out);
void ripDestroyTex3DS(RIPTex3DS* tex3ds);

size_t ripGetTex3DSSize(const RIPTex3DS* tex3ds, size_t level);
const u8* ripGetTex3DSData(const RIPTex3DS* tex3ds, size_t face, size_t level);
const u8* ripGetSubTex3DSData(const RIPTex3DS* tex3ds, const RIPSubTex3DS* subTex3ds, size_t level);

#endif /* _RIP_TEX3DS_H */