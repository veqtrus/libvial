/*
Copyright (c) 2019-2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <stdio.h>

#include <vial/string.h>

int main()
{
	struct vString str;
	vString_init(&str, "Hello World");
	puts(vString_cstr(&str));
	vString_push(&str, '!');
	puts(vString_cstr(&str));
	vString_pop(&str);
	puts(vString_cstr(&str));
	vString_append_cstr(&str, " and Everyone!");
	puts(vString_cstr(&str));
	for (int i = 0; i < 16; i++)
		vString_append_cstr(&str, " ~");
	puts(vString_cstr(&str));
	vString_erase(&str, 5, 10);
	puts(vString_cstr(&str));
	vString_insert_cstr(&str, 5, ",");
	puts(vString_cstr(&str));
	for (int i = 0; i < 32; i++)
		vString_pop(&str);
	puts(vString_cstr(&str));
	vString_clear(&str);
	puts(vString_cstr(&str));
	return 0;
}
