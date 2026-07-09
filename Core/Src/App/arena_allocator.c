/*
 * arena_allocator.c
 *
 *  Created on: Jul 9, 2026
 *      Author: Dawid Sac
 */
#include "App/arena_allocator.h"

#include "App/log.h"

static const allocator_vtable_t s_arena_vtable = { .alloc = arena_alloc,
		.dealloc = arena_dealloc, .free = arena_free };

void arena_init(arena_allocator_t *arena, uint8_t *buffer, size_t size) {
	arena->start = buffer;
	arena->end = buffer + size;
	arena->pos = buffer;

	arena->base.vtable = &s_arena_vtable;
}

void arena_free(allocator_t *self) {
	arena_allocator_t *arena = (arena_allocator_t*) self;

	arena->pos = arena->start;
}

void* arena_alloc(allocator_t *self, size_t size) {
	arena_allocator_t *arena = (arena_allocator_t*) self;

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
void arena_dealloc(allocator_t *self, void *ptr) {
	return;
}
