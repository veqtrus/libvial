/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_DEF_H
#define VIAL_DEF_H

#define vial_ptr_diff(hi, lo) (((const char *) (hi)) - ((const char *) (lo)))

#define VIAL_BIN_O(N) ( ((N) & 1) \
	| (((N) >> 2) & 2) \
	| (((N) >> 4) & 4) \
	| (((N) >> 6) & 8) \
	| (((N) >> 8) & 16) \
	| (((N) >> 10) & 32) \
	| (((N) >> 12) & 64) \
	| (((N) >> 14) & 128) )

#define VIAL_BIN(N) VIAL_BIN_O(0 ## N)

#endif
