#include <3ds.h>
#include <RIP/Pixels.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void flip16(const u8* src, u8* dst, size_t size) {
    for (size_t i = 0; i < size; i += 2) {
        dst[i] = src[i + 1];
        dst[i + 1] = src[i];
    }
}

static void flip24(const u8* src, u8* dst, size_t size) {
    for (size_t i = 0; i < size; i += 3) {
        dst[i] = src[i + 2];
        dst[i + 1] = src[i + 1];
        dst[i + 2] = src[i];
    }
}

static void flip32(const u8* src, u8* dst, size_t size) {
    for (size_t i = 0; i < size; i += 4) {
        dst[i] = src[i + 3];
        dst[i + 1] = src[i + 2];
        dst[i + 2] = src[i + 1];
        dst[i + 3] = src[i];
    }
}

int main() {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    void* buffer = malloc(1024 * 1024 * 4);
     if (!buffer)
        svcBreak(USERBREAK_PANIC);

    for (size_t i = 0; i < 1024 * 1024; ++i)
        ((u32*)buffer)[i] = rand() * rand();

    // 16bpp

    void* out16 = malloc(1024 * 1024 * 2);
    if (!out16)
        svcBreak(USERBREAK_PANIC);

    void* out16Expected = malloc(1024 * 1024 * 2);
    if (!out16Expected)
        svcBreak(USERBREAK_PANIC);

    if (!ripSwapPixelBytes(buffer, out16, 1024, 1024, RIP_PIXELFORMAT_RGBA4, false))
        svcBreak(USERBREAK_PANIC);

    flip16(buffer, out16Expected, 1024 * 1024 * 2);

    if (memcmp(out16, out16Expected, 1024 * 1024 * 2)) {
        printf("16BPP output mismatch!\n");
    } else {
        printf("16BPP output verified!\n");
    }

    free(out16);
    free(out16Expected);

    // 24bpp

    void* out24 = malloc(1024 * 1024 * 3);
    if (!out24)
        svcBreak(USERBREAK_PANIC);

    void* out24Expected = malloc(1024 * 1024 * 3);
    if (!out24Expected)
        svcBreak(USERBREAK_PANIC);

    if (!ripSwapPixelBytes(buffer, out24, 1024, 1024, RIP_PIXELFORMAT_RGB8, false))
        svcBreak(USERBREAK_PANIC);

    flip24(buffer, out24Expected, 1024 * 1024 * 3);

    if (memcmp(out24, out24Expected, 1024 * 1024 * 3)) {
        printf("24BPP output mismatch!\n");
    } else {
        printf("24BPP output verified!\n");
    }

    free(out24);
    free(out24Expected);

    // 32bpp

    void* out32 = malloc(1024 * 1024 * 4);
    if (!out32)
        svcBreak(USERBREAK_PANIC);

    void* out32Expected = malloc(1024 * 1024 * 4);
    if (!out32Expected)
        svcBreak(USERBREAK_PANIC);

    if (!ripSwapPixelBytes(buffer, out32, 1024, 1024, RIP_PIXELFORMAT_RGBA8, false))
        svcBreak(USERBREAK_PANIC);

    flip32(buffer, out32Expected, 1024 * 1024 * 4);

    if (memcmp(out32, out32Expected, 1024 * 1024 * 4)) {
        printf("32BPP output mismatch!\n");
    } else {
        printf("32BPP output verified!\n");
    }

    free(out32);
    free(out32Expected);

    while (aptMainLoop()) {
        hidScanInput();

        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
            break;

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    free(buffer);

    gfxExit();
    return 0;
}