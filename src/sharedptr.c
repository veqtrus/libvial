/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/sharedptr.h>

#include <stdlib.h>

#ifndef __STDC_NO_ATOMICS__
#include <stdatomic.h>
#endif

struct sharedptr {
	vial_dispose_f dispose;
	volatile int count;
#ifndef __STDC_NO_ATOMICS__
	atomic_flag flag;
#endif
};

static void dispose_nop(void *ptr) { }

void *vial_sharedptr_malloc(size_t size)
{
	return vial_sharedptr_make(size, dispose_nop);
}

void *vial_sharedptr_make(size_t size, vial_dispose_f dispose)
{
	struct sharedptr *sp = malloc(size + sizeof(*sp));
	sp->dispose = dispose;
	sp->count = 1;
#ifndef __STDC_NO_ATOMICS__
	atomic_flag_clear(&sp->flag);
#endif
	return sp + 1;
}

void vial_sharedptr_take(void *ptr)
{
	struct sharedptr *sp = ptr;
	sp--;
#ifndef __STDC_NO_ATOMICS__
	do {} while (atomic_flag_test_and_set(&sp->flag));
#endif
	sp->count++;
#ifndef __STDC_NO_ATOMICS__
	atomic_flag_clear(&sp->flag);
#endif
}

void vial_sharedptr_leave(void *ptr)
{
	struct sharedptr *sp = ptr;
	sp--;
#ifndef __STDC_NO_ATOMICS__
	do {} while (atomic_flag_test_and_set(&sp->flag));
#endif
	if (sp->count > 1) {
		sp->count--;
#ifndef __STDC_NO_ATOMICS__
		atomic_flag_clear(&sp->flag);
#endif
		return;
	}
#ifndef __STDC_NO_ATOMICS__
	atomic_flag_clear(&sp->flag);
#endif
	sp->dispose(ptr);
	free(sp);
}
