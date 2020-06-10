/*
Copyright (c) 2019-2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_STRING_H
#define VIAL_STRING_H

#include <stddef.h>

char *vial_strdup(const char *s);

int vial_cstr_starts(const char *s, const char *b);

char *vial_cstr_reverse(char *s);

#define VIAL_STRING_STATIC_SIZE 16

struct vial_string {
	size_t length;
	union {
		char static_buf[VIAL_STRING_STATIC_SIZE];
		struct {
			size_t alloc;
			char *buf;
		} dynamic;
	} str;
};

void vial_string_init(struct vial_string *self, const char *s);

void vial_string_init_copy(struct vial_string *self, const struct vial_string *s);

static inline const char *vial_string_cstr(const struct vial_string *self)
{
	return self->length < VIAL_STRING_STATIC_SIZE ? self->str.static_buf : self->str.dynamic.buf;
}

static inline char *vial_string_cstr_mut(struct vial_string *self)
{
	return self->length < VIAL_STRING_STATIC_SIZE ? self->str.static_buf : self->str.dynamic.buf;
}

static inline char vial_string_at(const struct vial_string *self, size_t index)
{
	return vial_string_cstr(self)[index];
}

static inline char vial_string_get(const struct vial_string *self, size_t index)
{
	return index < self->length ? vial_string_cstr(self)[index] : '\0';
}

static inline void vial_string_set(struct vial_string *self, size_t index, char c)
{
	if (index < self->length)
		vial_string_cstr_mut(self)[index] = c;
}

void vial_string_clear(struct vial_string *self);

char vial_string_pop(struct vial_string *self);

void vial_string_push(struct vial_string *self, char c);

void vial_string_append(struct vial_string *self, const struct vial_string *s);

void vial_string_append_cstr(struct vial_string *self, const char *s);

void vial_string_append_arr(struct vial_string *self, const char *s, size_t len);

void vial_string_insert(struct vial_string *self, size_t index, const struct vial_string *s);

void vial_string_insert_cstr(struct vial_string *self, size_t index, const char *s);

void vial_string_insert_arr(struct vial_string *self, size_t index, const char *s, size_t len);

void vial_string_erase(struct vial_string *self, size_t index, size_t count);

#endif
