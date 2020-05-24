/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <stdio.h>

#include <vial/def.h>
#include <vial/random.h>
#include <vial/encoding/unicode.h>

static void print_utf8_transitions()
{
	int top_bits, state, mask, new_state;
	for (top_bits = 0; top_bits < 32; top_bits++) {
		for (state = 0; state < 8; state++) {
			if (state == 0) {
				if ((top_bits & VIAL_BIN(10000)) == 0) {
					mask = VIAL_BIN(01111);
					new_state = 0;
				} else if ((top_bits & VIAL_BIN(11100)) == VIAL_BIN(11000)) {
					mask = VIAL_BIN(00011);
					new_state = 4;
				} else if ((top_bits & VIAL_BIN(11110)) == VIAL_BIN(11100)) {
					mask = VIAL_BIN(00001);
					new_state = 5;
				} else if ((top_bits & VIAL_BIN(11111)) == VIAL_BIN(11110)) {
					mask = 0;
					new_state = 6;
				} else {
					mask = VIAL_BIN(10000);
					new_state = 0;
				}
			} else if (state >= 4 && (top_bits & VIAL_BIN(11000)) == VIAL_BIN(10000)) {
				mask = VIAL_BIN(00111);
				new_state = state > 4 ? state - 1 : 0;
			} else {
				mask = VIAL_BIN(10000);
				new_state = 0;
			}
			printf("0x%02X,", (mask << 3) | new_state);
		}
		puts("");
	}
}

static uint32_t randchar(struct vial_random *random)
{
	uint32_t r = vial_random_int(random);
	const unsigned c = r & 3;
	r >>= 2;
	switch (c) {
	case 0:
		return r % 128;
	case 1:
		return 128 + r % (2048 - 128);
	case 2:
		return 2048 + r % (65536 - 2048);
	case 3:
		return 65536 + r % (131072 - 65536);
	}
	return 0;
}

static void test_utf8_random_chars()
{
	uint32_t code, sum;
	uint8_t buffer[1000];
	uint8_t *c;
	struct vial_random random;
	struct vial_unicode dec;
	vial_random_seed(&random, 0);
	c = buffer;
	while (c - buffer < sizeof(buffer) - 6) {
		code = randchar(&random);
		if ((code & 0xF800U) != 0xD800U)
			c += vial_utf8_encode(code, c);
	}
	*c = 0;
	sum = 0;
	for (int i = 0; i < 1000 * 1000; i++) {
		vial_unicode_init(&dec);
		for (c = buffer; *c; c++) {
			sum += vial_utf8_decode(&dec, &code, *c);
			sum += code;
		}
	}
	printf("UTF-8: error=%u, sum=%u\n", dec.error, sum);
}

static void test_utf16_random_chars()
{
	uint32_t code, sum;
	uint16_t buffer[500];
	uint16_t *c;
	struct vial_random random;
	struct vial_unicode dec;
	vial_random_seed(&random, 0);
	c = buffer;
	while (c - buffer < sizeof(buffer) / sizeof(uint16_t) - 4) {
		code = randchar(&random);
		if ((code & 0xF800U) != 0xD800U)
			c += vial_utf16_encode(code, c);
	}
	*c = 0;
	sum = 0;
	for (int i = 0; i < 1000 * 1000; i++) {
		vial_unicode_init(&dec);
		for (c = buffer; *c; c++) {
			sum += vial_utf16_decode(&dec, &code, *c);
			sum += code;
		}
	}
	printf("UTF-16: error=%u, sum=%u\n", dec.error, sum);
}

int main(int argc, char **argv)
{
	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 't') {
		print_utf8_transitions();
	} else {
		test_utf8_random_chars();
		test_utf16_random_chars();
	}
	return 0;
}
