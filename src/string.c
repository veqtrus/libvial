/*
Copyright (c) 2019-2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/string.h>

#include <stdlib.h>
#include <string.h>

char *string_dup(const char *s)
{
	if (s == NULL) return NULL;
	size_t len = strlen(s) + 1;
	char *res = malloc(len);
	memcpy(res, s, len);
	return res;
}

void vString_init(struct vString *self, const char *s)
{
	if (s == NULL) {
		self->length = 0;
		self->str.static_buf[0] = '\0';
		return;
	}
	self->length = strlen(s);
	if (self->length < VSTRING_STATIC_SIZE) {
		memcpy(self->str.static_buf, s, self->length + 1);
	} else {
		self->str.dynamic.alloc = self->length + 1;
		self->str.dynamic.buf = malloc(self->str.dynamic.alloc);
		memcpy(self->str.dynamic.buf, s, self->str.dynamic.alloc);
	}
}

void vString_clear(struct vString *self)
{
	if (self->length >= VSTRING_STATIC_SIZE)
		free(self->str.dynamic.buf);
	self->length = 0;
	self->str.static_buf[0] = '\0';
}

static void set_length(struct vString *self, size_t new_length)
{
	size_t needed_capacity;
	if (self->length < VSTRING_STATIC_SIZE) {
		if (new_length < VSTRING_STATIC_SIZE)
			goto finish;
		needed_capacity = new_length < VSTRING_STATIC_SIZE * 2
			? VSTRING_STATIC_SIZE * 2 : new_length + 1;
		char *buffer = malloc(needed_capacity);
		memcpy(buffer, self->str.static_buf, needed_capacity);
		self->str.dynamic.alloc = needed_capacity;
		self->str.dynamic.buf = buffer;
	} else {
		if (new_length < VSTRING_STATIC_SIZE) {
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

char vString_pop(struct vString *self)
{
	const char res = vString_cstr(self)[self->length - 1];
	set_length(self, self->length - 1);
	vString_cstr_mut(self)[self->length] = '\0';
	return res;
}

void vString_push(struct vString *self, char c)
{
	set_length(self, self->length + 1);
	vString_cstr_mut(self)[self->length - 1] = c;
	vString_cstr_mut(self)[self->length] = '\0';
}

void vString_append(struct vString *self, const struct vString *s)
{
	vString_append_arr(self, vString_cstr(s), s->length);
}

void vString_append_cstr(struct vString *self, const char *s)
{
	vString_append_arr(self, s, strlen(s));
}

void vString_append_arr(struct vString *self, const char *s, size_t len)
{
	set_length(self, self->length + len);
	memcpy(vString_cstr_mut(self) + (self->length - len), s, len + 1);
}

void vString_insert(struct vString *self, size_t index, const struct vString *s)
{
	vString_insert_arr(self, index, vString_cstr(s), s->length);
}

void vString_insert_cstr(struct vString *self, size_t index, const char *s)
{
	vString_insert_arr(self, index, s, strlen(s));
}

void vString_insert_arr(struct vString *self, size_t index, const char *s, size_t len)
{
	if (index > self->length)
		return;
	const size_t old_length = self->length;
	set_length(self, self->length + len);
	memmove(vString_cstr_mut(self) + index + len, vString_cstr(self) + index, old_length - index);
	memcpy(vString_cstr_mut(self) + index, s, len);
	vString_cstr_mut(self)[self->length] = '\0';
}

void vString_erase(struct vString *self, size_t index, size_t count)
{
	if (index + count > self->length)
		return;
	memmove(vString_cstr_mut(self) + index, vString_cstr(self) + index + count, self->length - index - count);
	set_length(self, self->length - count);
	vString_cstr_mut(self)[self->length] = '\0';
}
