/*
Copyright (c) 2019-2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_STRING_H
#define VIAL_STRING_H

#include <stddef.h>

char *string_dup(const char *s);

#define VSTRING_STATIC_SIZE 16

struct vString {
	size_t length;
	union {
		char static_buf[VSTRING_STATIC_SIZE];
		struct {
			size_t alloc;
			char *buf;
		} dynamic;
	} str;
};

void vString_init(struct vString *self, const char *s);

static inline const char *vString_cstr(const struct vString *self)
{
	return self->length < VSTRING_STATIC_SIZE ? self->str.static_buf : self->str.dynamic.buf;
}

static inline char *vString_cstr_mut(struct vString *self)
{
	return self->length < VSTRING_STATIC_SIZE ? self->str.static_buf : self->str.dynamic.buf;
}

static inline char vString_get(const struct vString *self, size_t index)
{
	return index < self->length ? vString_cstr(self)[index] : '\0';
}

static inline void vString_set(struct vString *self, size_t index, char c)
{
	if (index < self->length)
		vString_cstr_mut(self)[index] = c;
}

void vString_clear(struct vString *self);

char vString_pop(struct vString *self);

void vString_push(struct vString *self, char c);

void vString_append(struct vString *self, const struct vString *s);

void vString_append_cstr(struct vString *self, const char *s);

void vString_append_arr(struct vString *self, const char *s, size_t len);

void vString_insert(struct vString *self, size_t index, const struct vString *s);

void vString_insert_cstr(struct vString *self, size_t index, const char *s);

void vString_insert_arr(struct vString *self, size_t index, const char *s, size_t len);

void vString_erase(struct vString *self, size_t index, size_t count);

#endif
