/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_ENCODING_JSON_H
#define VIAL_ENCODING_JSON_H

#include <stddef.h>

#include <vial/collections/vector.h>
#include <vial/sharedptr.h>
#include <vial/string.h>

enum vial_json_type {
	VIAL_JSON_INVALID = 0,
	VIAL_JSON_NULL,
	VIAL_JSON_FALSE,
	VIAL_JSON_TRUE,
	VIAL_JSON_NUMBER,
	VIAL_JSON_STRING,
	VIAL_JSON_ARRAY,
	VIAL_JSON_OBJECT
};

typedef vial_vector(struct vial_json) *vial_json_array_t;
typedef vial_vector(struct vial_json_keyvalue) *vial_json_object_t;

struct vial_json {
	enum vial_json_type type;
	union {
		double as_number;
		char *as_string;
		vial_json_array_t as_array;
		vial_json_object_t as_object;
		void *as_sharedptr;
	} value;
};

struct vial_json_keyvalue { char *key; struct vial_json value; };

static inline void vial_json_dispose(struct vial_json *self)
{
	switch (self->type) {
	case VIAL_JSON_STRING:
	case VIAL_JSON_ARRAY:
	case VIAL_JSON_OBJECT:
		vial_sharedptr_leave(self->value.as_sharedptr);
		break;
	default:
		break;
	}
}

struct vial_json vial_json_create(enum vial_json_type type);

#define vial_json_create_bool(value) vial_json_create((value) ? VIAL_JSON_TRUE : VIAL_JSON_FALSE)

static inline struct vial_json vial_json_create_num(double value)
{
	struct vial_json result;
	result.type = VIAL_JSON_NUMBER;
	result.value.as_number = value;
	return result;
}

struct vial_json vial_json_create_str(const char *value);

void vial_json_push(struct vial_json *self, struct vial_json item);

void vial_json_put(struct vial_json *self, const char *key, struct vial_json item);

struct vial_json vial_json_at(struct vial_json *self, size_t idx);

struct vial_json vial_json_get(struct vial_json *self, const char *key);

const char *vial_json_key(struct vial_json *self, size_t idx);

static inline size_t vial_json_length(struct vial_json *self)
{
	if (self->type == VIAL_JSON_ARRAY)
		return self->value.as_array->size;
	if (self->type == VIAL_JSON_OBJECT)
		return self->value.as_object->size;
	return 0;
}

struct vial_json vial_json_decode(const char *str);

struct vial_string vial_json_encode(struct vial_json *self, const char *indent);

#endif
