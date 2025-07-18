#include <3ds.h>
#include <RIP/Pixels.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define WIDTH 1024
#define HEIGHT 1024
#define NUM_PIXELS WIDTH * HEIGHT

static void writeFile(const char* path, const void* data, size_t size) {
    FILE* f = fopen(path, "wb");
    if (!f)
        svcBreak(USERBREAK_PANIC);

    fwrite(data, size, 1, f);
    fclose(f);
}

// To RGB8

static void checkRGB8ToRGBA8(const u8* srcBuffer) {
    void* dst0 = malloc(NUM_PIXELS * 4);
    if (!dst0)
        svcBreak(USERBREAK_PANIC);

    for (size_t i = 0; i < NUM_PIXELS; ++i) {
        const u8* srcp = &((const u8*)srcBuffer)[i * 3];
        u8* dstp = &((u8*)dst0)[i * 4];
        dstp[0] = srcp[0];
        dstp[1] = srcp[1];
        dstp[2] = srcp[2];
        dstp[3] = 0xFF;
    }

    void* dst1 = malloc(NUM_PIXELS * 4);
    if (!dst1)
        svcBreak(USERBREAK_PANIC);

    const bool success = ripConvertPixels(srcBuffer, dst1, WIDTH, HEIGHT, RIP_PIXELFORMAT_RGB8, RIP_PIXELFORMAT_RGBA8);
    if (!success)
        svcBreak(USERBREAK_PANIC);

    if (memcmp(dst0, dst1, NUM_PIXELS * 4)) {
        printf("RGB8 -> RGBA8: mismatch!\n");
    } else {
        printf("RGB8 -> RGBA8: verified!\n");
    }

    free(dst0);
    free(dst1);
}

int main() {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    srand(svcGetSystemTick());

    void* srcBuffer = malloc(NUM_PIXELS * 4);
    if (!srcBuffer)
        svcBreak(USERBREAK_PANIC);

    for (size_t i = 0; i < NUM_PIXELS; ++i)
        ((u32*)srcBuffer)[i] = rand() + rand();

    checkRGB8ToRGBA8(srcBuffer);

    while (aptMainLoop()) {
        hidScanInput();

        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
            break;

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    free(srcBuffer);

    gfxExit();
    return 0;
}