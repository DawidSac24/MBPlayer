/*
 * hash_map.h
 *  Created on: Jul 20, 2026
 *      Author: Dawid Sac
 *  Hash Map Implementation with FNV-1a hashing and linear probing
 */

#ifndef INC_APP_HASH_MAP_H_
#define INC_APP_HASH_MAP_H_

#include "Common/Allocators/allocator.h"

#include <stdint.h>
#include <stdbool.h>

struct hm_entry {
	char *key;
	void *val;
	uint32_t cashed_hash;
};

struct hash_map {
	struct allocator *allocator;
	struct hm_entry *entries;
	size_t capacity;
	size_t length;
};

struct hash_map* hm_init(size_t capacity, struct allocator *allocator);
void hm_free(struct hash_map *map);

void* hm_get(struct hash_map *map, const char *key);
const char* hm_set(struct hash_map *map, const char *key, void *value);
bool hm_pop(struct hash_map *map, const char *key);

size_t hm_length(struct hash_map *map);

#endif /* INC_APP_HASH_MAP_H_ */
