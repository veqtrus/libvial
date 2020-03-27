/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_IO_HASHSTREAM_H
#define VIAL_IO_HASHSTREAM_H

#include <ccan/crypto/sha256/sha256.h>

#include <vial/io/stream/stream.h>

/* SHA-256 */
struct vHashStream {
	struct vStream stream;
	struct sha256_ctx hash;
};

error_t vHashStream_init(struct vHashStream *self);

error_t vHashStream_sha256(struct vHashStream *self, void *buf);

error_t vHashStream_sha256d(struct vHashStream *self, void *buf);

#endif
