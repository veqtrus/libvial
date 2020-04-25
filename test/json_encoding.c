/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <stdio.h>
#include <string.h>

#include <vial/encoding/json.h>

int main()
{
	struct vial_string encoded;
	struct vial_json json;
	const char *tests[] = {
		"{}", "[]", "\"hello\"", "{\"hello\":\"world\"}", "[\"hello\",\"world\"]", "[{},[3,4]]",
		NULL
	};
	for (const char **test = tests; *test; test++) {
		json = vial_json_decode(*test);
		encoded = vial_json_encode(&json, "\t");
		vial_json_dispose(&json);
		json = vial_json_decode(vial_string_cstr(&encoded));
		vial_string_clear(&encoded);
		encoded = vial_json_encode(&json, NULL);
		if (strcmp(vial_string_cstr(&encoded), *test)) {
			printf("json test failed: %s -> %s\n", *test, vial_string_cstr(&encoded));
		} else {
			printf("json test passed: %s\n", *test);
		}
		vial_string_clear(&encoded);
	}
	return 0;
}
