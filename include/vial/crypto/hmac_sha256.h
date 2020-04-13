/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_CRYPTO_HMAC_SHA256_H
#define VIAL_CRYPTO_HMAC_SHA256_H

#include <vial/crypto/sha256.h>

struct vial_hmac_sha256 {
	struct vial_sha256 hasher;
	uint32_t k_pad[SHA256_BLOCK_SIZE / 4];
};

void vial_hmac_sha256_init(struct vial_hmac_sha256 *self, const void *key, size_t keylen);

void vial_hmac_sha256_update(struct vial_hmac_sha256 *self, const void *in, size_t inlen);

void vial_hmac_sha256_done(struct vial_hmac_sha256 *self, void *out);

void vial_hmac_sha256_mac(void *out, const void *key, size_t keylen, const void *in, size_t inlen);

#endif
