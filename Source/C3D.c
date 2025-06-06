#include <RIP/C3D.h>
#include <RIP/Convert.h>

#if RIP_BACKEND == RIP_BACKEND_CITRO3D

static RIPPixelFormat cvtPF(GPU_TEXCOLOR fmt) {
    switch (fmt) {
        case GPU_RGBA8:
            return RIP_PIXELFORMAT_RGBA8;
        case GPU_RGB8:
            return RIP_PIXELFORMAT_RGB8;
        case GPU_RGBA5551:
            return RIP_PIXELFORMAT_RGB5A1;
        case GPU_RGB565:
            return RIP_PIXELFORMAT_RGB565;
        case GPU_RGBA4:
            return RIP_PIXELFORMAT_RGBA4;
        case GPU_LA8:
            return RIP_PIXELFORMAT_LA8;
        case GPU_HILO8:
            return RIP_PIXELFORMAT_HILO8;
        case GPU_L8:
            return RIP_PIXELFORMAT_L8;
        case GPU_A8:
            return RIP_PIXELFORMAT_A8;
        case GPU_LA4:
            return RIP_PIXELFORMAT_LA4;
        case GPU_L4:
            return RIP_PIXELFORMAT_L4;
        case GPU_A4:
            return RIP_PIXELFORMAT_A4;
        case GPU_ETC1:
            return RIP_PIXELFORMAT_ETC1;
        case GPU_ETC1A4:
            return RIP_PIXELFORMAT_ETC1A4;
    }

    svcBreak(USERBREAK_PANIC);
    return RIP_PIXELFORMAT_RGB8;
}

bool ripConvertAndLoadC3DTexImage(C3D_Tex* tex, const void* data, GPU_TEXFACE face, int level) {
    const RIPPixelFormat pixelFormat = cvtPF(tex->fmt);
    const size_t size = linearAlloc(tex->width * tex->height * ripGetPixelFormatBPP(pixelFormat));
    void* tmp = linearAlloc(size);
    if (!tmp)
        return NULL;

    bool ret = false;
    if (ripConvertFromNative(data, tmp, tex->width, tex->height, pixelFormat, false)) {
        C3D_TexLoadImage(tex, tmp, face, level);
        ret = true;
    }

    linearFree(tmp);
    return ret;
}

bool ripConvertInPlaceAndLoadC3DTexImage(C3D_Tex* tex, void* data, GPU_TEXFACE face, int level) {
    const RIPPixelFormat pixelFormat = cvtPF(tex->fmt);
    if (ripConvertInPlaceToNative(data, tex->width, tex->height, pixelFormat)) {
        C3D_TexLoadImage(tex, data, face, level);
        return true;
    }

    return false;
}

#endif // RIP_BACKEND == RIP_BACKEND_CITRO3D