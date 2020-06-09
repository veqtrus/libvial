/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <stdio.h>

#include <vial/random.h>

int main(int argc, char **argv)
{
	struct vial_random rng;
	uint32_t n;
	vial_random_seed(&rng, 0);
	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'o') {
		do {
			n = vial_random_int(&rng);
		} while (fwrite(&n, sizeof(n), 1, stdout) > 0);
	}
	return 0;
}
