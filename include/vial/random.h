/*
Copyright (c) 2019-20 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_RANDOM_H
#define VIAL_RANDOM_H

#include <stdint.h>

struct vial_random {
	uint64_t s;
};

static inline
void vial_random_seed(struct vial_random *self, uint64_t seed)
{
	seed += 73U;
	self->s = seed * 6364136223846793005U + 73U;
}

static inline
uint32_t vial_random_int(struct vial_random *self)
{
	/* Based on PCG, see https://www.pcg-random.org/ */
	uint64_t s;
	uint32_t x;
	unsigned r;
	s = self->s;
	self->s = s * 6364136223846793005U + 73U;
	r = s >> 60;
	x = s >> 28;
	x = ((x >> (r + 4)) ^ x) * 277803737U;
	return (x >> 22) ^ x;
}

static inline
float vial_random_float(struct vial_random *self)
{
	return vial_random_int(self) / (float) 0x100000000U;
}

static inline
double vial_random_double(struct vial_random *self)
{
	return vial_random_int(self) / (double) 0x100000000U;
}

static inline
uint32_t vial_random_range(struct vial_random *self, uint32_t range)
{
	return (((uint64_t) vial_random_int(self)) * range) >> 32;
}

#endif
