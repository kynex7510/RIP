#include <RIP/Tex3DS.h>
#include <RIP/Texture.h>

#include "Allocator.h"

#include <string.h> // memcpy, memset

#if RIP_BACKEND == RIP_BACKEND_KYGX
#ifdef KYGX_BAREMETAL
#define COMPRESSION_USE_LIBN3DS
#else
#define COMPRESSION_USE_LIBCTRU
#endif // KYGX_BAREMETAL
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU || RIP_BACKEND_CITRO3D
#define COMPRESSION_USE_LIBCTRU
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS
#define COMPRESSION_USE_LIBN3DS
#endif

#ifdef COMPRESSION_USE_LIBCTRU
#include <3ds.h>
#elif COMPRESSION_USE_LIBN3DS
// TODO
#error "Not implemented!"
#endif

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
} TexStream;

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

RIP_INLINE u8* getTexDataPtr(const RIPTexture* tex, size_t face, size_t level) {
    u8* p = tex->faces[face];
    if (p) {
        const size_t offset = ripGetTextureDataOffset(tex->width, tex->height, tex->pixelFormat, level);
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

    RIP_ASSERT(false);
    return -1;
}

static ssize_t texStreamReadMem(void* userdata, void* out, size_t size) {
    TexStream* stream = (TexStream*)userdata;
    RIP_ASSERT(stream);
    
    const size_t actualSize = size < (stream->size - stream->offset) ? size : (stream->size - stream->offset);
    memcpy(out, (u8*)(stream->handle) + stream->offset, actualSize);
    stream->offset += actualSize;
    return actualSize;
}

static ssize_t texStreamReadFile(void* userdata, void* out, size_t size) {
    TexStream* stream = (TexStream*)userdata;
    RIP_ASSERT(stream);
    
    const size_t actualSize = fread(out, 1, size, (FILE*)stream->handle);
    stream->offset += actualSize;
    return actualSize;
}

static void getMemTexStream(TexStream* stream, const u8* data, size_t size) {
    RIP_ASSERT(stream);

    stream->handle = (void*)data;
    stream->offset = 0;
    stream->size = size;
    stream->read = texStreamReadMem;
}

static void getFileTexStream(TexStream* stream, FILE* f) {
    RIP_ASSERT(stream);

    int ret = fseek(f, 0, SEEK_END);
    RIP_ASSERT(ret == 0);

    stream->handle = (void*)f;
    stream->offset = 0;
    stream->size = ftell(f);
    stream->read = texStreamReadFile;

    ret = fseek(f, 0, SEEK_SET);
    RIP_ASSERT(ret == 0);
}

static bool loadTextureImpl(TexStream* stream, RIPTexture* out) {
    RIP_ASSERT(stream);
    RIP_ASSERT(out);
    
    // Parse header.
    RawHeader header;
    stream->read(stream, &header, sizeof(RawHeader));
    RIP_ASSERT(header.type == TEX_TYPE_2D || header.type == TEX_TYPE_CUBEMAP);

    memset(out->faces, 0, 6 * sizeof(u8*));
    out->isCubeMap = header.type == TEX_TYPE_CUBEMAP;

    out->width = (1 << (header.widthLog2 + 3));
    RIP_ASSERT(out->width >= 8);

    out->height = (1 << (header.heightLog2 + 3));
    RIP_ASSERT(out->height >= 8);

    out->pixelFormat = wrapPixelFormat(header.format);
    out->levels = header.mipmapLevels + 1; // Add one for base level.

    if (!out->isCubeMap) {
        out->numOfSubTextures = header.numSubTextures;
    } else {
        // It doesn't make sense for a cubemap to have sub-textures.
        RIP_ASSERT(header.numSubTextures == 0);
        out->numOfSubTextures = 0; 
    }

    // Allocate sub textures.
    out->subTextures = ripHeapAlloc(sizeof(RIPSubTexture) * out->numOfSubTextures);
    if (!out->subTextures)
        return false;
    
    // Allocate texture data.
    const size_t dataSize = ripGetTextureDataSize(out->width, out->height, out->pixelFormat, out->levels);
    const size_t allocSize = ripGetTextureDataSize(out->width, out->height, out->pixelFormat, RIP_MAX_TEX_LEVELS);
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

        RIPSubTexture* subTex = &out->subTextures[i];
        subTex->xFactor = raw.left / 1024.0f;
        subTex->yFactor = 1.0f - (raw.top / 1024.0f);
        subTex->width = raw.width;
        subTex->height = raw.height;
    }

    // Load texture data.
    decompressIOVec iov[6];

    for (size_t i = 0; i < numFaces; ++i) {
        u8* p = getTexDataPtr(out, i, 0);
        RIP_ASSERT(p);
        iov[i].data = p;
        iov[i].size = dataSize;
    }

    RIP_ASSERT(decompressV(iov, numFaces, stream->read, (void*)stream, 0));
    return true;
}

bool ripLoadTexture(const u8* data, size_t size, RIPTexture* out) {
    if (!data || !out)
        return false;

    TexStream stream;
    getMemTexStream(&stream, data, size);
    return loadTextureImpl(&stream, out);
}

bool ripLoadTextureFromFile(FILE* f, RIPTexture* out) {
    if (f == NULL || !out)
        return false;

    TexStream stream;
    getFileTexStream(&stream, f);
    return loadTextureImpl(&stream, out);
}

bool ripLoadTextureFromPath(const char* path, RIPTexture* out) {
    if (!path || !out)
        return false;

    FILE* f = fopen(path, "rb");
    if (f == NULL)
        return false;

    const bool ret = ripLoadTextureFromFile(f, out);
    fclose(f);
    return ret;
}

void ripDestroyTexture(RIPTexture* tex) {
    if (!tex)
        return;

    const size_t numFaces = tex->isCubeMap ? 6 : 1;
    for (size_t i = 0; i < numFaces; ++i)
        ripLinearFree(tex->faces[i]);

    ripHeapFree(tex->subTextures);
}

size_t ripGetTextureSize(const RIPTexture* tex, size_t level) {
    if (tex && level < tex->levels)
        return (((tex->width >> level) * (tex->height >> level) * ripGetPixelFormatBPP(tex->pixelFormat)) >> 3);

    return 0;
}

const u8* ripGetTextureData(const RIPTexture* tex, size_t face, size_t level) {
    if (tex && (level < tex->levels)) {
        if (face < (tex->isCubeMap ? 6 : 1))
            return getTexDataPtr(tex, face, level);
    }

    return NULL;
}

const u8* ripGetSubTextureData(const RIPTexture* tex, const RIPSubTexture* subTex, size_t level) {
    // TODO
    return NULL;
}

bool ripIsTextureCompressed(RIPPixelFormat pixelFormat) {
    if (pixelFormat)
        return pixelFormat == RIP_PIXELFORMAT_ETC1 || pixelFormat == RIP_PIXELFORMAT_ETC1A4;

    return false;
}