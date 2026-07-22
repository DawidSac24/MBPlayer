/*
 * ptr_vector.h
 *
 *  Created on: 21 Jul 2026
 *      Author: majst
 */

#ifndef INC_COMMON_CONTAINERS_PTR_VECTOR_H_
#define INC_COMMON_CONTAINERS_PTR_VECTOR_H_

#include "Common/Allocators/allocator.h"

#include <stdbool.h>
#include <stddef.h>

struct ptr_vector;
typedef struct ptr_vector *ptr_vector_t;

ptr_vector_t pvec_init(struct allocator *allocator, size_t capacity);
void pvec_free(ptr_vector_t self);

bool pvec_push_last(ptr_vector_t self, void *data);
void* pvec_pop_last(ptr_vector_t self);

bool pvec_push_first(ptr_vector_t self, void *data);
void* pvec_pop_first(ptr_vector_t self);

void* pvec_remove_at(ptr_vector_t self, size_t index);
void* pvec_remove_val(ptr_vector_t self, void *val);

bool pvec_empty(ptr_vector_t self);
bool pvec_full(ptr_vector_t self);
size_t pvec_len(ptr_vector_t self);

#endif /* INC_COMMON_CONTAINERS_PTR_VECTOR_H_ */
