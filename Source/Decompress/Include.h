/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _RIP_DECOMPRESS_H
#define _RIP_DECOMPRESS_H

#include <RIP/Defs.h>

#if RIP_BACKEND == RIP_BACKEND_KYGX
#ifndef KYGX_BAREMETAL
#define COMPRESSION_USE_LIBCTRU
#endif // KYGX_BAREMETAL
#elif RIP_BACKEND == RIP_BACKEND_LIBCTRU || RIP_BACKEND == RIP_BACKEND_CITRO3D
#define COMPRESSION_USE_LIBCTRU
#endif

#ifdef COMPRESSION_USE_LIBCTRU
#include <3ds.h>
#else
#include "Decompress.h"
#endif // COMPRESSION_USE_LIBCTRU

#endif /* _RIP_DECOMPRESS_H */