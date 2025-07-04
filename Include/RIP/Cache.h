/**
 * @file Cache.h
 * @brief Cache utilities.
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _RIP_CACHE_H
#define _RIP_CACHE_H

#include <RIP/Defs.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Flush (clean + invalidate) data cache.
 * @param[in] addr Address of the data to flush.
 * @param[in] size Size of the data to flush.
 */
void ripFlushDataCache(const void* data, size_t size);

/**
 * @brief Invalidate data cache.
 * @param[in] addr Address of the data to invalidate.
 * @param[in] size Size of the data to invalidate.
 */
void ripInvalidateDataCache(const void* data, size_t size);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _RIP_CACHE_H */