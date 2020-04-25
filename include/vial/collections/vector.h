/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_COLLECTIONS_VECTOR_H
#define VIAL_COLLECTIONS_VECTOR_H

#include <stdlib.h>
#include <string.h>

#define vial_vector(T) struct { T *values; size_t size, capacity; }

typedef vial_vector(void) vial_vector_void_t;

typedef vial_vector(char) vial_vector_char_t;
typedef vial_vector(short) vial_vector_short_t;
typedef vial_vector(int) vial_vector_int_t;
typedef vial_vector(long) vial_vector_long_t;

typedef vial_vector(unsigned char) vial_vector_uchar_t;
typedef vial_vector(unsigned short) vial_vector_ushort_t;
typedef vial_vector(unsigned int) vial_vector_uint_t;
typedef vial_vector(unsigned long) vial_vector_ulong_t;

typedef vial_vector(const char *) vial_vector_cstr_t;

#define VIAL_VECTOR_EMPTY { NULL, 0, 0 }

#define vial_vector_init(self) do { (self).values = NULL; (self).capacity = (self).size = 0; } while (0)

#define vial_vector_dispose(self) free((self).values)

#define vial_vector_at(self, idx) (self).values[(idx)]

#define vial_vector_last(self) (self).values[(self).size - 1]

static inline void _vial_vector_reserve(void *vec, size_t isize, size_t capacity)
{
	vial_vector_void_t *self = (vial_vector_void_t *) vec;
	if (self->capacity < capacity) {
		self->capacity *= 2;
		if (self->capacity < capacity)
			self->capacity = capacity;
		self->values = realloc(self->values, self->capacity * isize);
	}
}

#define vial_vector_reserve(self, capacity) _vial_vector_reserve((void *) &(self), sizeof((self).values[0]), (capacity))

static inline void _vial_vector_shrink(void *vec, size_t isize)
{
	vial_vector_void_t *self = (vial_vector_void_t *) vec;
	if (self->capacity > self->size) {
		self->capacity = self->size;
		self->values = realloc(self->values, self->capacity * isize);
	}
}

#define vial_vector_shrink(self) _vial_vector_shrink((void *) &(self), sizeof((self).values[0]))

#define vial_vector_add(self, item) do { \
	vial_vector_reserve((self), (self).size + 1); \
	(self).values[(self).size++] = (item); \
} while (0)

#define vial_vector_addall(self, items, count) do { \
	vial_vector_reserve((self), (self).size + (count)); \
	memcpy((self).values + (self).size, (items), sizeof((self).values[0]) * (count)); \
	(self).size += (count); \
} while (0)

#define vial_vector_insert(self, idx, item) do { \
	vial_vector_reserve((self), (self).size + 1); \
	memmove((self).values + (idx) + 1, (self).values + (idx), ((self).size - (idx)) * sizeof((self).values[0])); \
	(self).values[(idx)] = (item); (self).size++; \
} while (0)

#define vial_vector_remove(self, idx) do { \
	memmove((self).values + (idx), (self).values + (idx) + 1, ((self).size - (idx) - 1) * sizeof((self).values[0])); \
	(self).size--; \
} while (0)

#define vial_vector_clear(self) do { (self).size = 0; } while (0)

#define vial_vector_foreach(itr, vec) for ((itr) = (vec).values; (itr) < (vec).values + (vec).size; ++(itr))

#endif
