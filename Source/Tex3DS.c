/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <RIP/Tex3DS.h>
#include <RIP/Texture.h>

#include "Allocator.h"
#include "Decompress/Include.h"

#include <string.h> // memcpy, memset

#define TEX_TYPE_2D 0
#define TEX_TYPE_CUBEMAP 1

#define TEX_FORMAT_RGBA8 0
#define TEX_FORMAT_RGB8 1
#define TEX_FORMAT_RGB5A1 2
#define TEX_FORMAT_RGB565 3
#define TEX_FORMAT_RGBA4 4
#define TEX_FORMAT_LA8 5
#define TEX_FORMAT_HILO8 6
#define TEX_FORMAT_L8 7
#define TEX_FORMAT_A8 8
#define TEX_FORMAT_LA4 9
#define TEX_FORMAT_L4 10
#define TEX_FORMAT_A4 11
#define TEX_FORMAT_ETC1 12
#define TEX_FORMAT_ETC1A4 13

typedef struct {
    void* handle;
    size_t offset;
    size_t size;
    decompressCallback read;
} Tex3DSStream;

typedef struct __attribute__((packed)) {
    u16 numSubTextures;
    u8 widthLog2 : 3;
    u8 heightLog2 : 3;
    u8 type : 1;
    u8 format;
    u8 mipmapLevels;
} RawHeader;

typedef struct {
    u16 width;
    u16 height;
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
} RawSubTexture;

RIP_INLINE u8* getTex3DSDataPtr(const RIPTex3DS* tex3ds, size_t face, size_t level) {
    u8* p = tex3ds->faces[face];
    if (p) {
        const size_t offset = ripGetTextureDataOffset(tex3ds->width, tex3ds->height, tex3ds->pixelFormat, level);
        return p + offset;
    }

    return NULL;
}

RIP_INLINE RIPPixelFormat wrapPixelFormat(u8 rawFormat) {
    switch (rawFormat) {
        case TEX_FORMAT_RGBA8:
            return RIP_PIXELFORMAT_RGBA8;
        case TEX_FORMAT_RGB8:
            return RIP_PIXELFORMAT_RGB8;
        case TEX_FORMAT_RGB5A1:
            return RIP_PIXELFORMAT_RGB5A1;
        case TEX_FORMAT_RGB565:
            return RIP_PIXELFORMAT_RGB565;
        case TEX_FORMAT_RGBA4:
            return RIP_PIXELFORMAT_RGBA4;
        case TEX_FORMAT_LA8:
            return RIP_PIXELFORMAT_LA8;
        case TEX_FORMAT_HILO8:
            return RIP_PIXELFORMAT_HILO8;
        case TEX_FORMAT_L8:
            return RIP_PIXELFORMAT_L8;
        case TEX_FORMAT_A8:
            return RIP_PIXELFORMAT_A8;
        case TEX_FORMAT_LA4:
            return RIP_PIXELFORMAT_LA4;
        case TEX_FORMAT_L4:
            return RIP_PIXELFORMAT_L4;
        case TEX_FORMAT_A4:
            return RIP_PIXELFORMAT_A4;
        case TEX_FORMAT_ETC1:
            return RIP_PIXELFORMAT_ETC1;
        case TEX_FORMAT_ETC1A4:
            return RIP_PIXELFORMAT_ETC1A4;
    }

    RIP_ASSERT(false && "Invalid pixel format!");
    return 0;
}

static ssize_t tex3dsStreamReadMem(void* userdata, void* out, size_t size) {
    Tex3DSStream* stream = (Tex3DSStream*)userdata;
    RIP_ASSERT(stream);
    
    const size_t actualSize = size < (stream->size - stream->offset) ? size : (stream->size - stream->offset);
    memcpy(out, (u8*)(stream->handle) + stream->offset, actualSize);
    stream->offset += actualSize;
    return actualSize;
}

static ssize_t tex3dsStreamReadFile(void* userdata, void* out, size_t size) {
    Tex3DSStream* stream = (Tex3DSStream*)userdata;
    RIP_ASSERT(stream);
    
    const size_t actualSize = fread(out, 1, size, (FILE*)stream->handle);
    stream->offset += actualSize;
    return actualSize;
}

static void getMemTex3DSStream(Tex3DSStream* stream, const u8* data, size_t size) {
    RIP_ASSERT(stream);

    stream->handle = (void*)data;
    stream->offset = 0;
    stream->size = size;
    stream->read = tex3dsStreamReadMem;
}

static void getFileTex3DSStream(Tex3DSStream* stream, FILE* f) {
    RIP_ASSERT(stream);

    int ret = fseek(f, 0, SEEK_END);
    RIP_ASSERT(ret == 0);

    stream->handle = (void*)f;
    stream->offset = 0;
    stream->size = ftell(f);
    stream->read = tex3dsStreamReadFile;

    ret = fseek(f, 0, SEEK_SET);
    RIP_ASSERT(ret == 0);
}

// This next function is adapted from https://github.com/devkitPro/citro3d/blob/9f21cf7b380ce6f9e01a0420f19f0763e5443ca7/source/tex3ds.c#L74
// Original license text:

