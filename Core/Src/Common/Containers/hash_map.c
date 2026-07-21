/*
 * hash_map.c
 *
 *  Created on: Jul 20, 2026
 *      Author: Dawid Sac
 */

#include "Common/Containers/hash_map.h"

#include "Common/Utils/Log.h"

#include <string.h>

#define FNV_OFFSET_BASIS 0x811C9DC5
#define FNV_PRIME 0x01000193

#define MINIMAL_CAPACITY 16
#define LOAD_FACTOR 0.7

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

static uint32_t hash_key(const char *key);

static const char* hm_set_entry(struct allocator *allocator,
		struct hm_entry *entries, size_t capacity, const char *key, void *value,
		size_t *plength);
static bool hm_expend(hash_map_t map);

hash_map_t hm_init(size_t capacity, struct allocator *allocator) {
	struct hash_map *map = (struct hash_map*) mem_alloc(allocator,
			sizeof(struct hash_map));
	if (map == NULL) {
		LOG(CONTAINER, LOG_ERROR, "Error allocating the hash map\r\n");
		return NULL;
	}

	map->length = 0;
	if (capacity < MINIMAL_CAPACITY) {
		LOG(CONTAINER, LOG_WARN,
				"The hash map capacity must be higher then %d\r\n",
				MINIMAL_CAPACITY);
		map->capacity = MINIMAL_CAPACITY;
	} else {
		capacity--;
		uint32_t bit_position = 32 - __builtin_clz(capacity);
		map->capacity = 1 << bit_position;
	}

	map->entries = (struct hm_entry*) mem_calloc(allocator, map->capacity,
			sizeof(struct hm_entry));
	if (map->entries == NULL) {
		mem_dealloc(allocator, map);
		return NULL;
	}
	map->allocator = allocator;
	return map;
}
void hm_free(hash_map_t map) {
	for (size_t i = 0; i < map->capacity; i++) {
		if (map->entries[i].key != NULL && map->entries[i].key != (void*) -1)
			mem_dealloc(map->allocator, (void*) map->entries[i].key);
	}

	mem_dealloc(map->allocator, map->entries);
	mem_dealloc(map->allocator, map);
}

void* hm_get(hash_map_t map, const char *key) {
	uint32_t hash = hash_key(key);
	size_t index = (size_t) (hash & (uint32_t) (map->capacity - 1));

	while (map->entries[index].key != NULL) {
		if (map->entries[index].key != (void*) -1
				&& map->entries[index].cashed_hash == hash
				&& strcmp(key, map->entries[index].key) == 0) {
			return map->entries[index].val;
		}

		index = (index + 1) & (map->capacity - 1);
	}
	return NULL;
}
const char* hm_set(hash_map_t map, const char *key, void *value) {
	if (value == NULL) {
		LOG(CONTAINER, LOG_WARN, "Value for hash_map_set cannot be null\r\n");
		return NULL;
	}

	if (map->length >= map->capacity * LOAD_FACTOR) {
		if (!hm_expend(map)) {
			LOG(CONTAINER, LOG_ERROR, "Failed to expend the hash map\r\n");
			return NULL;
		}
	}

	const char *res = hm_set_entry(map->allocator, map->entries, map->capacity,
			key, value, &map->length);
	if (res == NULL)
		LOG(CONTAINER, LOG_ERROR,
				"Failed to set value: %s for key: %p in the hash map\r\n", key,
				value);
	return res;
}

bool hm_pop(hash_map_t map, const char *key) {
	uint32_t hash = hash_key(key);
	size_t index = (size_t) (hash & (uint32_t) (map->capacity - 1));

	while (map->entries[index].key != NULL) {
		if (map->entries[index].key != (void*) -1
				&& map->entries[index].cashed_hash == hash
				&& strcmp(key, map->entries[index].key) == 0) {
			map->entries[index].key = (void*) -1;
			return true;
		}

		index = (index + 1) & (map->capacity - 1);
	}
	return false;
}

size_t hm_length(hash_map_t map) {
	return map->length;
}

static uint32_t hash_key(const char *key) {
	uint32_t hash = FNV_OFFSET_BASIS;

	for (const char *p = key; *p != '\0'; p++) {
		hash = hash ^ (*p);
		hash *= FNV_PRIME;
	}
	return hash;
}

static const char* hm_set_entry(struct allocator *allocator,
		struct hm_entry *entries, size_t capacity, const char *key, void *value,
		size_t *plength) {
	uint32_t hash = hash_key(key);
	size_t index = (size_t) (hash & (uint32_t) (capacity - 1));

	size_t tombstone;
	bool found_tombstone = false;
	while (entries[index].key != NULL) {
		if (entries[index].key != (void*) -1
				&& entries[index].cashed_hash == hash
				&& strcmp(key, entries[index].key) == 0) {
			entries[index].val = value;
			return entries[index].key;
		}
		if (entries[index].key == (void*) -1 && !found_tombstone) {
			tombstone = index;
			found_tombstone = true;
		}

		index = (index + 1) & (capacity - 1);
	}
	if (found_tombstone)
		index = tombstone;

	if (plength != NULL) {
		size_t len = strlen(key) + 1;
		char *new_key = mem_alloc(allocator, len);
		if (new_key == NULL) {
			return NULL;
		}
		memcpy(new_key, key, len);

		if (key == NULL) {
			return NULL;
		}
		(*plength)++;
		entries[index].key = new_key;
	} else {
		entries[index].key = (char*) key;
	}
	entries[index].val = value;
	entries[index].cashed_hash = hash;
	return entries[index].key;
}

static bool hm_expend(hash_map_t map) {
	size_t new_capacity = map->capacity * 2;
	if (new_capacity < map->capacity) {
		return false;
	}
	LOG(CONTAINER, LOG_INFO, "Expanding Hash Map, new capacity: %d",
			new_capacity);
	struct hm_entry *new_entries = mem_calloc(map->allocator, new_capacity,
			sizeof(struct hm_entry));
	if (new_entries == NULL) {
		return false;
	}

	for (size_t i = 0; i < map->capacity; i++) {
		struct hm_entry entry = map->entries[i];

		if (entry.key != NULL && entry.key != (void*) -1)
			hm_set_entry(map->allocator, new_entries, new_capacity, entry.key,
					entry.val, NULL);

	}

	mem_dealloc(map->allocator, map->entries);
	map->entries = new_entries;
	map->capacity = new_capacity;
	return true;
}
