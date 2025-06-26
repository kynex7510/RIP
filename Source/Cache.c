#include <RIP/Cache.h>

#if RIP_BACKEND == RIP_BACKEND_KYGX
#include <GX/Wrappers/FlushCacheRegions.h>
#include <GX/Utility.h>
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU
#include <3ds.h>
#elif RIP_BACKEND == RIP_BACKEND_CITRO3D
#include <citro3d.h>
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS
#include <drivers/cache.h>
#endif

void ripFlushDataCache(void* data, size_t size) {
#if RIP_BACKEND == RIP_BACKEND_KYGX
    GXFlushCacheRegionsBuffer flush;
    flush.addr = data;
    flush.size = size;
    kygxSyncFlushCacheRegions(&flush, NULL, NULL);
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU || RIP_BACKEND == RIP_BACKEND_CITRO3D
    if (R_FAILED(GSPGPU_FlushDataCache(data, size)))
        svcBreak(USERBREAK_PANIC);
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS
    flushDCacheRange(data, size);
#endif
}

void ripInvalidateDataCache(void* data, size_t size) {
#if RIP_BACKEND == RIP_BACKEND_KYGX
    kygxInvalidateDataCache(data, size);
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU || RIP_BACKEND == RIP_BACKEND_CITRO3D
    if (R_FAILED(GSPGPU_InvalidateDataCache(data, size)))
        svcBreak(USERBREAK_PANIC);
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS
    invalidateDCacheRange(data, size);
#endif
}