/*
 * ptr_vector.c
 *
 *  Created on: 22 Jul 2026
 *      Author: majst
 */

#include "Common/Containers/ptr_vector.h"

#include "Common/Utils/Log.h"

#define MIN_CAPACITY 4
#define EXPEND_MULT 2

struct ptr_vector {
	struct allocator *allocator;
	void **data;
	size_t capacity;
	size_t len;
};

bool pvec_expend(ptr_vector_t self);

ptr_vector_t pvec_init(struct allocator *allocator, size_t capacity) {
	ptr_vector_t self = (ptr_vector_t) mem_alloc(allocator,
			sizeof(struct ptr_vector));
	if (self == NULL) {
		LOG(CONTAINER, LOG_ERROR, "Failed to allocate pointer vector\r\n");
		return NULL;
	}

	if (capacity < MIN_CAPACITY) {
		LOG(CONTAINER, LOG_WARN, "Vector must have a size of at least %d\r\n",
				MIN_CAPACITY);
		capacity = MIN_CAPACITY;
	}

	self->data = mem_alloc(allocator, sizeof(void*) * capacity);
	if (self->data == NULL) {
		LOG(CONTAINER, LOG_ERROR, "Failed to allocate vector data\r\n");
		mem_free(allocator, self);
		return NULL;
	}

	self->allocator = allocator;
	self->capacity = capacity;
	self->len = 0;
	return self;
}
void pvec_free(ptr_vector_t self) {
	if (!self)
		return;
	if (self->data) {
		mem_free(self->allocator, self->data);
	}
	mem_free(self->allocator, self->data);
}

bool pvec_expend(ptr_vector_t self) {
	size_t new_capacity = self->capacity * EXPEND_MULT;
	if (new_capacity < self->capacity)
		return false;
	LOG(CONTAINER, LOG_DEBUG, "Expending pointer vector from %d to %d\r\n",
			self->capacity, new_capacity);

	void **new_data = mem_realloc(self->allocator, self->data,
			sizeof(void*) * new_capacity);

	if (new_data == NULL) {
		LOG(CONTAINER, LOG_INFO, "Failed to reallocate the pointer vector\r\n");
		return false;
	}

	self->data = new_data;
	self->capacity = new_capacity;
	return true;
}

bool pvec_push_last(ptr_vector_t self, void *data) {
	if (self->len == self->capacity) {
		if (!pvec_expend(self))
			return false;
	}

	self->data[self->len] = data;
	self->len++;
	return true;
}
void* pvec_pop_last(ptr_vector_t self) {
	if (self->len == 0)
		return NULL;

	return self->data[--self->len];
}

bool pvec_push_first(ptr_vector_t self, void *data) {
	if (self->len == self->capacity) {
		if (!pvec_expend(self))
			return false;
	}

	if (self->len > 0) {
		memmove(self->data[1], self->data[0], self->len * sizeof(void*));
	}

	self->data[0] = data;
	self->len++;
	return true;
}
void* pvec_pop_first(ptr_vector_t self) {
	if (self->len == 0)
		return NULL;

	void *val = self->data[0];
	self->len--;

	if (self->len > 0) {
		memmove(self->data[0], self->data[1], self->len * sizeof(void*));
	}
	return val;
}

bool pvec_empty(ptr_vector_t self) {
	return self->len == 0;
}
bool pvec_full(ptr_vector_t self) {
	return self->len == self->capacity;
}
size_t pvec_len(ptr_vector_t self) {
	return self->len;
}
