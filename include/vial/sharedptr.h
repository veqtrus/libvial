/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_SHAREDPTR_H
#define VIAL_SHAREDPTR_H

#include <stddef.h>

typedef void (*sharedptr_dispose_f)(void *);

void *sharedptr_malloc(size_t size);

void *sharedptr_make(size_t size, sharedptr_dispose_f dispose);

void sharedptr_take(void *ptr);

void sharedptr_leave(void *ptr);

#endif
