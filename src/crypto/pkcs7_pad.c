/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/crypto/pkcs7_pad.h>

size_t vial_pkcs7_pad(uint8_t *buf, size_t len, unsigned blklen)
{
	const unsigned count = 16 - (blklen & 15);
	buf += len;
	for (unsigned i = 0; i < count; i++)
		buf[i] = count;
	return len + count;
}
