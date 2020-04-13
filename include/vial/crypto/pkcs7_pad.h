/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_PKCS7_PAD_H
#define VIAL_PKCS7_PAD_H

#include <stddef.h>
#include <stdint.h>

size_t vial_pkcs7_pad(uint8_t *buf, size_t len, unsigned blklen);

static inline size_t vial_pkcs7_size(uint8_t *buf, size_t len)
{
	return len - buf[len - 1];
}

#endif
