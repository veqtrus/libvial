/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_CRYPTO_SHA256_H
#define VIAL_CRYPTO_SHA256_H

#include <stddef.h>
#include <stdint.h>

#define SHA256_BLOCK_SIZE 64

struct vial_sha256 {
	uint64_t length;
	unsigned curlen;
	uint32_t state[8];
	uint8_t buf[SHA256_BLOCK_SIZE];
};

void vial_sha256_init(struct vial_sha256 *self);

void vial_sha256_update(struct vial_sha256 *self, const uint8_t *in, size_t inlen);

void vial_sha256_done(struct vial_sha256 *self, uint8_t *out);

void vial_sha256_hash(void *out, const void *in, size_t inlen);

#endif
