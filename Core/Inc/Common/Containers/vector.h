/*
 * vector.h
 *
 *  Created on: 21 Jul 2026
 *      Author: majst
 */

#ifndef INC_COMMON_CONTAINERS_VECTOR_H_
#define INC_COMMON_CONTAINERS_VECTOR_H_

struct ptr_vector {
	struct allocator *allocator;
	void *data;
	size_t elem_size;
	size_t capacity;
	size_t len;
};


#endif /* INC_COMMON_CONTAINERS_VECTOR_H_ */
