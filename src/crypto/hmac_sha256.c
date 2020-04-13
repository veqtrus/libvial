/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/crypto/hmac_sha256.h>

#include <string.h>

#define IPAD 0x36363636UL
#define OPAD 0x5C5C5C5CUL

void vial_hmac_sha256_init(struct vial_hmac_sha256 *self, const void *key, size_t keylen)
{
	int i;
	memset(self->k_pad, 0, SHA256_BLOCK_SIZE);
	if (keylen > SHA256_BLOCK_SIZE) {
		vial_sha256_init(&self->hasher);
		vial_sha256_update(&self->hasher, key, keylen);
		vial_sha256_done(&self->hasher, (uint8_t *) self->k_pad);
	} else {
		memcpy(self->k_pad, key, keylen);
	}
	for (i = 0; i < SHA256_BLOCK_SIZE / 4; ++i)
		self->k_pad[i] ^= IPAD;
	/* we have k_ipad for now */
	vial_sha256_init(&self->hasher);
	vial_sha256_update(&self->hasher, (const uint8_t *) self->k_pad, SHA256_BLOCK_SIZE);
	/* xor again to get k_opad */
	for (i = 0; i < SHA256_BLOCK_SIZE / 4; ++i)
		self->k_pad[i] ^= IPAD ^ OPAD;
}

void vial_hmac_sha256_update(struct vial_hmac_sha256 *self, const void *in, size_t inlen)
{
	vial_sha256_update(&self->hasher, in, inlen);
}

void vial_hmac_sha256_done(struct vial_hmac_sha256 *self, void *out)
{
	vial_sha256_done(&self->hasher, out);
	vial_sha256_init(&self->hasher);
	vial_sha256_update(&self->hasher, (const uint8_t *) self->k_pad, SHA256_BLOCK_SIZE);
	vial_sha256_update(&self->hasher, out, 32);
	vial_sha256_done(&self->hasher, out);
}

void vial_hmac_sha256_mac(void *out, const void *key, size_t keylen, const void *in, size_t inlen)
{
	struct vial_hmac_sha256 mac;
	vial_hmac_sha256_init(&mac, key, keylen);
	vial_hmac_sha256_update(&mac, in, inlen);
	vial_hmac_sha256_done(&mac, out);
}