/*------------------------------------------------------------------------------
 * Copyright (c) 2017
 *     Michael Theall (mtheall)
 *
 * This file is part of citro3d.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software in
 *      a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *   2. Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *   3. This notice may not be removed or altered from any source distribution.
 *----------------------------------------------------------------------------*/

static bool loadTex3DSImpl(Tex3DSStream* stream, RIPTex3DS* out) {
    RIP_ASSERT(stream);
    RIP_ASSERT(out);
    
    // Parse header.
    RawHeader header;
    stream->read(stream, &header, sizeof(RawHeader));
    if (header.type != TEX_TYPE_2D && header.type != TEX_TYPE_CUBEMAP)
        return false;

    memset(out->faces, 0, 6 * sizeof(u8*));
    out->isCubeMap = header.type == TEX_TYPE_CUBEMAP;

    out->width = (1 << (header.widthLog2 + 3));
    if (out->width < 8)
        return false;

    out->height = (1 << (header.heightLog2 + 3));
    if (out->height < 8)
        return false;

    out->pixelFormat = wrapPixelFormat(header.format);
    out->levels = header.mipmapLevels + 1; // Add one for base level.

    if (!out->isCubeMap) {
        out->numOfSubTextures = header.numSubTextures;
    } else {
        // It doesn't make sense for a cubemap to have sub-textures.
        if (header.numSubTextures)
            return false;

        out->numOfSubTextures = 0; 
    }

    // Allocate subtex info structures.
    out->subTextures = ripHeapAlloc(sizeof(RIPSubTex3DS) * out->numOfSubTextures);
    if (!out->subTextures)
        return false;
    
    // Allocate texture data.
    const size_t dataSize = ripGetTextureDataSize(out->width, out->height, out->pixelFormat, out->levels);
    const size_t allocSize = ripGetTextureDataSize(out->width, out->height, out->pixelFormat, ripGetNumTextureLevels(out->width, out->height));
    const size_t numFaces = out->isCubeMap ? 6 : 1;

    for (size_t i = 0; i < numFaces; ++i) {
        out->faces[i] = ripLinearAlloc(allocSize);
        if (!out->faces[i]) {
            ripDestroyTexture(out);
            return false;
        }
    }

    // Load sub-texture info.
    for (size_t i = 0; i < out->numOfSubTextures; ++i) {
        RawSubTexture raw;
        stream->read(stream, &raw, sizeof(RawSubTexture));

        RIPSubTex3DS* subTex = &out->subTextures[i];
        subTex->xFactor = raw.left / 1024.0f;
        subTex->yFactor = 1.0f - (raw.top / 1024.0f);
        subTex->width = raw.width;
        subTex->height = raw.height;
    }

    // Load texture data.
    decompressIOVec iov[6];

    for (size_t i = 0; i < numFaces; ++i) {
        u8* p = getTex3DSDataPtr(out, i, 0);
        RIP_ASSERT(p);
        iov[i].data = p;
        iov[i].size = dataSize;
    }

    RIP_ASSERT(decompressV(iov, numFaces, stream->read, (void*)stream, 0));
    return true;
}

bool ripLoadTex3DS(const u8* data, size_t size, RIPTex3DS* out) {
    if (!data || !out)
        return false;

    Tex3DSStream stream;
    getMemTex3DSStream(&stream, data, size);
    return loadTex3DSImpl(&stream, out);
}

bool ripLoadTex3DSFromFile(FILE* f, RIPTex3DS* out) {
    if (f == NULL || !out)
        return false;

    Tex3DSStream stream;
    getFileTex3DSStream(&stream, f);
    return loadTex3DSImpl(&stream, out);
}

bool ripLoadTex3DSFromPath(const char* path, RIPTex3DS* out) {
    if (!path || !out)
        return false;

    FILE* f = fopen(path, "rb");
    if (f == NULL)
        return false;

    const bool ret = ripLoadTex3DSFromFile(f, out);
    fclose(f);
    return ret;
}

void ripDestroyTex3DS(RIPTex3DS* tex3ds) {
    if (!tex3ds)
        return;

    const size_t numFaces = tex3ds->isCubeMap ? 6 : 1;
    for (size_t i = 0; i < numFaces; ++i)
        ripLinearFree(tex3ds->faces[i]);

    ripHeapFree(tex3ds->subTextures);
}

size_t ripGetTex3DSSize(const RIPTex3DS* tex3ds, size_t level) {
    if (tex3ds && level < tex3ds->levels)
        return (((tex3ds->width >> level) * (tex3ds->height >> level) * ripGetPixelFormatBPP(tex3ds->pixelFormat)) >> 3);

    return 0;
}

const u8* ripGetTex3DSData(const RIPTex3DS* tex3ds, size_t face, size_t level) {
    if (tex3ds && (level < tex3ds->levels)) {
        if (face < (tex3ds->isCubeMap ? 6 : 1))
            return getTex3DSDataPtr(tex3ds, face, level);
    }

    return NULL;
}

const u8* ripGetSubTex3DSData(const RIPTex3DS* tex3ds, const RIPSubTex3DS* subTex3DS, size_t level) {
    // TODO
    return NULL;
}