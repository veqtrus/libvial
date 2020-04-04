/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_MSORT_H
#define VIAL_MSORT_H

#include <stddef.h>

#include <vial/sort/sort.h>

void vial_msort_tmp(void *ptr, void *tmp, size_t count, size_t size, vial_comp_f comp, void *context);

void vial_msort(void *ptr, size_t count, size_t size, vial_comp_f comp, void *context);

#endif
