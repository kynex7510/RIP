#include <RIP/Cache.h>

#if RIP_BACKEND == RIP_BACKEND_KYGX
#include <KYGX/Wrappers/FlushCacheRegions.h>
#include <KYGX/Utility.h>
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU
#include <3ds.h>
#elif RIP_BACKEND == RIP_BACKEND_CITRO3D
#include <citro3d.h>
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS
#include <drivers/cache.h>
#endif

void ripFlushDataCache(const void* data, size_t size) {
#if RIP_BACKEND == RIP_BACKEND_KYGX
    kygxSyncFlushSingleBuffer(data, size);
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU || RIP_BACKEND == RIP_BACKEND_CITRO3D
    if (R_FAILED(GSPGPU_FlushDataCache(data, size)))
        svcBreak(USERBREAK_PANIC);
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS
    flushDCacheRange(data, size);
#endif
}

void ripInvalidateDataCache(const void* data, size_t size) {
#if RIP_BACKEND == RIP_BACKEND_KYGX
    kygxInvalidateDataCache(data, size);
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU || RIP_BACKEND == RIP_BACKEND_CITRO3D
    if (R_FAILED(GSPGPU_InvalidateDataCache(data, size)))
        svcBreak(USERBREAK_PANIC);
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS
    invalidateDCacheRange(data, size);
#endif
}