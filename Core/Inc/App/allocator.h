/*
 * allocator.h
 *
 *  Created on: Jul 9, 2026
 *      Author: Dawid Sac
 */

#ifndef INC_APP_ALLOCATOR_H_
#define INC_APP_ALLOCATOR_H_

#include <stddef.h>

#define MEM_ALLIGN 4

typedef struct allocator_s allocator_t;

typedef struct {
	void* (*alloc)(allocator_t *self, size_t size);
	void (*dealloc)(allocator_t *self, void *ptr);
	void (*free)(allocator_t *self);
} allocator_vtable_t;

struct allocator_s {
	const allocator_vtable_t *vtable;
};

static inline void* mem_alloc(allocator_t *self, size_t size) {
	return self->vtable->alloc(self, size);
}

static inline void mem_dealloc(allocator_t *self, void *ptr) {
	self->vtable->dealloc(self, ptr);
}

static inline void mem_free(allocator_t *self) {
	self->vtable->free(self);
}

#endif /* INC_APP_ALLOCATOR_H_ */
