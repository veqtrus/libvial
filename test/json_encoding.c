/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vial/encoding/json.h>

static void test_encdec()
{
	char *encoded;
	struct vial_json json;
	const char *tests[] = {
		"{}", "[]", "\"hello\"", "{\"hello\":\"world\"}", "[\"hello\",\"world\"]", "[{},[3,4]]",
		NULL
	};
	for (const char **test = tests; *test; test++) {
		json = vial_json_decode(*test);
		encoded = vial_json_encode(json, "\t");
		vial_json_leave(&json);
		json = vial_json_decode(encoded);
		free(encoded);
		encoded = vial_json_encode(json, NULL);
		if (strcmp(encoded, *test)) {
			printf("json test failed: %s -> %s\n", *test, encoded);
		} else {
			printf("json test passed: %s\n", *test);
		}
		free(encoded);
	}
}

int main()
{
	test_encdec();
	return 0;
}
