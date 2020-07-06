/*
Copyright (c) 2020 Pavlos Georgiou

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
