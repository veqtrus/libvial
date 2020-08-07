/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_FUNC_H
#define VIAL_FUNC_H

typedef void (*vial_dispose_f)(void *);

typedef int (*vial_comp_f)(const void *l, const void *r);
typedef int (*vial_comp_ctx_f)(const void *l, const void *r, void *ctx);

typedef void (*vial_consumer_f)(const void *v, void *ctx);
typedef void (*vial_biconsumer_f)(const void *a, const void *b, void *ctx);

void vial_dispose_nop(void *p);

int vial_comp_cstr(const void *l, const void *r);

#endif
