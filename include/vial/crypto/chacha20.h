/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_CRYPTO_CHACHA20_H
#define VIAL_CRYPTO_CHACHA20_H

#include <stddef.h>
#include <stdint.h>

struct vial_chacha20 {
	uint32_t state[16];
};

void vial_chacha20_init(struct vial_chacha20 *self, const void *key, const void *nonce);

void vial_chacha20_next(struct vial_chacha20 *self, void *out);

#endif
