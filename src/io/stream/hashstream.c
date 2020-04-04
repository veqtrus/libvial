/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/stream/hashstream.h>

#include <stddef.h>
#include <ccan/crypto/sha256/sha256.h>

static void impl_dispose(struct vial_stream *self) { }

static vial_error_t impl_capabilities(struct vial_stream *self, int *capabilities)
{
	*capabilities = VIAL_STREAM_CAN_WRITE;
	return NULL;
}

static vial_error_t impl_not_supported()
{
	return vial_error_new(VIAL_STREAM_NOT_SUPPORTED, VIAL_STREAM_NOT_SUPPORTED, NULL);
}

static vial_error_t impl_write(struct vial_stream *self, const void *buf, size_t size)
{
	if (size == 0) return NULL;
	struct sha256_ctx *hash = &((struct vial_hashstream *) self)->hash;
	sha256_update(hash, buf, size);
	return NULL;
}

static const struct vial_stream_vtable vtable = {
	impl_dispose,
	impl_capabilities,
	impl_not_supported,
	impl_not_supported,
	impl_not_supported,
	impl_not_supported,
	impl_not_supported,
	impl_not_supported,
	impl_write
};

vial_error_t vial_hashstream_init(struct vial_hashstream *self)
{
	self->stream.vtable = &vtable;
	sha256_init(&self->hash);
	return NULL;
}

vial_error_t vial_hashstream_sha256(struct vial_hashstream *self, void *buf)
{
	struct sha256_ctx *hash = &self->hash;
	struct sha256 *res = (struct sha256*)buf;
	sha256_done(hash, res);
	sha256_init(hash);
	return NULL;
}

vial_error_t vial_hashstream_sha256d(struct vial_hashstream *self, void *buf)
{
	struct sha256_ctx *hash = &self->hash;
	struct sha256 *res = (struct sha256*)buf;
	sha256_done(hash, res);
	sha256_init(hash);
	sha256_update(hash, res, 32);
	sha256_done(hash, res);
	sha256_init(hash);
	return NULL;
}
