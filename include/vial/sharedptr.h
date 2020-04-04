/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_SHAREDPTR_H
#define VIAL_SHAREDPTR_H

#include <stddef.h>

typedef void (*vial_sharedptr_dispose_f)(void *);

void *vial_sharedptr_malloc(size_t size);

void *vial_sharedptr_make(size_t size, vial_sharedptr_dispose_f dispose);

void vial_sharedptr_take(void *ptr);

void vial_sharedptr_leave(void *ptr);

#endif
