/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vial/encoding/base_encoding.h>

static int test_base_encoding(const char *original, const char *encoded, int base)
{
	int status;
	char *buf[2];
	buf[0] = malloc(strlen(encoded) * 2);
	buf[1] = malloc(strlen(encoded) * 2);
	status = vial_base_encode(buf[0], (const uint8_t *) original, strlen(original), base);
	if (status < 0) {
		status = -1;
		goto done;
	}
	status = vial_base_decode((uint8_t *) buf[1], encoded, base);
	if (status < 0) {
		status = -2;
		goto done;
	} else {
		buf[1][status] = '\0';
	}
	if (strcmp(buf[0], encoded)) {
		status = 1;
		goto done;
	}
	if (strcmp(buf[1], original)) {
		status = 2;
		goto done;
	}
	status = 0;
done:
	free(buf[0]);
	free(buf[1]);
	return status;
}

static void print_test_result(int status)
{
	switch (status) {
	case -1:
		puts("failed encoding");
		return;
	case -2:
		puts("failed decoding");
		return;
	case 1:
		puts("incorrect encoding");
		return;
	case 2:
		puts("incorrect decoding");
		return;
	case 0:
		puts("succeded");
		return;
	default:
		puts("unknown status");
		return;
	}
}

int main() {
	struct test {
		const char *original, *encoded;
		int expected, base;
	} tests[] = {
		{ "\x41\x4F\x09", "414F09", 0, 16 },
		{ "\x41\x4F\x09", "414f09", 0, -16 },
		{ "", "123", -2, 16 },
		{ "any carnal pleasure.", "YW55IGNhcm5hbCBwbGVhc3VyZS4=", 0, 64 },
		{ "any carnal pleasure", "YW55IGNhcm5hbCBwbGVhc3VyZQ==", 0, 64 },
		{ "any carnal pleasur", "YW55IGNhcm5hbCBwbGVhc3Vy", 0, 64 },
		{ "any carnal pleasu", "YW55IGNhcm5hbCBwbGVhc3U=", 0, 64 },
		{ "any carnal pleasu", "YW55IGNhcm5hbCBwbGVhc3U", 0, -64 },
		{ NULL }
	};
	for (struct test *t = tests; t->original != NULL; t++) {
		int status = test_base_encoding(t->original, t->encoded, t->base);
		if (status != t->expected) {
			printf("FAILED: \"%s\" -> \"%s\"\n", t->original, t->encoded);
			printf("  status: ");
			print_test_result(status);
			printf("  expected: ");
			print_test_result(t->expected);
		}
	}
	return 0;
}
