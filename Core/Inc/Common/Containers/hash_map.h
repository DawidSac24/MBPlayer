/*
 * hash_map.h
 *  Created on: Jul 20, 2026
 *      Author: Dawid Sac
 *  Hash Map Implementation with FNV-1a hashing and linear probing
 */

#ifndef INC_COMMON_CONTAINERS_MAP_H_
#define INC_COMMON_CONTAINERS_MAP_H_

#include "Common/Allocators/allocator.h"

#include <stdint.h>
#include <stdbool.h>

struct hash_map;
typedef struct hash_map *hash_map_t;

hash_map_t hm_init(size_t capacity, struct allocator *allocator);
void hm_free(hash_map_t map);

void* hm_get(hash_map_t map, const char *key);
const char* hm_set(hash_map_t map, const char *key, void *value);
bool hm_pop(hash_map_t map, const char *key);

size_t hm_length(hash_map_t map);

#endif /* INC_COMMON_CONTAINERS_MAP_H_ */
