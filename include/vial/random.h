/*
Copyright (c) 2019 Pavlos Georgiou

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

void vial_random_seed(struct vial_random *self, uint64_t seed);

double vial_random_unit(struct vial_random *self);

uint32_t vial_random_int(struct vial_random *self, uint32_t range);

#endif
