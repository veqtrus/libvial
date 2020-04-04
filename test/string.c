/*
Copyright (c) 2019-2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <stdio.h>

#include <vial/string.h>

int main()
{
	struct vial_string str;
	vial_string_init(&str, "Hello World");
	puts(vial_string_cstr(&str));
	vial_string_push(&str, '!');
	puts(vial_string_cstr(&str));
	vial_string_pop(&str);
	puts(vial_string_cstr(&str));
	vial_string_append_cstr(&str, " and Everyone!");
	puts(vial_string_cstr(&str));
	for (int i = 0; i < 16; i++)
		vial_string_append_cstr(&str, " ~");
	puts(vial_string_cstr(&str));
	vial_string_erase(&str, 5, 10);
	puts(vial_string_cstr(&str));
	vial_string_insert_cstr(&str, 5, ",");
	puts(vial_string_cstr(&str));
	for (int i = 0; i < 32; i++)
		vial_string_pop(&str);
	puts(vial_string_cstr(&str));
	vial_string_clear(&str);
	puts(vial_string_cstr(&str));
	return 0;
}
