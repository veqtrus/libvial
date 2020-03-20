/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_RANDOM_H
#define VIAL_RANDOM_H

#include <stdint.h>

struct vRandom {
	uint64_t s;
};

void vRandom_seed(struct vRandom *self, uint64_t seed);

double vRandom_unit(struct vRandom *self);

uint32_t vRandom_int(struct vRandom *self, uint32_t range);

#endif
