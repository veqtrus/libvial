/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/hashstream.h>

#include <stddef.h>
#include <ccan/crypto/sha256/sha256.h>

static void impl_dispose(struct vStream *self) { }

static error_t impl_capabilities(struct vStream *self, int *capabilities)
{
	*capabilities = VSTREAM_SUPPORTS_WRITE;
	return NULL;
}

static error_t impl_not_supported()
{
	return error_new(VSTREAM_NOT_SUPPORTED, VSTREAM_NOT_SUPPORTED, NULL);
}

static error_t impl_write(struct vStream *self, const void *buf, size_t size)
{
	if (size == 0) return NULL;
	struct sha256_ctx *hash = &((struct vHashStream *) self)->hash;
	sha256_update(hash, buf, size);
	return NULL;
}

static const struct vStream_vtable vtable = {
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

error_t vHashStream_init(struct vHashStream *self)
{
	self->stream.vtable = &vtable;
	sha256_init(&self->hash);
	return NULL;
}

error_t vHashStream_sha256(struct vHashStream *self, void *buf)
{
	struct sha256_ctx *hash = &self->hash;
	struct sha256 *res = (struct sha256*)buf;
	sha256_done(hash, res);
	sha256_init(hash);
	return NULL;
}

error_t vHashStream_sha256d(struct vHashStream *self, void *buf)
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
