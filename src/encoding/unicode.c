/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/encoding/unicode.h>

const uint8_t vial_utf8_transitions[256] = {
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x78,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,0x80,0x80,0x80,0x38,0x3C,0x3D,0x3E,
	0x80,0x80,0x80,0x80,0x38,0x3C,0x3D,0x3E,
	0x80,0x80,0x80,0x80,0x38,0x3C,0x3D,0x3E,
	0x80,0x80,0x80,0x80,0x38,0x3C,0x3D,0x3E,
	0x80,0x80,0x80,0x80,0x38,0x3C,0x3D,0x3E,
	0x80,0x80,0x80,0x80,0x38,0x3C,0x3D,0x3E,
	0x80,0x80,0x80,0x80,0x38,0x3C,0x3D,0x3E,
	0x80,0x80,0x80,0x80,0x38,0x3C,0x3D,0x3E,
	0x1C,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x1C,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x1C,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x1C,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x0D,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x0D,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x06,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80
};