/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_BASE_ENCODING_H
#define VIAL_BASE_ENCODING_H

#include <stddef.h>
#include <stdint.h>

long vial_base_encode(char *dst, const uint8_t *src, size_t len, int base);

long vial_base_decode(uint8_t *dst, const char *src, int base);

#endif
