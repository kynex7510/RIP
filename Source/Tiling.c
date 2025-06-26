#include <RIP/Tiling.h>

#if RIP_BACKEND == RIP_BACKEND_KYGX
#include <GX/Wrappers/DisplayTransfer.h>
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU
#include <3ds.h>
#elif RIP_BACKEND == RIP_BACKEND_CITRO3D
#include <citro3d.h>
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS
#include <debug.h>
#include <drivers/cache.h>
#include <drivers/gfx.h>
#include <arm11/drivers/gx.h>
#endif

#include <string.h> // memcpy

RIP_INLINE void handleTile(const u8* src, u8* dst, size_t width, size_t bpp, bool makeTiled) {
    // https://en.wikipedia.org/wiki/Moser-de_Bruijn_sequence
    static const u8 Z_ORDER_TABLE[8] = { 0x00, 0x01, 0x04, 0x05, 0x10, 0x11, 0x14, 0x15 };

    for (size_t r = 0; r < 8; ++r) {
        // Optimization: pick 2 pixels at time since they're always contiguous.
        for (size_t c = 0; c < 8; c += 2) {
            const size_t linearIndex = (((r * width) + c) * bpp) >> 3;
            const size_t tiledIndex = ((Z_ORDER_TABLE[c] + (Z_ORDER_TABLE[r] << 1)) * bpp) >> 3;
            memcpy(&dst[makeTiled ? tiledIndex : linearIndex], &src[makeTiled ? linearIndex : tiledIndex], bpp >> 2);
        }
    }
}

RIP_INLINE void swTiling(const u8* src, u8* dst, size_t width, size_t height, size_t bpp, bool makeTiled) {
    size_t tiledIndex = 0;
    for (size_t r = 0; r < height; r += 8) {
        for (size_t c = 0; c < width; c += 8) {
            const size_t linearIndex = (((r * width) + c) * bpp) >> 3;
            handleTile(&src[makeTiled ? linearIndex : tiledIndex], &dst[makeTiled ? tiledIndex : linearIndex], width, bpp, makeTiled);
            tiledIndex += (64 * bpp) >> 3;
        }
    }
}

