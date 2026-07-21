/*
 * arena_allocator.c
 *
 *  Created on: Jul 9, 2026
 *      Author: Dawid Sac
 */
#include "Common/Allocators/arena_allocator.h"

#include "Common/Utils/log.h"

static const struct allocator_vtable s_arena_vtable = { .alloc = arena_alloc,
		.dealloc = arena_dealloc, .free = arena_free };

void arena_init(struct arena_allocator *arena, uint8_t *buffer, size_t size) {
	arena->start = buffer;
	arena->end = buffer + size;
	arena->pos = buffer;

	arena->base.vtable = &s_arena_vtable;
}

void arena_free(struct allocator *self) {
	struct arena_allocator *arena = (struct arena_allocator*) self;

	arena->pos = arena->start;
}

void* arena_alloc(struct allocator *self, size_t size) {
	struct arena_allocator *arena = (struct arena_allocator*) self;

	uint8_t *const last_pos = arena->pos;
	size = (size + MEM_ALLIGN - 1) & ~(MEM_ALLIGN - 1);

	if (last_pos + size > arena->end || last_pos + size < arena->start) {
		LOG(ALLOCATOR, LOG_ERROR,
				"Arena allocator failed to allocate memory: size overflowed\r\n");
		return NULL;
	}

	arena->pos += size;
	return last_pos;
}
void arena_dealloc(struct allocator *self, void *ptr) {
	return;
}
