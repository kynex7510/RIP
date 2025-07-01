#include <RIP/Swap.h>
#include <RIP/Texture.h>

#include <string.h> // memcpy

static u8 g_TmpBuffer[RIP_MAX_TEX_DIMS * 4];

extern void rip_memcpy(u8* dst, const u8* src, size_t size);
extern void rip_swapBytesImpl(u8* dst, const u8* src, size_t size, size_t bytesPerPixel);

void ripSwapBytes(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    RIP_ASSERT(width <= RIP_MAX_TEX_DIMS);
    RIP_ASSERT(height <= RIP_MAX_TEX_DIMS);

    if (!width || !height)
        return;

    const size_t bytesPerPixel = ripGetPixelFormatBPP(pixelFormat) >> 3;
    RIP_ASSERT(bytesPerPixel > 0);

    if (flip) {
        const size_t rowSize = width * bytesPerPixel;
        
        for (size_t r = 0; r < (height >> 1); ++r) {
            const size_t index0 = r * rowSize;
            const size_t index1 = (height - r - 1) * rowSize;

            rip_memcpy(g_TmpBuffer, &src[index0], rowSize);
            rip_swapBytesImpl(&dst[index0], &src[index1], rowSize, bytesPerPixel);
            rip_swapBytesImpl(&dst[index1], g_TmpBuffer, rowSize, bytesPerPixel);
        }
    } else {
        const size_t size = width * height * bytesPerPixel;
        rip_swapBytesImpl(dst, src, size, bytesPerPixel);
    }
}