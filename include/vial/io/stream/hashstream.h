/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_IO_HASHSTREAM_H
#define VIAL_IO_HASHSTREAM_H

#include <ccan/crypto/sha256/sha256.h>

#include <vial/io/stream/stream.h>

/* SHA-256 */
struct vial_hashstream {
	struct vial_stream stream;
	struct sha256_ctx hash;
};

vial_error_t vial_hashstream_init(struct vial_hashstream *self);

vial_error_t vial_hashstream_sha256(struct vial_hashstream *self, void *buf);

vial_error_t vial_hashstream_sha256d(struct vial_hashstream *self, void *buf);

#endif
