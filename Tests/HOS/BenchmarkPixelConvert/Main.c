#include <3ds.h>
#include <RIP/Pixels.h>

#include <stdio.h>
#include <stdlib.h>

#define TICKS_PER_NS (SYSCLOCK_ARM11 / 1000000000.0)

#define WIDTH 1024
#define HEIGHT 1024

static inline double u64ToDouble(u64 value) {
    return (((double)(u32)(value >> 32))*0x100000000ULL+(u32)value);
}

static void benchmarkConvertPixels(const u8* src, u8* dst, RIPPixelFormat srcPixelFormat, RIPPixelFormat dstPixelFormat) {
    printf("ripConvertPixels(src=0x%08lX, dst=0x%08lX, width=%u, height=%u, srcPixelFormat=%u, dstPixelFormat=%u);\n",
        (u32)src, (u32)dst,
        WIDTH, HEIGHT,
        srcPixelFormat, dstPixelFormat);

    if (R_FAILED(svcFlushProcessDataCache(CUR_PROCESS_HANDLE, 1, 0x7000000)))
        svcBreak(USERBREAK_PANIC);

    const u64 start = svcGetSystemTick();
    const bool success = ripConvertPixels(src, dst, WIDTH, HEIGHT, srcPixelFormat, dstPixelFormat);
    const u64 end = svcGetSystemTick();

    if (!success)
        svcBreak(USERBREAK_PANIC);

    if (R_FAILED(svcFlushProcessDataCache(CUR_PROCESS_HANDLE, 1, 0x7000000)))
        svcBreak(USERBREAK_PANIC);

    const double ns = u64ToDouble(end - start) / TICKS_PER_NS;
    printf("- It took %fns (%fms)\n", ns, ns / 1000000000.0);
}

int main() {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    srand(svcGetSystemTick());

    void* srcBuffer = malloc(1024 * 1024 * 4);
    if (!srcBuffer)
        svcBreak(USERBREAK_PANIC);

    for (size_t i = 0; i < 1024 * 1024; ++i)
        ((u32*)srcBuffer)[i] = rand() * rand();

    void* dstBuffer = malloc(1024 * 1024 * 4);
    if (!dstBuffer)
        svcBreak(USERBREAK_PANIC);

    // All times are taken on O2DS, tests compiled in Release mode.

    // RGB8 -> RGBA8, 1024x1024: 0.021ms
    benchmarkConvertPixels(srcBuffer, dstBuffer, RIP_PIXELFORMAT_RGB8, RIP_PIXELFORMAT_RGBA8);

    while (aptMainLoop()) {
        hidScanInput();

        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
            break;

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    free(dstBuffer);
    free(srcBuffer);

    gfxExit();
    return 0;
}