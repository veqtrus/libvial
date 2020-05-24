/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <stdio.h>

#include <vial/random.h>

static uint8_t rng8(uint16_t *state)
{
	/* Based on PCG, see https://www.pcg-random.org/ */
	uint16_t s;
	uint8_t x;
	unsigned r1, r2;
	s = *state;
	*state = s * 8121U + 73U;
	r1 = s >> 14;
	r2 = (s >> 12) & 3;
	/* random shift */
	x = s >> (1 + r1);
	/* random xorshift */
	x ^= x >> (2 + r2);
	return x;
}

static void test8()
{
	uint32_t count;
	uint16_t state = 54321;
	uint8_t x, y, map[256 * 256] = { 0 };
	puts("P2\n256 256\n255");
	for (count = 0; count <= INT16_MAX; count++) {
		x = rng8(&state);
		y = rng8(&state);
		map[((int) y) * 256 + x] = 128 + (count >> 8);
	}
	for (count = 0; count <= UINT16_MAX; count++) {
		printf("%4d", (int) map[count]);
	}
}

int main(int argc, char **argv)
{
	struct vial_random rng;
	uint32_t n;
	vial_random_seed(&rng, 0);
	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'o') {
// 		test8();
// 		return 0;
		do {
			n = vial_random_int(&rng);
		} while (fwrite(&n, sizeof(n), 1, stdout) > 0);
	}
	return 0;
}
