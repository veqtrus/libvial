/*
Copyright (c) 2019-20 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_RANDOM_H
#define VIAL_RANDOM_H

#include <stdint.h>

#define VIAL_RANDOM_INVP(N) (1.0 / (1ULL << (N)))
#define VIAL_RANDOM_PCG_MUL 6364136223846793005U
#define VIAL_RANDOM_PCG_INC 73U

struct vial_random {
	uint64_t s;
};

static inline
uint64_t _vial_random_next(struct vial_random *self)
{
	/* Based on PCG, see https://www.pcg-random.org/ */
	uint64_t x;
	unsigned r;
	x = self->s;
	self->s = x * VIAL_RANDOM_PCG_MUL + VIAL_RANDOM_PCG_INC;
	r = (x >> 60) + 4;
	x ^= x >> 23;
	return x ^ (x << r);
}

static inline
void vial_random_seed(struct vial_random *self, uint64_t seed)
{
	seed += VIAL_RANDOM_PCG_INC;
	self->s = seed * VIAL_RANDOM_PCG_MUL + VIAL_RANDOM_PCG_INC;
}

static inline
uint32_t vial_random_int(struct vial_random *self)
{
	return _vial_random_next(self) >> 32;
}

static inline
float vial_random_float(struct vial_random *self)
{
	return vial_random_int(self) * (float) VIAL_RANDOM_INVP(32);
}

static inline
double vial_random_double(struct vial_random *self)
{
	return (_vial_random_next(self) >> 8) * VIAL_RANDOM_INVP(56);
}

uint32_t vial_random_range(struct vial_random *self, uint32_t range);

static inline
uint32_t vial_random_between(struct vial_random *self, uint32_t incl_min, uint32_t excl_max)
{
	return incl_min + vial_random_range(self, excl_max - incl_min);
}

double vial_random_normal(struct vial_random *self, double *second);

#endif
