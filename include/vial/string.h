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

typedef const char *vial_string;

vial_string vial_string_new(const char *s);

static inline size_t vial_string_length(vial_string self)
{
	const size_t *len = (const size_t *) self;
	return *(len - 1);
}

void vial_string_take(vial_string self);

void vial_string_leave(vial_string self);

vial_string vial_string_from_char(int value);
vial_string vial_string_from_char_array(const char *start, size_t length);
vial_string vial_string_from_double(double value);
vial_string vial_string_from_float(float value);
vial_string vial_string_from_int(int value);
vial_string vial_string_from_long(long value);
vial_string vial_string_from_uint(unsigned int value);
vial_string vial_string_from_ulong(unsigned long value);
vial_string vial_string_from_size(size_t value);

vial_string vial_string_concat(const char *a, const char *b);

#endif