RIP_INLINE void hwTiling(const u8* src, u8* dst, size_t width, size_t height, RIPPixelFormat pixelFormat, bool makeTiled) {
#if RIP_BACKEND == RIP_BACKEND_KYGX // KYGX
    u32 fmt;

    switch (pixelFormat) {
        case RIP_PIXELFORMAT_RGBA8:
            fmt = KYGX_DISPLAYTRANSFER_FMT_RGBA8;
            break;
        case RIP_PIXELFORMAT_RGB8:
            fmt = KYGX_DISPLAYTRANSFER_FMT_RGB8;
            break;
        case RIP_PIXELFORMAT_RGB565:
            fmt = KYGX_DISPLAYTRANSFER_FMT_RGB565;
            break;
        case RIP_PIXELFORMAT_RGB5A1:
            fmt = KYGX_DISPLAYTRANSFER_FMT_RGB5A1;
            break;
        case RIP_PIXELFORMAT_RGBA4:
            fmt = KYGX_DISPLAYTRANSFER_FMT_RGBA4;
            break;
        default:
            KYGX_UNREACHABLE("Invalid pixel format!");
    }

    GXDisplayTransferFlags transferFlags;
    transferFlags.mode = makeTiled ? KYGX_DISPLAYTRANSFER_MODE_L2T : KYGX_DISPLAYTRANSFER_MODE_T2L;
    transferFlags.srcFmt = fmt;
    transferFlags.dstFmt = fmt;
    transferFlags.downscale = KYGX_DISPLAYTRANSFER_DOWNSCALE_NONE;
    transferFlags.verticalFlip = false;
    transferFlags.blockMode32 = false;

    kygxSyncDisplayTransferChecked(src, dst, width, height, width, height, &transferFlags);
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU || RIP_BACKEND == RIP_BACKEND_CITRO3D // libctru || citro3d
    GX_TRANSFER_FORMAT fmt;

    switch (pixelFormat) {
        case RIP_PIXELFORMAT_RGBA8:
            fmt = GX_TRANSFER_FMT_RGBA8;
            break;
        case RIP_PIXELFORMAT_RGB8:
            fmt = GX_TRANSFER_FMT_RGB8;
            break;
        case RIP_PIXELFORMAT_RGB565:
            fmt = GX_TRANSFER_FMT_RGB565;
            break;
        case RIP_PIXELFORMAT_RGB5A1:
            fmt = GX_TRANSFER_FMT_RGB5A1;
            break;
        case RIP_PIXELFORMAT_RGBA4:
            fmt = GX_TRANSFER_FMT_RGBA4;
            break;
        default:
            svcBreak(USERBREAK_PANIC);
    }

    u32 flags = GX_TRANSFER_OUT_TILED(makeTiled ? 1 : 0);
    flags |= GX_TRANSFER_IN_FORMAT(fmt);
    flags |= GX_TRANSFER_OUT_FORMAT(fmt);

#if RIP_BACKEND == RIP_BACKEND_LIBCTRU // libctru
    GX_DisplayTransfer((u32*)src, GX_BUFFER_DIM(width, height), (u32*)dst, GX_BUFFER_DIM(width, height), flags);
    gspWaitForPPF();
#elif RIP_BACKEND == RIP_BACKEND_CITRO3D // citro3d
    C3D_SyncDisplayTransfer((u32*)src, GX_BUFFER_DIM(width, height), (u32*)dst, GX_BUFFER_DIM(width, height), flags);
#endif
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS // libn3ds
    u32 fmt;

    switch (pixelFormat) {
        case RIP_PIXELFORMAT_RGBA8:
            fmt = GX_ABGR8;
            break;
        case RIP_PIXELFORMAT_RGB8:
            fmt = GX_BGR8;
            break;
        case RIP_PIXELFORMAT_RGB565:
            fmt = GX_BGR565;
            break;
        case RIP_PIXELFORMAT_RGB5A1:
            fmt = GX_A1BGR5;
            break;
        case RIP_PIXELFORMAT_RGBA4:
            fmt = GX_ABGR4;
            break;
        default:
            panicMsg("Invalid pixel format!");
    }

    u32 flags = makeTiled ? PPF_OUT_TILED : PPF_OUT_LINEAR;
    flags |= PPF_I_FMT(fmt);
    flags |= PPF_O_FMT(fmt);

    GX_displayTransfer((u32*)src, PPF_DIM(width, height), (u32*)dst, PPF_DIM(width, height), flags);
    GFX_waitForPPF();
#endif
}

RIP_INLINE bool canUseHW(u16 width, u16 height, RIPPixelFormat pixelFormat) {
    if (RIP_BACKEND != RIP_BACKEND_KYGX &&
        RIP_BACKEND != RIP_BACKEND_LIBCTRU &&
        RIP_BACKEND != RIP_BACKEND_LIBN3DS)
        return false;

    // Transfer engine doesn't support anything lower than 64x64.
    if (width < 64 || height < 64)
        return false;

    // Only certain formats are supported.
    switch (pixelFormat) {
        case RIP_PIXELFORMAT_RGBA8:
        case RIP_PIXELFORMAT_RGB8:
        case RIP_PIXELFORMAT_RGB565:
        case RIP_PIXELFORMAT_RGB5A1:
        case RIP_PIXELFORMAT_RGBA4:
            return true;
        default:
            return false;
    }
}

static bool tilingImpl(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat, bool makeTiled) {
    if (width < 8 || height < 8 || (width & 7) || (height & 7))
        return false;

    // Use the hardware if possible.
    if (canUseHW(width, height, pixelFormat)) {
        hwTiling(src, dst, width, height, pixelFormat, makeTiled);
    } else {
        swTiling(src, dst, width, height, ripGetPixelFormatBPP(pixelFormat), makeTiled);
    }

    return true;
}

bool ripTile(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat) {
    return tilingImpl(src, dst, width, height, pixelFormat,  true);
}

bool ripUntile(const u8* src, u8* dst, u16 width, u16 height, RIPPixelFormat pixelFormat) {
    return tilingImpl(src, dst, width, height, pixelFormat, false);
}