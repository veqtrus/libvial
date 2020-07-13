/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/text/stringbuilder.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void vial_stringbuilder_init(struct vial_stringbuilder *self)
{
	self->length = 0;
	self->capacity = 16;
	self->buffer = malloc(16);
}

void vial_stringbuilder_init_cap(struct vial_stringbuilder *self, size_t capacity)
{
	self->length = 0;
	self->capacity = capacity;
	self->buffer = malloc(capacity);
}

void vial_stringbuilder_init_cstr(struct vial_stringbuilder *self, const char *s)
{
	self->length = 0;
	self->capacity = 0;
	self->buffer = NULL;
	vial_stringbuilder_append_cstr(self, s);
}

void vial_stringbuilder_dispose(struct vial_stringbuilder *self)
{
	free(self->buffer);
	self->capacity = self->length = 0;
	self->buffer = NULL;
}

void vial_stringbuilder_reserve(struct vial_stringbuilder *self, size_t capacity)
{
	if (self->capacity < capacity) {
		self->capacity *= 2;
		if (self->capacity < capacity)
			self->capacity = capacity;
		self->buffer = realloc(self->buffer, self->capacity);
	}
}

void vial_stringbuilder_append_char(struct vial_stringbuilder *self, int value)
{
	vial_stringbuilder_reserve(self, self->length + 1);
	self->buffer[self->length++] = value;
}

void vial_stringbuilder_append_char_array(struct vial_stringbuilder *self, const char *start, size_t length)
{
	if (length == 0) return;
	vial_stringbuilder_reserve(self, self->length + length);
	memcpy(self->buffer + self->length, start, length);
	self->length += length;
}

void vial_stringbuilder_append_cstr(struct vial_stringbuilder *self, const char *value)
{
	if (value == NULL) return;
	vial_stringbuilder_append_char_array(self, value, strlen(value));
}

void vial_stringbuilder_append_double(struct vial_stringbuilder *self, double value)
{
	char buf[32];
	sprintf(buf, "%f", value);
	vial_stringbuilder_append_cstr(self, buf);
}

void vial_stringbuilder_append_float(struct vial_stringbuilder *self, float value)
{
	char buf[32];
	sprintf(buf, "%f", value);
	vial_stringbuilder_append_cstr(self, buf);
}

void vial_stringbuilder_append_int(struct vial_stringbuilder *self, int value)
{
	char buf[32];
	sprintf(buf, "%d", value);
	vial_stringbuilder_append_cstr(self, buf);
}

void vial_stringbuilder_append_long(struct vial_stringbuilder *self, long value)
{
	char buf[32];
	sprintf(buf, "%ld", value);
	vial_stringbuilder_append_cstr(self, buf);
}

void vial_stringbuilder_append_uint(struct vial_stringbuilder *self, unsigned int value)
{
	char buf[32];
	sprintf(buf, "%u", value);
	vial_stringbuilder_append_cstr(self, buf);
}

void vial_stringbuilder_append_ulong(struct vial_stringbuilder *self, unsigned long value)
{
	char buf[32];
	sprintf(buf, "%lu", value);
	vial_stringbuilder_append_cstr(self, buf);
}

void vial_stringbuilder_append_size(struct vial_stringbuilder *self, size_t value)
{
	char buf[32];
	sprintf(buf, "%zu", value);
	vial_stringbuilder_append_cstr(self, buf);
}

void vial_stringbuilder_append_string(struct vial_stringbuilder *self, vial_string value)
{
	vial_stringbuilder_append_char_array(self, value, vial_string_length(value));
}

void vial_stringbuilder_insert_char(struct vial_stringbuilder *self, size_t offset, int value)
{
	vial_stringbuilder_reserve(self, self->length + 1);
	for (size_t i = self->length; i --> offset;)
		self->buffer[i + 1] = self->buffer[i];
	self->buffer[offset] = value;
	self->length++;
}

void vial_stringbuilder_insert_char_array(struct vial_stringbuilder *self, size_t offset, const char *start, size_t length)
{
	if (length == 0) return;
	vial_stringbuilder_reserve(self, self->length + length);
	memmove(self->buffer + offset + length, self->buffer + offset, length);
	memcpy(self->buffer + offset, start, length);
	self->length += length;
}

void vial_stringbuilder_insert_cstr(struct vial_stringbuilder *self, size_t offset, const char *value)
{
	if (value == NULL) return;
	vial_stringbuilder_insert_char_array(self, offset, value, strlen(value));
}

void vial_stringbuilder_insert_double(struct vial_stringbuilder *self, size_t offset, double value)
{
	char buf[32];
	sprintf(buf, "%f", value);
	vial_stringbuilder_insert_cstr(self, offset, buf);
}

void vial_stringbuilder_insert_float(struct vial_stringbuilder *self, size_t offset, float value)
{
	char buf[32];
	sprintf(buf, "%f", value);
	vial_stringbuilder_insert_cstr(self, offset, buf);
}

void vial_stringbuilder_insert_int(struct vial_stringbuilder *self, size_t offset, int value)
{
	char buf[32];
	sprintf(buf, "%d", value);
	vial_stringbuilder_insert_cstr(self, offset, buf);
}

void vial_stringbuilder_insert_long(struct vial_stringbuilder *self, size_t offset, long value)
{
	char buf[32];
	sprintf(buf, "%ld", value);
	vial_stringbuilder_insert_cstr(self, offset, buf);
}

void vial_stringbuilder_insert_uint(struct vial_stringbuilder *self, size_t offset, unsigned int value)
{
	char buf[32];
	sprintf(buf, "%u", value);
	vial_stringbuilder_insert_cstr(self, offset, buf);
}

void vial_stringbuilder_insert_ulong(struct vial_stringbuilder *self, size_t offset, unsigned long value)
{
	char buf[32];
	sprintf(buf, "%lu", value);
	vial_stringbuilder_insert_cstr(self, offset, buf);
}

void vial_stringbuilder_insert_size(struct vial_stringbuilder *self, size_t offset, size_t value)
{
	char buf[32];
	sprintf(buf, "%zu", value);
	vial_stringbuilder_insert_cstr(self, offset, buf);
}

void vial_stringbuilder_insert_string(struct vial_stringbuilder *self, size_t offset, vial_string value)
{
	vial_stringbuilder_insert_char_array(self, offset, value, vial_string_length(value));
}


void vial_stringbuilder_delete(struct vial_stringbuilder *self, size_t start, size_t end)
{
	self->length -= end - start;
	memmove(self->buffer + start, self->buffer + end, end - start);
}

void vial_stringbuilder_delete_at(struct vial_stringbuilder *self, size_t index)
{
	self->length--;
	for (size_t i = index; i < self->length; i++)
		self->buffer[i] = self->buffer[i + 1];
}

void vial_stringbuilder_reverse(struct vial_stringbuilder *self)
{
	char t, *l, *r;
	l = self->buffer;
	r = self->buffer + self->length - 1;
	for (; l < r; ++l, --r) {
		t = *l; *l = *r; *r = t;
	}
}

vial_string vial_stringbuilder_to_string(const struct vial_stringbuilder *self)
{
	return vial_string_from_char_array(self->buffer, self->length);
}

char *vial_stringbuilder_extract_cstr(struct vial_stringbuilder *self)
{
	vial_stringbuilder_append_char(self, 0);
	char *result = self->buffer;
	self->capacity = self->length = 0;
	self->buffer = NULL;
	return result;
}
