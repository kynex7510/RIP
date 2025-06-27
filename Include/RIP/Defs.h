/**
 * @file Defs.h
 * @brief Common definitions.
 */
#ifndef _RIP_DEFS_H
#define _RIP_DEFS_H

#define RIP_BACKEND_KYGX 0x01
#define RIP_BACKEND_LIBCTRU 0x02
#define RIP_BACKEND_CITRO3D 0x04
#define RIP_BACKEND_LIBN3DS 0x08

#if !(RIP_BACKEND & 0x0F)
#error "Invalid backend!"
#endif // RIP_BACKEND & 0x0F

#ifdef __cplusplus

#include <cstdint>
#include <cstddef>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;

#define RIP_INLINE inline

#else

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define RIP_INLINE static inline

#endif // __cplusplus

// Enable diagnostics if we're using KYGX.
#if RIP_BACKEND == RIP_BACKEND_KYGX
#define RIP_ASSERT(cond) KYGX_ASSERT(cond)
#else
#define RIP_ASSERT(cond) (void)(cond)
#endif

#endif /* _RIP_DEFS_H */