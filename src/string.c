/*
Copyright (c) 2019-2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/string.h>

#include <stdlib.h>
#include <string.h>

char *vial_strdup(const char *s)
{
	if (s == NULL) return NULL;
	size_t len = strlen(s) + 1;
	char *res = malloc(len);
	memcpy(res, s, len);
	return res;
}

int vial_cstr_starts(const char *s, const char *b)
{
	for (; *b; ++s, ++b)
		if (*s != *b)
			return 0;
	return 1;
}

void vial_string_init(struct vial_string *self, const char *s)
{
	if (s == NULL) {
		self->length = 0;
		self->str.static_buf[0] = '\0';
		return;
	}
	self->length = strlen(s);
	if (self->length < VIAL_STRING_STATIC_SIZE) {
		memcpy(self->str.static_buf, s, self->length + 1);
	} else {
		self->str.dynamic.alloc = self->length + 1;
		self->str.dynamic.buf = malloc(self->str.dynamic.alloc);
		memcpy(self->str.dynamic.buf, s, self->str.dynamic.alloc);
	}
}

void vial_string_init_copy(struct vial_string *self, const struct vial_string *s)
{
	self->length = 0;
	self->str.static_buf[0] = '\0';
	vial_string_append(self, s);
}

void vial_string_clear(struct vial_string *self)
{
	if (self->length >= VIAL_STRING_STATIC_SIZE)
		free(self->str.dynamic.buf);
	self->length = 0;
	self->str.static_buf[0] = '\0';
}

static void set_length(struct vial_string *self, size_t new_length)
{
	size_t needed_capacity;
	if (self->length < VIAL_STRING_STATIC_SIZE) {
		if (new_length < VIAL_STRING_STATIC_SIZE)
			goto finish;
		needed_capacity = new_length < VIAL_STRING_STATIC_SIZE * 2
			? VIAL_STRING_STATIC_SIZE * 2 : new_length + 1;
		char *buffer = malloc(needed_capacity);
		memcpy(buffer, self->str.static_buf, needed_capacity);
		self->str.dynamic.alloc = needed_capacity;
		self->str.dynamic.buf = buffer;
	} else {
		if (new_length < VIAL_STRING_STATIC_SIZE) {
			char *buffer = self->str.dynamic.buf;
			memcpy(self->str.static_buf, buffer, new_length + 1);
			goto finish;
		}
		if (new_length < self->str.dynamic.alloc)
			goto finish;
		needed_capacity = self->str.dynamic.alloc * 2;
		self->str.dynamic.alloc = needed_capacity;
		self->str.dynamic.buf = realloc(self->str.dynamic.buf, needed_capacity);
	}
finish:
	self->length = new_length;
}

char vial_string_pop(struct vial_string *self)
{
	const char res = vial_string_cstr(self)[self->length - 1];
	set_length(self, self->length - 1);
	vial_string_cstr_mut(self)[self->length] = '\0';
	return res;
}

void vial_string_push(struct vial_string *self, char c)
{
	set_length(self, self->length + 1);
	vial_string_cstr_mut(self)[self->length - 1] = c;
	vial_string_cstr_mut(self)[self->length] = '\0';
}

void vial_string_append(struct vial_string *self, const struct vial_string *s)
{
	vial_string_append_arr(self, vial_string_cstr(s), s->length);
}

void vial_string_append_cstr(struct vial_string *self, const char *s)
{
	vial_string_append_arr(self, s, strlen(s));
}

void vial_string_append_arr(struct vial_string *self, const char *s, size_t len)
{
	set_length(self, self->length + len);
	memcpy(vial_string_cstr_mut(self) + (self->length - len), s, len + 1);
}

void vial_string_insert(struct vial_string *self, size_t index, const struct vial_string *s)
{
	vial_string_insert_arr(self, index, vial_string_cstr(s), s->length);
}

void vial_string_insert_cstr(struct vial_string *self, size_t index, const char *s)
{
	vial_string_insert_arr(self, index, s, strlen(s));
}

void vial_string_insert_arr(struct vial_string *self, size_t index, const char *s, size_t len)
{
	if (index > self->length)
		return;
	const size_t old_length = self->length;
	set_length(self, self->length + len);
	memmove(vial_string_cstr_mut(self) + index + len, vial_string_cstr(self) + index, old_length - index);
	memcpy(vial_string_cstr_mut(self) + index, s, len);
	vial_string_cstr_mut(self)[self->length] = '\0';
}

void vial_string_erase(struct vial_string *self, size_t index, size_t count)
{
	if (index + count > self->length)
		return;
	memmove(vial_string_cstr_mut(self) + index, vial_string_cstr(self) + index + count, self->length - index - count);
	set_length(self, self->length - count);
	vial_string_cstr_mut(self)[self->length] = '\0';
}
