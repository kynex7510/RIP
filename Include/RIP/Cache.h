/**
 * @file Cache.h
 * @brief Cache utilities.
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
 * @note This is a no-op when no backend is provided.
 */
void ripFlushDataCache(void* data, size_t size);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _RIP_CACHE_H */