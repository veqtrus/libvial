/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_DEF_H
#define VIAL_DEF_H

typedef void (*vial_dispose_f)(void *);

typedef int (*vial_comp_f)(const void *, const void *);
typedef int (*vial_comp_ctx_f)(const void *, const void *, void *);

#endif
