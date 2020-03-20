/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/sharedptr.h>

#include <stdlib.h>

struct sharedptr {
	sharedptr_dispose_f dispose;
	volatile int count;
};

static void dispose_nop(void *ptr) { }

void *sharedptr_malloc(size_t size)
{
	return sharedptr_make(size, dispose_nop);
}

void *sharedptr_make(size_t size, sharedptr_dispose_f dispose)
{
	struct sharedptr *sp = malloc(size + sizeof(*sp));
	sp->dispose = dispose;
	sp->count = 1;
	return sp + 1;
}

void sharedptr_take(void *ptr)
{
	struct sharedptr *sp = ptr;
	(sp - 1)->count++;
}

void sharedptr_leave(void *ptr)
{
	struct sharedptr *sp = ptr;
	sp--;
	if (sp->count > 1) {
		sp->count--;
		return;
	}
	sp->dispose(ptr);
	free(sp);
}
