/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/sort/msort.h>

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#if CHAR_BIT != 8
#error char not 8 bits
#endif
typedef char *byte_ptr;

static void vial_msort_merge(byte_ptr left, byte_ptr right, byte_ptr end,
	void *tmp, size_t size, vial_comp_ctx_f comp, void *context)
{
	byte_ptr start = left, mid = right, res = tmp;
	if (start < mid && comp(mid - size, mid, context) <= 0)
		return;
	while (left < mid && right < end) {
		if (comp(left, right, context) > 0) {
			memcpy(res, right, size);
			right += size;
		} else {
			memcpy(res, left, size);
			left += size;
		}
		res += size;
	}
	if (left < mid) {
		memcpy(res, left, mid - left);
		res += mid - left;
	}
	if (right < end)
		memcpy(res, right, end - right);
	memcpy(start, tmp, end - start);
}

void vial_msort_tmp(void *ptr, void *tmp, size_t count, size_t size, vial_comp_ctx_f comp, void *context)
{
	size_t half;
	byte_ptr mid, end;
	if (count < 2)
		return;
	half = (count + 1) / 2;
	end = mid = ptr;
	mid += size * half;
	end += size * count;
	vial_msort_tmp(ptr, tmp, half, size, comp, context);
	vial_msort_tmp(mid, tmp, count - half, size, comp, context);
	vial_msort_merge(ptr, mid, end, tmp, size, comp, context);
}

void vial_msort(void *ptr, size_t count, size_t size, vial_comp_ctx_f comp, void *context)
{
	void *tmp = malloc(count * size);
	vial_msort_tmp(ptr, tmp, count, size, comp, context);
	free(tmp);
}
