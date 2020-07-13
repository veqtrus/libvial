/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <stdio.h>
#include <stdlib.h>

#include <vial/text/stringbuilder.h>
#include <vial/string.h>

int main()
{
	struct vial_stringbuilder sbuilder;
	vial_stringbuilder_init_cstr(&sbuilder, "Hello World");
	vial_stringbuilder_append_char_array(&sbuilder, "! ", 2);
	vial_stringbuilder_append_int(&sbuilder, 55);
	vial_stringbuilder_append_char(&sbuilder, ' ');
	vial_stringbuilder_append_double(&sbuilder, 1.23);
	vial_stringbuilder_append_char(&sbuilder, ' ');
	vial_string str = vial_stringbuilder_to_string(&sbuilder);
	vial_stringbuilder_insert_string(&sbuilder, 6, str);
	vial_stringbuilder_append_size(&sbuilder, 1);
	vial_string_leave(str);
	str = vial_stringbuilder_to_string(&sbuilder);
	puts(str);
	vial_string_leave(str);
	return 0;
}
