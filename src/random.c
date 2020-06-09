/*
Copyright (c) 2019-20 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/random.h>

#include <stddef.h>
#include <math.h>

uint32_t vial_random_range(struct vial_random *self, uint32_t range)
{
	if (range == 0)
		return 0;
	const uint32_t threshold = (-range) % range;
	uint32_t r;
	do {
		r = vial_random_int(self);
	} while (r < threshold);
	return r % range;
}

double vial_random_normal(struct vial_random *self, double *second)
{
	double x, y, s;
	do {
		x = ((_vial_random_next(self) >> 8) * VIAL_RANDOM_INVP(55)) - 1.0;
		y = ((_vial_random_next(self) >> 8) * VIAL_RANDOM_INVP(55)) - 1.0;
		s = x * x + y * y;
	} while (s <= 0.0 || s >= 1.0);
	s = sqrt(-2.0 * log(s) / s);
	if (second != NULL)
		*second = y * s;
	return x * s;
}
