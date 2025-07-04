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

/// @brief Sub-texture object.
typedef struct {
    float xFactor; ///< Sub-texture X factor.
    float yFactor; ///< Sub-texture Y factor.
    u16 width;     ///< Sub-texture width.
    u16 height;    ///< Sub-texture height.
} RIPSubTexture;

/// @brief Texture object.
typedef struct {
    u8* faces[6];               ///< Internal texture data.
    bool isCubeMap;             ///< Whether this texture is a cube map.
    u16 width;                  ///< Texture width.
    u16 height;                 ///< Texture height.
    RIPPixelFormat pixelFormat; ///< Pixel format.
    size_t levels;              ///< MipMap levels.
    size_t numOfSubTextures;    ///< Number of sub-textures.
    RIPSubTexture* subTextures; ///< Sub-texture info.
} RIPTexture;

bool ripLoadTexture(const u8* data, size_t size, RIPTexture* out);
bool ripLoadTextureFromFile(FILE* f, RIPTexture* out);
bool ripLoadTextureFromPath(const char* path, RIPTexture* out);
void ripDestroyTexture(RIPTexture* tex);

size_t ripGetTextureSize(const RIPTexture* tex, size_t level);
const u8* ripGetTextureData(const RIPTexture* tex, size_t face, size_t level);
const u8* ripGetSubTextureData(const RIPTexture* tex, const RIPSubTexture* subTex, size_t level);

bool ripIsTextureCompressed(RIPPixelFormat pixelFormat);

#endif /* _RIP_TEX3DS_H */