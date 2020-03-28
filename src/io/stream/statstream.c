/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/stream/statstream.h>

#define _self ((struct vStatStream *) self)

static void impl_dispose(struct vStream *self) { }

static error_t impl_capabilities(struct vStream *self, int *capabilities)
{
	*capabilities = VSTREAM_SUPPORTS_ALL ^ VSTREAM_SUPPORTS_READ;
	return NULL;
}

static error_t impl_nop() { return NULL; }

static error_t impl_seek(struct vStream *self, long offset, enum vStreamSeek origin)
{
	switch (origin) {
	case vStreamSeek_SET:
		_self->position = offset;
		break;
	case vStreamSeek_CUR:
		if (offset < 0) _self->position -= -offset;
		else _self->position += offset;
		break;
	case vStreamSeek_END:
		_self->position = _self->size;
		if (offset < 0) _self->position -= -offset;
		else _self->position += offset;
		break;
	}
	return NULL;
}

static error_t impl_position(struct vStream *self, size_t *position)
{
	*position = _self->position;
	return NULL;
}

static error_t impl_available(struct vStream *self, size_t *available)
{
	*available = _self->size - _self->position;
	return NULL;
}

static error_t impl_read(struct vStream *self, void *buf, size_t size)
{
	return error_new(VSTREAM_NOT_SUPPORTED, VSTREAM_NOT_SUPPORTED, NULL);
}

static error_t impl_write(struct vStream *self, const void *buf, size_t size)
{
	_self->position += size;
	if (_self->position > _self->size)
		_self->size = _self->position;
	return NULL;
}

static const struct vStream_vtable vtable = {
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

error_t vStatStream_init(struct vStatStream *self)
{
	self->stream.vtable = &vtable;
	self->size = self->position = 0;
	return NULL;
}
