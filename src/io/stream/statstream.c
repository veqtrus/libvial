/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/stream/statstream.h>

#define _self ((struct vial_statstream *) self)

static void impl_dispose(struct vial_stream *self) { }

static vial_error impl_capabilities(struct vial_stream *self, int *capabilities)
{
	*capabilities = VIAL_STREAM_CAN_ALL ^ VIAL_STREAM_CAN_READ;
	return NULL;
}

static vial_error impl_nop() { return NULL; }

static vial_error impl_seek(struct vial_stream *self, long offset, enum vial_stream_seek origin)
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

static vial_error impl_position(struct vial_stream *self, size_t *position)
{
	*position = _self->position;
	return NULL;
}

static vial_error impl_available(struct vial_stream *self, size_t *available)
{
	*available = _self->size - _self->position;
	return NULL;
}

static vial_error impl_read(struct vial_stream *self, void *buf, size_t size)
{
	return vial_error_new(VIAL_STREAM_NOT_SUPPORTED, VIAL_STREAM_NOT_SUPPORTED, NULL);
}

static vial_error impl_write(struct vial_stream *self, const void *buf, size_t size)
{
	_self->position += size;
	if (_self->position > _self->size)
		_self->size = _self->position;
	return NULL;
}

static const struct vial_stream_vtable vtable = {
	impl_dispose,
	impl_capabilities,
	impl_nop,
	impl_nop,
	impl_seek,
	impl_position,
	impl_available,
	impl_read,
	impl_write
};

vial_error vial_statstream_init(struct vial_statstream *self)
{
	self->stream.vtable = &vtable;
	self->size = self->position = 0;
	return NULL;
}
