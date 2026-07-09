/*
 * arenaAllocator.h
 *
 *  Created on: Jul 9, 2026
 *      Author: Dawid Sac
 */

#ifndef INC_APP_ARENA_ALLOCATOR_H_
#define INC_APP_ARENA_ALLOCATOR_H_

#include "App/allocator.h"

#include <stdio.h>
#include <stdint.h>

typedef struct {
	allocator_t base;
	uint8_t *start;
	uint8_t *end;
	uint8_t *pos;
} arena_allocator_t;

void arena_init(arena_allocator_t *arena, uint8_t *buffer, size_t size);
void arena_free(allocator_t *self);

void* arena_alloc(allocator_t *self, size_t size);
void arena_dealloc(allocator_t *self, void *ptr);

#endif /* INC_APP_ARENA_ALLOCATOR_H_ */
