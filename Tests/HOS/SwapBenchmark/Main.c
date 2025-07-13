#include <3ds.h>
#include <RIP/Swap.h>

#include <stdio.h>
#include <stdlib.h>

#define TICKS_PER_NS (SYSCLOCK_ARM11 / 1000000000.0)

static inline double u64_to_double(u64 value) {
    return (((double)(u32)(value >> 32))*0x100000000ULL+(u32)value);
}

static void benchmarkSwapBytes(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool flip) {
    printf("ripSwapBytes(src=0x%08lX, dst=0x%08lX, width=%u, height=%u, pixelFormat=%u, flip=%s);\n",
        (u32)src, (u32)dst,
        width, height,
        pixelFormat,
        (flip ? "true" : "false"));

    if (R_FAILED(svcFlushProcessDataCache(CUR_PROCESS_HANDLE, 1, 0x7000000)))
        svcBreak(USERBREAK_PANIC);

    const u64 start = svcGetSystemTick();
    ripSwapBytes(src, dst, width, height, pixelFormat, flip);
    const u64 end = svcGetSystemTick();

    if (R_FAILED(svcFlushProcessDataCache(CUR_PROCESS_HANDLE, 1, 0x7000000)))
        svcBreak(USERBREAK_PANIC);

    const double ns = u64_to_double(end - start) / TICKS_PER_NS;
    printf("- It took %fns (%fms)\n", ns, ns / 1000000000.0);
}

static void benchmarkSwapBytesBothFlip(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat) {
    benchmarkSwapBytes(src, dst, width, height, pixelFormat, false);
    benchmarkSwapBytes(src, dst, width, height, pixelFormat, true);
}

int main() {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    void* buffer = malloc(1024 * 1024 * 4);
    if (!buffer)
        svcBreak(USERBREAK_PANIC);

    for (size_t i = 0; i < 1024 * 1024; ++i)
        ((u32*)buffer)[i] = rand() * rand();

    // 1024x1024 RGB565 (16bpp)
    benchmarkSwapBytesBothFlip(buffer, buffer, 1024, 1024, RIP_PIXELFORMAT_RGB565);

    // 1024x1024 RGB8 (24bpp)
    benchmarkSwapBytesBothFlip(buffer, buffer, 1024, 1024, RIP_PIXELFORMAT_RGB8);

    // 1024x1024 RGBA8 (32bpp)
    benchmarkSwapBytesBothFlip(buffer, buffer, 1024, 1024, RIP_PIXELFORMAT_RGBA8);

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