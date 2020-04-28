/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/stream/hashstream.h>

#include <stddef.h>

static void impl_dispose(struct vial_stream *self) { }

static vial_error impl_capabilities(struct vial_stream *self, int *capabilities)
{
	*capabilities = VIAL_STREAM_CAN_WRITE;
	return NULL;
}

static vial_error impl_not_supported()
{
	return vial_error_new(VIAL_STREAM_NOT_SUPPORTED, VIAL_STREAM_NOT_SUPPORTED, NULL);
}

static vial_error impl_write(struct vial_stream *self, const void *buf, size_t size)
{
	if (size == 0) return NULL;
	struct vial_sha256 *hasher = &((struct vial_hashstream *) self)->hasher;
	vial_sha256_update(hasher, buf, size);
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

vial_error vial_hashstream_init(struct vial_hashstream *self)
{
	self->stream.vtable = &vtable;
	vial_sha256_init(&self->hasher);
	return NULL;
}

vial_error vial_hashstream_sha256(struct vial_hashstream *self, void *buf)
{
	struct vial_sha256 *hasher = &self->hasher;
	vial_sha256_done(hasher, buf);
	vial_sha256_init(hasher);
	return NULL;
}

vial_error vial_hashstream_sha256d(struct vial_hashstream *self, void *buf)
{
	struct vial_sha256 *hasher = &self->hasher;
	vial_sha256_done(hasher, buf);
	vial_sha256_init(hasher);
	vial_sha256_update(hasher, buf, 32);
	vial_sha256_done(hasher, buf);
	vial_sha256_init(hasher);
	return NULL;
}
