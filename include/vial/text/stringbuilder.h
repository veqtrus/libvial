/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_STRINGBUILDER_H
#define VIAL_STRINGBUILDER_H

#include <vial/string.h>

struct vial_stringbuilder {
	char *buffer;
	size_t length, capacity;
};

void vial_stringbuilder_init(struct vial_stringbuilder *self);
void vial_stringbuilder_init_cap(struct vial_stringbuilder *self, size_t capacity);
void vial_stringbuilder_init_cstr(struct vial_stringbuilder *self, const char *s);

void vial_stringbuilder_dispose(struct vial_stringbuilder *self);

void vial_stringbuilder_reserve(struct vial_stringbuilder *self, size_t capacity);

static inline
char vial_stringbuilder_get(const struct vial_stringbuilder *self, size_t index)
{
	return self->buffer[index];
}

static inline
void vial_stringbuilder_set(const struct vial_stringbuilder *self, size_t index, int value)
{
	self->buffer[index] = value;
}

void vial_stringbuilder_append_char(struct vial_stringbuilder *self, int value);
void vial_stringbuilder_append_char_array(struct vial_stringbuilder *self, const char *start, size_t length);
void vial_stringbuilder_append_cstr(struct vial_stringbuilder *self, const char *value);
void vial_stringbuilder_append_double(struct vial_stringbuilder *self, double value);
void vial_stringbuilder_append_float(struct vial_stringbuilder *self, float value);
void vial_stringbuilder_append_int(struct vial_stringbuilder *self, int value);
void vial_stringbuilder_append_long(struct vial_stringbuilder *self, long value);
void vial_stringbuilder_append_uint(struct vial_stringbuilder *self, unsigned int value);
void vial_stringbuilder_append_ulong(struct vial_stringbuilder *self, unsigned long value);
void vial_stringbuilder_append_size(struct vial_stringbuilder *self, size_t value);
void vial_stringbuilder_append_string(struct vial_stringbuilder *self, vial_string value);

void vial_stringbuilder_insert_char(struct vial_stringbuilder *self, size_t offset, int value);
void vial_stringbuilder_insert_char_array(struct vial_stringbuilder *self, size_t offset, const char *start, size_t length);
void vial_stringbuilder_insert_cstr(struct vial_stringbuilder *self, size_t offset, const char *value);
void vial_stringbuilder_insert_double(struct vial_stringbuilder *self, size_t offset, double value);
void vial_stringbuilder_insert_float(struct vial_stringbuilder *self, size_t offset, float value);
void vial_stringbuilder_insert_int(struct vial_stringbuilder *self, size_t offset, int value);
void vial_stringbuilder_insert_long(struct vial_stringbuilder *self, size_t offset, long value);
void vial_stringbuilder_insert_uint(struct vial_stringbuilder *self, size_t offset, unsigned int value);
void vial_stringbuilder_insert_ulong(struct vial_stringbuilder *self, size_t offset, unsigned long value);
void vial_stringbuilder_insert_size(struct vial_stringbuilder *self, size_t offset, size_t value);
void vial_stringbuilder_insert_string(struct vial_stringbuilder *self, size_t offset, vial_string value);

void vial_stringbuilder_delete(struct vial_stringbuilder *self, size_t start, size_t end);
void vial_stringbuilder_delete_at(struct vial_stringbuilder *self, size_t index);

void vial_stringbuilder_reverse(struct vial_stringbuilder *self);

vial_string vial_stringbuilder_to_string(const struct vial_stringbuilder *self);
char *vial_stringbuilder_extract_cstr(struct vial_stringbuilder *self);

#endif
