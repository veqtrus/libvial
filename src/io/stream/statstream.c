/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/stream/statstream.h>

#define _self ((struct vial_statstream *) self)

static vial_error impl_capabilities(void *self, int *capabilities)
{
	*capabilities = VIAL_STREAM_CAN_ALL ^ VIAL_STREAM_CAN_READ;
	return NULL;
}

static vial_error impl_nop() { return NULL; }

static vial_error impl_seek(void *self, long offset, enum vial_stream_seek origin)
{
	switch (origin) {
	case VIAL_STREAM_SEEK_SET:
		_self->position = offset;
		break;
	case VIAL_STREAM_SEEK_CUR:
		if (offset < 0) _self->position -= -offset;
		else _self->position += offset;
		break;
	case VIAL_STREAM_SEEK_END:
		_self->position = _self->size;
		if (offset < 0) _self->position -= -offset;
		else _self->position += offset;
		break;
	}
	return NULL;
}

static vial_error impl_position(void *self, size_t *position)
{
	*position = _self->position;
	return NULL;
}

static vial_error impl_available(void *self, size_t *available)
{
	*available = _self->size - _self->position;
	return NULL;
}

static vial_error impl_write(void *self, const void *buf, size_t size)
{
	_self->position += size;
	if (_self->position > _self->size)
		_self->size = _self->position;
	return NULL;
}

VIAL_BEGIN_CLASS_DEF(vial_statstream, vial_object) VIAL_END_CLASS_DEF;

VIAL_BEGIN_IMPLEMENTATION(vial_statstream, vial_stream)
	impl_capabilities,
	impl_nop,
	impl_nop,
	impl_seek,
	impl_position,
	impl_available,
	vial_throws_notsup_error,
	impl_write,
VIAL_END_IMPLEMENTATION(vial_statstream, vial_stream);

vial_error vial_statstream_init(struct vial_statstream *self)
{
	vial_object_init(&self->as_vial_object);
	self->as_vial_object.typeinfo = &vial_statstream_typeinfo;
	self->stream.vtable = &vial_statstream_as_vial_stream;
	self->size = self->position = 0;
	return NULL;
}
