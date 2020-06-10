/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_CRYPTO_SECURE_RANDOM
#define VIAL_CRYPTO_SECURE_RANDOM

#include <stddef.h>
#include <stdbool.h>

#include <vial/crypto/chacha20.h>
#include <vial/io/stream/stream.h>

bool vial_secure_random_get(void *buf, size_t size);

VIAL_BEGIN_CLASS_DECL(vial_randstream, vial_object)
	VIAL_IMPLEMENTS(vial_stream);
	struct vial_chacha20 state;
	uint8_t bytes[sizeof(struct vial_chacha20)];
	unsigned used_bytes;
VIAL_END_CLASS_DECL;
VIAL_BEGIN_VIRTUALS(vial_randstream) VIAL_END_VIRTUALS;

vial_error vial_randstream_init(struct vial_randstream *self, const void *key, const void *nonce);

vial_error vial_randstream_init_random(struct vial_randstream *self);

#endif
