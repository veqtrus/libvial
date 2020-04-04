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

#ifndef vial_isort

#define vial_isort(ptr, len, tmp) do { \
		for (int vial_isort_i = 1; vial_isort_i < (len); ++vial_isort_i) { \
			(tmp) = (ptr)[vial_isort_i]; \
			for (int vial_isort_j = vial_isort_i - 1; vial_isort_j >= 0 && (ptr)[vial_isort_j] > (tmp); --vial_isort_j) \
				(ptr)[vial_isort_j + 1] = (ptr)[vial_isort_j]; \
			(ptr)[vial_isort_j] = (tmp); \
		} \
	} while (0)

#endif
