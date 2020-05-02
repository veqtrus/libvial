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
#include <vial/collections/map.h>
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

struct vial_json_array;
struct vial_json_object;

struct vial_json {
	enum vial_json_type type;
	union {
		double as_number;
		char *as_string;
		struct vial_json_array *as_array;
		struct vial_json_object *as_object;
		void *as_sharedptr;
	} value;
};

struct vial_json_array {
	vial_vector(struct vial_json) vec;
};

struct vial_json_object {
	vial_map(const char *, struct vial_json) map;
};

static inline
struct vial_json vial_json_take(struct vial_json self)
{
	switch (self.type) {
	case VIAL_JSON_STRING:
	case VIAL_JSON_ARRAY:
	case VIAL_JSON_OBJECT:
		vial_sharedptr_take(self.value.as_sharedptr);
		break;
	default:
		break;
	}
	return self;
}

static inline
void vial_json_leave(struct vial_json *self)
{
	switch (self->type) {
	case VIAL_JSON_STRING:
	case VIAL_JSON_ARRAY:
	case VIAL_JSON_OBJECT:
		vial_sharedptr_leave(self->value.as_sharedptr);
		return;
	default:
		return;
	}
}

static inline
struct vial_json vial_json_create_invalid(void)
{
	struct vial_json result;
	result.type = VIAL_JSON_INVALID;
	return result;
}

static inline
struct vial_json vial_json_create_null(void)
{
	struct vial_json result;
	result.type = VIAL_JSON_NULL;
	return result;
}

static inline
struct vial_json vial_json_create_bool(int value)
{
	struct vial_json result;
	result.type = value ? VIAL_JSON_TRUE : VIAL_JSON_FALSE;
	return result;
}

static inline
struct vial_json vial_json_create_num(double value)
{
	struct vial_json result;
	result.type = VIAL_JSON_NUMBER;
	result.value.as_number = value;
	return result;
}

struct vial_json vial_json_create_str(const char *value);

struct vial_json vial_json_create_array(void);

struct vial_json vial_json_create_object(void);

void vial_json_push_move(struct vial_json self, struct vial_json item);

static inline
void vial_json_push(struct vial_json self, struct vial_json item)
{
	vial_json_push_move(self, vial_json_take(item));
}

static inline
void vial_json_push_bool(struct vial_json self, int value)
{
	vial_json_push_move(self, vial_json_create_bool(value));
}

static inline
void vial_json_push_num(struct vial_json self, double value)
{
	vial_json_push_move(self, vial_json_create_num(value));
}

static inline
void vial_json_push_str(struct vial_json self, const char *value)
{
	vial_json_push_move(self, vial_json_create_str(value));
}

void vial_json_put_move(struct vial_json self, const char *key, struct vial_json item);

static inline
void vial_json_put(struct vial_json self, const char *key, struct vial_json item)
{
	vial_json_put_move(self, key, vial_json_take(item));
}

static inline
void vial_json_put_bool(struct vial_json self, const char *key, int value)
{
	vial_json_put_move(self, key, vial_json_create_bool(value));
}

static inline
void vial_json_put_num(struct vial_json self, const char *key, double value)
{
	vial_json_put_move(self, key, vial_json_create_num(value));
}

static inline
void vial_json_put_str(struct vial_json self, const char *key, const char *value)
{
	vial_json_put_move(self, key, vial_json_create_str(value));
}

struct vial_json vial_json_at(struct vial_json self, size_t idx);

struct vial_json vial_json_get(struct vial_json self, const char *key);

static inline size_t vial_json_length(const struct vial_json self)
{
	if (self.type == VIAL_JSON_ARRAY)
		return self.value.as_array->vec.size;
	if (self.type == VIAL_JSON_OBJECT)
		return self.value.as_object->map.size;
	return 0;
}

struct vial_json vial_json_decode(const char *str);

struct vial_string vial_json_encode(struct vial_json self, const char *indent);

#endif
