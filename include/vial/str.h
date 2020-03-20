/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_STR_H
#define VIAL_STR_H

#include <stddef.h>

#define SSTR_MAX_SIZE 255

typedef struct sstr {
	size_t len;
	char str[256];
} sstr_t;

char *string_dup(const char *s);

sstr_t *sstr_init(sstr_t *dst, const char *src);

sstr_t *sstr_append(sstr_t *dst, const sstr_t *src);

sstr_t *sstr_cat(sstr_t *dst, const char *src);

sstr_t *sstr_split(sstr_t *fst, sstr_t *snd, size_t i);

#endif
