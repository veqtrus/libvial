/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/crypto/chacha20.h>

#include <string.h>

void vial_chacha20_init(struct vial_chacha20 *self, const void *key, const void *nonce)
{
	memcpy(self->state, "expand 32-byte k", 16);
	memcpy(self->state + 4, key, 32);
	memcpy(self->state + 13, nonce, 12);
	self->state[12] = 0;
}

#define QR(a, b, c, d) \
	a += b; d ^= a; d = (d << 16) | (d >> 16); \
	c += d; b ^= c; b = (b << 12) | (b >> 20); \
	a += b; d ^= a; d = (d << 8) | (d >> 24); \
	c += d; b ^= c; b = (b << 7) | (b >> 25)

#define ROUNDS 20

void vial_chacha20_next(struct vial_chacha20 *self, void *out)
{
	int i;
	uint32_t x[16];
	for (i = 0; i < 16; ++i)
		x[i] = self->state[i];
	for (i = 0; i < ROUNDS / 2; ++i) {
		// Odd round
		QR(x[0], x[4], x[ 8], x[12]); // column 0
		QR(x[1], x[5], x[ 9], x[13]); // column 1
		QR(x[2], x[6], x[10], x[14]); // column 2
		QR(x[3], x[7], x[11], x[15]); // column 3
		// Even round
		QR(x[0], x[5], x[10], x[15]); // diagonal 1 (main diagonal)
		QR(x[1], x[6], x[11], x[12]); // diagonal 2
		QR(x[2], x[7], x[ 8], x[13]); // diagonal 3
		QR(x[3], x[4], x[ 9], x[14]); // diagonal 4
	}
	for (i = 0; i < 16; ++i)
		x[i] += self->state[i];
	self->state[12]++; // increment counter
	memcpy(out, x, sizeof(x));
}
