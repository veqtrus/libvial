/*
Copyright (c) 2019 Pavlos Georgiou

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

#ifndef foreach
#define foreach(i, ptr, len) for ((i) = (ptr); (i) < (ptr) + (len); ++(i))
#endif

#ifndef vial_array_reverse
#include <stddef.h>
#define vial_array_reverse(T, arr, len) do { \
	T vial_tmp; T *vial_l; T *vial_r; \
	for (vial_l = (arr), vial_r = vial_l + (len) - 1; vial_l < vial_r; ++vial_l, --vial_r) { \
		vial_tmp = *vial_l; *vial_l = *vial_r; *vial_r = vial_tmp; \
	} \
} while (0)
#endif
