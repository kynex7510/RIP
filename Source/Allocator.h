/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _RIP_ALLOCATOR_H
#define _RIP_ALLOCATOR_H

#include <RIP/Defs.h>

#if RIP_BACKEND == RIP_BACKEND_KYGX
#include <KYGX/Allocator.h>
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU
#include <3ds.h>
#elif RIP_BACKEND == RIP_BACKEND_CITRO3D
#include <citro3d.h>
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS
// TODO
#endif

#include <stdlib.h> // malloc, free

RIP_INLINE void* ripHeapAlloc(size_t size) {
#if RIP_BACKEND == RIP_BACKEND_KYGX
    return kygxAlloc(KYGX_MEM_HEAP, size);
#else
    return malloc(size);
#endif
}

RIP_INLINE void ripHeapFree(void* p) {
#if RIP_BACKEND == RIP_BACKEND_KYGX
    kygxFree(p);
#else
    free(p);
#endif
}

RIP_INLINE void* ripLinearAlloc(size_t size) {
#if RIP_BACKEND == RIP_BACKEND_KYGX
    return kygxAlloc(KYGX_MEM_LINEAR, size);
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU || RIP_BACKEND == RIP_BACKEND_CITRO3D
    return linearAlloc(size);
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS
// TODO
#error "Unimplemented!"
#endif
}

RIP_INLINE void ripLinearFree(void* p) {
#if RIP_BACKEND == RIP_BACKEND_KYGX
    kygxFree(p);
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU || RIP_BACKEND == RIP_BACKEND_CITRO3D
    linearFree(p);
#elif RIP_BACKEND == RIP_BACKEND_LIBN3DS
// TODO
#error "Unimplemented!"
#endif
}

#endif /* _RIP_ALLOCATOR_H */