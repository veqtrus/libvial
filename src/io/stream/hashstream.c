/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/stream/hashstream.h>

#include <stddef.h>

static vial_error impl_capabilities(void *self, int *capabilities)
{
	*capabilities = VIAL_STREAM_CAN_WRITE;
	return NULL;
}

static vial_error impl_write(void *self, const void *buf, size_t size)
{
	if (size == 0) return NULL;
	struct vial_sha256 *hasher = &((struct vial_hashstream *) self)->hasher;
	vial_sha256_update(hasher, buf, size);
	return NULL;
}

VIAL_BEGIN_CLASS_DEF(vial_hashstream, vial_object) VIAL_END_CLASS_DEF;

VIAL_BEGIN_IMPLEMENTATION(vial_hashstream, vial_stream)
	impl_capabilities,
	vial_throws_notsup_error,
	vial_throws_notsup_error,
	vial_throws_notsup_error,
	vial_throws_notsup_error,
	vial_throws_notsup_error,
	vial_throws_notsup_error,
	impl_write,
VIAL_END_IMPLEMENTATION(vial_hashstream, vial_stream);

vial_error vial_hashstream_init(struct vial_hashstream *self)
{
	vial_object_init(&self->as_vial_object);
	self->as_vial_object.typeinfo = &vial_hashstream_typeinfo;
	self->as_vial_stream.vtable = &vial_hashstream_as_vial_stream;
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
