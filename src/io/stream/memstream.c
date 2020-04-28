/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/stream/memstream.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define _self ((struct vial_memstream *) self)

static vial_error resize(struct vial_memstream *self, size_t size)
{
	if (self->allocated >= size)
		return NULL;
	if (!self->resizeable)
		return vial_error_new(VIAL_STREAM_NOT_SUPPORTED, VIAL_STREAM_NOT_SUPPORTED, NULL);
	size_t size2 = self->allocated * 2;
	if (size2 > size) size = size2;
	self->buf = realloc(self->buf, size);
	self->allocated = size;
	return NULL;
}

static void impl_dispose(struct vial_stream *self)
{
	if (_self->resizeable)
		free(_self->buf);
	_self->buf = NULL;
}

static vial_error impl_capabilities(struct vial_stream *self, int *capabilities)
{
	if (_self->buf == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
	*capabilities = VIAL_STREAM_CAN_ALL;
	return NULL;
}

static vial_error impl_close(struct vial_stream *self)
{
	if (_self->buf == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
	free(_self->buf);
	_self->buf = NULL;
	_self->allocated = _self->size = _self->position = 0;
	return NULL;
}

static vial_error impl_flush(struct vial_stream *self) { return NULL; }

static vial_error impl_seek(struct vial_stream *self, long offset, enum vial_stream_seek origin)
{
	if (_self->buf == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
	bool back = offset < 0;
	if (back) offset = -offset;
	switch (origin) {
	case VIAL_STREAM_SEEK_SET:
		if (back)
			return vial_error_new(VIAL_STREAM_NOT_SUPPORTED, VIAL_STREAM_NOT_SUPPORTED, NULL);
		vial_error_rethrow(resize(_self, offset));
		_self->position = offset;
		if (_self->position > _self->size)
			_self->size = _self->position;
		break;
	case VIAL_STREAM_SEEK_CUR:
		if (back) {
			if (offset > _self->position)
				return vial_error_new(VIAL_STREAM_NOT_SUPPORTED, VIAL_STREAM_NOT_SUPPORTED, NULL);
			_self->position -= offset;
		} else {
			vial_error_rethrow(resize(_self, _self->position + offset));
			_self->position += offset;
			if (_self->position > _self->size)
				_self->size = _self->position;
		}
		break;
	case VIAL_STREAM_SEEK_END:
		if (!back && offset != 0)
			return vial_error_new(VIAL_STREAM_NOT_SUPPORTED, VIAL_STREAM_NOT_SUPPORTED, NULL);
		if (offset > _self->size)
			return vial_error_new(VIAL_STREAM_NOT_SUPPORTED, VIAL_STREAM_NOT_SUPPORTED, NULL);
		_self->position = _self->size - offset;
		break;
	}
	return NULL;
}

static vial_error impl_position(struct vial_stream *self, size_t *position)
{
	if (_self->buf == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
	*position = _self->position;
	return NULL;
}

static vial_error impl_available(struct vial_stream *self, size_t *available)
{
	if (_self->buf == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
	*available = _self->size - _self->position;
	return NULL;
}

static vial_error impl_read(struct vial_stream *self, void *buf, size_t size)
{
	if (_self->buf == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
	if (_self->size - _self->position < size)
		return vial_error_new(VIAL_STREAM_NOT_SUPPORTED, VIAL_STREAM_NOT_SUPPORTED, NULL);
	memcpy(buf, _self->buf + _self->position, size);
	_self->position += size;
	return NULL;
}

static vial_error impl_write(struct vial_stream *self, const void *buf, size_t size)
{
	if (_self->buf == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
	vial_error_rethrow(resize(_self, _self->position + size));
	memcpy(_self->buf + _self->position, buf, size);
	_self->position += size;
	if (_self->position > _self->size)
		_self->size = _self->position;
	return NULL;
}

static const struct vial_stream_vtable vtable = {
	impl_dispose,
	impl_capabilities,
	impl_close,
	impl_flush,
	impl_seek,
	impl_position,
	impl_available,
	impl_read,
	impl_write
};

vial_error vial_memstream_init_buf(struct vial_memstream *self, void *buf, size_t size)
{
	self->stream.vtable = &vtable;
	self->buf = buf;
	self->allocated = self->size = size;
	self->position = 0;
	self->resizeable = false;
	return NULL;
}

vial_error vial_memstream_init(struct vial_memstream *self, size_t size)
{
	self->stream.vtable = &vtable;
	self->buf = size == 0 ? NULL : malloc(size);
	self->allocated = self->size = size;
	self->position = 0;
	self->resizeable = true;
	return NULL;
}
