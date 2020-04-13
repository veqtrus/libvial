/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/crypto/sha256.h>

#include <string.h>

/* This is based on SHA256 implementation in LibTomCrypt that was released into
 * public domain by Tom St Denis. */

#define GET_BE32(buf) ( \
	(((uint32_t) (buf)[0]) << 24) | \
	(((uint32_t) (buf)[1]) << 16) | \
	(((uint32_t) (buf)[2]) << 8) | \
	((uint32_t) (buf)[3]) )
#define PUT_BE32(buf, val) do { \
		(buf)[0] = ((val) >> 24) & 0xFF; \
		(buf)[1] = ((val) >> 16) & 0xFF; \
		(buf)[2] = ((val) >> 8) & 0xFF; \
		(buf)[3] = (val) & 0xFF; \
	} while (0)
#define PUT_BE64(buf, val) do { \
		(buf)[0] = ((val) >> 56) & 0xFF; \
		(buf)[1] = ((val) >> 48) & 0xFF; \
		(buf)[2] = ((val) >> 40) & 0xFF; \
		(buf)[3] = ((val) >> 32) & 0xFF; \
		(buf)[4] = ((val) >> 24) & 0xFF; \
		(buf)[5] = ((val) >> 16) & 0xFF; \
		(buf)[6] = ((val) >> 8) & 0xFF; \
		(buf)[7] = (val) & 0xFF; \
	} while (0)

/* the K array */
static const uint32_t K[64] = {
	0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL,
	0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL,
	0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL,
	0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
	0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL, 0x983e5152UL,
	0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
	0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL,
	0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
	0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL,
	0xd6990624UL, 0xf40e3585UL, 0x106aa070UL, 0x19a4c116UL, 0x1e376c08UL,
	0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL,
	0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
	0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

/* Various logical functions */
#define Ch(x, y, z)     (z ^ (x & (y ^ z)))
#define Maj(x, y, z)    (((x | y) & z) | (x & y))
#define RR(x, n)        ((x >> n) | (x << (32 - n)))
#define SR(x, n)        (x >> n)
#define Sigma0(x)       (RR(x, 2) ^ RR(x, 13) ^ RR(x, 22))
#define Sigma1(x)       (RR(x, 6) ^ RR(x, 11) ^ RR(x, 25))
#define sigma0(x)       (RR(x, 7) ^ RR(x, 18) ^ SR(x, 3))
#define sigma1(x)       (RR(x, 17) ^ RR(x, 19) ^ SR(x, 10))
#ifndef MIN
#define MIN(x, y)       (((x) < (y)) ? (x) : (y))
#endif

/* compress 512-bits */
static void sha256_compress(struct vial_sha256 *md, const uint8_t *buf)
{
	int i;
	uint32_t a, b, c, d, e, f, g, h,
		T1, T2, W[64];
	/* copy state */
	a = md->state[0];
	b = md->state[1];
	c = md->state[2];
	d = md->state[3];
	e = md->state[4];
	f = md->state[5];
	g = md->state[6];
	h = md->state[7];
	/* copy 512 bits from buffer into W[0..15] */
	for (i = 0; i < 16; ++i) {
		W[i] = GET_BE32(buf);
		buf += 4;
	}
	/* fill W[16..63] */
	for (i = 16; i < 64; ++i)
		W[i] = sigma1(W[i - 2]) + W[i - 7] + sigma0(W[i - 15]) + W[i - 16];
	/* compress */
	for (i = 0; i < 64; ++i) {
		T1 = h + Sigma1(e) + Ch(e, f, g) + K[i] + W[i];
		T2 = Sigma0(a) + Maj(a, b, c);
		h = g; g = f; f = e;
		e = d + T1;
		d = c; c = b; b = a;
		a = T1 + T2;
	}
	/* feedback */
	md->state[0] += a;
	md->state[1] += b;
	md->state[2] += c;
	md->state[3] += d;
	md->state[4] += e;
	md->state[5] += f;
	md->state[6] += g;
	md->state[7] += h;
}

/* Initialize the hash state */
void vial_sha256_init(struct vial_sha256 *self)
{
	self->length = 0;
	self->curlen = 0;
	self->state[0] = 0x6A09E667UL;
	self->state[1] = 0xBB67AE85UL;
	self->state[2] = 0x3C6EF372UL;
	self->state[3] = 0xA54FF53AUL;
	self->state[4] = 0x510E527FUL;
	self->state[5] = 0x9B05688CUL;
	self->state[6] = 0x1F83D9ABUL;
	self->state[7] = 0x5BE0CD19UL;
}

/**
 * Process a block of memory though the hash
 * @param md     The hash state
 * @param in     The data to hash
 * @param inlen  The length of the data (octets)
 */
void vial_sha256_update(struct vial_sha256 *self, const uint8_t *in, size_t inlen)
{
	size_t n;
	if (self->curlen >= sizeof(self->buf))
		return;
	while (inlen > 0) {
		if (self->curlen == 0 && inlen >= SHA256_BLOCK_SIZE) {
			sha256_compress(self, in);
			self->length += SHA256_BLOCK_SIZE * 8;
			in += SHA256_BLOCK_SIZE;
			inlen -= SHA256_BLOCK_SIZE;
		} else {
			n = MIN(inlen, (SHA256_BLOCK_SIZE - self->curlen));
			memcpy(self->buf + self->curlen, in, n);
			self->curlen += n;
			in += n;
			inlen -= n;
			if (self->curlen == SHA256_BLOCK_SIZE) {
				sha256_compress(self, self->buf);
				self->length += 8 * SHA256_BLOCK_SIZE;
				self->curlen = 0;
			}
		}
	}
}

/**
 * Terminate the hash to get the digest
 * @param md  The hash state
 * @param out [out] The destination of the hash (32 bytes)
 */
void vial_sha256_done(struct vial_sha256 *self, uint8_t *out)
{
	int i;
	if (self->curlen >= sizeof(self->buf))
		return;
	/* increase the length of the message */
	self->length += self->curlen * 8;
	/* append the '1' bit */
	self->buf[self->curlen++] = (uint8_t) 0x80;
	/* if the length is currently above 56 bytes we append zeros
	 * then compress.  Then we can fall back to padding zeros and length
	 * encoding like normal.
	 */
	if (self->curlen > 56) {
		while (self->curlen < SHA256_BLOCK_SIZE)
			self->buf[self->curlen++] = (uint8_t) 0;
		sha256_compress(self, self->buf);
		self->curlen = 0;
	}
	/* pad up to 56 bytes of zeroes */
	while (self->curlen < 56)
		self->buf[self->curlen++] = (uint8_t) 0;
	/* store length */
	PUT_BE64(self->buf + 56, self->length);
	sha256_compress(self, self->buf);
	/* copy output */
	for (i = 0; i < 8; ++i) {
		PUT_BE32(out, self->state[i]);
		out += 4;
	}
}

void vial_sha256_hash(void *out, const void *in, size_t inlen)
{
	struct vial_sha256 hasher;
	vial_sha256_init(&hasher);
	vial_sha256_update(&hasher, in, inlen);
	vial_sha256_done(&hasher, out);
}
