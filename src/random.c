/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/random.h>

#include <stdint.h>

#define RAND_XSS_CONST UINT64_C(0x2545F4914F6CDD1D)

static uint64_t xorshift64s(struct vial_random *self)
{
	/* The state must be seeded with a nonzero value. */
	uint64_t x = self->s;
	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;
	self->s = x;
	return x * RAND_XSS_CONST;
}

void vial_random_seed(struct vial_random *self, uint64_t seed)
{
	seed ^= RAND_XSS_CONST;
	self->s = seed ? seed : RAND_XSS_CONST;
}

double vial_random_unit(struct vial_random *self)
{
	const double r = xorshift64s(self) >> 8;
	return r / (UINT64_C(1) << 56);
}

uint32_t vial_random_int(struct vial_random *self, uint32_t range)
{
	return ((xorshift64s(self) >> 32) * range) >> 32;
}
