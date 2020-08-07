/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/func.h>

#include <string.h>

void vial_dispose_nop(void *p) { }

int vial_comp_cstr(const void *l, const void *r)
{
	return strcmp(
		* (const char *const *) l,
		* (const char *const *) r
	);
}
