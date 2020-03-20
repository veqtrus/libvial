/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/str.h>

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

sstr_t *sstr_init(sstr_t *dst, const char *src)
{
	dst->len = 0;
	dst->str[0] = 0;
	if (src == NULL) return dst;
	size_t len = strlen(src);
	if (len > SSTR_MAX_SIZE) return NULL;
	dst->len = len;
	memcpy(dst->str, src, len + 1);
	return dst;
}

sstr_t *sstr_append(sstr_t *dst, const sstr_t *src)
{
	size_t len = dst->len + src->len;
	if (len > SSTR_MAX_SIZE) return NULL;
	memcpy(&dst->str[dst->len], src->str, src->len + 1);
	dst->len =  len;
	return dst;
}

sstr_t *sstr_cat(sstr_t *dst, const char *src)
{
	if (src == NULL) return dst;
	size_t src_len = strlen(src), len = dst->len + src_len;
	if (len > SSTR_MAX_SIZE) return NULL;
	memcpy(&dst->str[dst->len], src, src_len + 1);
	dst->len = len;
	return dst;
}

sstr_t *sstr_split(sstr_t *fst, sstr_t *snd, size_t i)
{
	if (i > fst->len) return NULL;
	snd->len = fst->len - i;
	memcpy(snd->str, &fst->str[i], snd->len);
	fst->len = i;
	fst->str[i] = 0;
	return fst;
}
