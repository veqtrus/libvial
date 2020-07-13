/*
Copyright (c) 2019-2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vial/sharedptr.h>

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

char *vial_cstr_reverse(char *s)
{
	char t, *l, *r;
	l = s;
	r = s + strlen(s) - 1;
	for (; l < r; ++l, --r) {
		t = *l; *l = *r; *r = t;
	}
	return s;
}

vial_string vial_string_new(const char *s)
{
	size_t len = 0;
	size_t *plen;
	char *buf;
	if (s != NULL)
		len = strlen(s);
	plen = vial_sharedptr_malloc(len + sizeof(size_t) + 1);
	*plen = len;
	buf = (char *) (plen + 1);
	if (s == NULL)
		buf[0] = 0;
	else
		memcpy(buf, s, len + 1);
	return buf;
}

void vial_string_take(vial_string self)
{
	union {
		vial_string str;
		size_t *sz;
	} ptr;
	ptr.str = self;
	vial_sharedptr_take(ptr.sz - 1);
}

void vial_string_leave(vial_string self)
{
	union {
		vial_string str;
		size_t *sz;
	} ptr;
	ptr.str = self;
	vial_sharedptr_leave(ptr.sz - 1);
}

vial_string vial_string_from_char(int value)
{
	size_t *plen;
	char *buf;
	plen = vial_sharedptr_malloc(sizeof(size_t) + 2);
	*plen = 1;
	buf = (char *) (plen + 1);
	buf[0] = value;
	buf[1] = 0;
	return buf;
}

vial_string vial_string_from_char_array(const char *start, size_t length)
{
	size_t *plen;
	char *buf;
	plen = vial_sharedptr_malloc(length + sizeof(size_t) + 1);
	*plen = length;
	buf = (char *) (plen + 1);
	if (length != 0)
		memcpy(buf, start, length);
	buf[length] = 0;
	return buf;
}

vial_string vial_string_from_double(double value)
{
	char buf[32];
	sprintf(buf, "%f", value);
	return vial_string_new(buf);
}

vial_string vial_string_from_float(float value)
{
	char buf[32];
	sprintf(buf, "%f", value);
	return vial_string_new(buf);
}

vial_string vial_string_from_int(int value)
{
	char buf[32];
	sprintf(buf, "%d", value);
	return vial_string_new(buf);
}

vial_string vial_string_from_long(long value)
{
	char buf[32];
	sprintf(buf, "%ld", value);
	return vial_string_new(buf);
}

vial_string vial_string_from_uint(unsigned int value)
{
	char buf[32];
	sprintf(buf, "%u", value);
	return vial_string_new(buf);
}

vial_string vial_string_from_ulong(unsigned long value)
{
	char buf[32];
	sprintf(buf, "%lu", value);
	return vial_string_new(buf);
}

vial_string vial_string_from_size(size_t value)
{
	char buf[32];
	sprintf(buf, "%zu", value);
	return vial_string_new(buf);
}

vial_string vial_string_concat(const char *a, const char *b)
{
	size_t len, len_a = 0, len_b = 0;
	size_t *plen;
	char *buf;
	if (a != NULL)
		len_a = strlen(a);
	if (b != NULL)
		len_b = strlen(b);
	len = len_a + len_b;
	plen = vial_sharedptr_malloc(len + sizeof(size_t) + 1);
	*plen = len;
	buf = (char *) (plen + 1);
	if (a != NULL)
		memcpy(buf, a, len_a);
	if (b != NULL)
		memcpy(buf + len_a, b, len_b);
	buf[len] = 0;
	return buf;
}
