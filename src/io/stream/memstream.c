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

#define _self ((struct vMemStream *) self)

static error_t resize(struct vMemStream *self, size_t size)
{
	if (self->allocated >= size)
		return NULL;
	if (!self->resizeable)
		return error_new(VSTREAM_NOT_SUPPORTED, VSTREAM_NOT_SUPPORTED, NULL);
	size_t size2 = self->allocated * 2;
	if (size2 > size) size = size2;
	self->buf = realloc(self->buf, size);
	self->allocated = size;
	return NULL;
}

static void impl_dispose(struct vStream *self)
{
	if (_self->resizeable)
		free(_self->buf);
	_self->buf = NULL;
}

static error_t impl_capabilities(struct vStream *self, int *capabilities)
{
	if (_self->buf == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	*capabilities = VSTREAM_SUPPORTS_ALL;
	return NULL;
}

static error_t impl_close(struct vStream *self)
{
	if (_self->buf == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	free(_self->buf);
	_self->buf = NULL;
	_self->allocated = _self->size = _self->position = 0;
	return NULL;
}

static error_t impl_flush(struct vStream *self) { return NULL; }

static error_t impl_seek(struct vStream *self, long offset, enum vStreamSeek origin)
{
	if (_self->buf == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	bool back = offset < 0;
	if (back) offset = -offset;
	switch (origin) {
	case vStreamSeek_SET:
		if (back)
			return error_new(VSTREAM_NOT_SUPPORTED, VSTREAM_NOT_SUPPORTED, NULL);
		error_rethrow(resize(_self, offset));
		_self->position = offset;
		if (_self->position > _self->size)
			_self->size = _self->position;
		break;
	case vStreamSeek_CUR:
		if (back) {
			if (offset > _self->position)
				return error_new(VSTREAM_NOT_SUPPORTED, VSTREAM_NOT_SUPPORTED, NULL);
			_self->position -= offset;
		} else {
			error_rethrow(resize(_self, _self->position + offset));
			_self->position += offset;
			if (_self->position > _self->size)
				_self->size = _self->position;
		}
		break;
	case vStreamSeek_END:
		if (!back && offset != 0)
			return error_new(VSTREAM_NOT_SUPPORTED, VSTREAM_NOT_SUPPORTED, NULL);
		if (offset > _self->size)
			return error_new(VSTREAM_NOT_SUPPORTED, VSTREAM_NOT_SUPPORTED, NULL);
		_self->position = _self->size - offset;
		break;
	}
	return NULL;
}

static error_t impl_position(struct vStream *self, size_t *position)
{
	if (_self->buf == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	*position = _self->position;
	return NULL;
}

static error_t impl_available(struct vStream *self, size_t *available)
{
	if (_self->buf == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	*available = _self->size - _self->position;
	return NULL;
}

static error_t impl_read(struct vStream *self, void *buf, size_t size)
{
	if (_self->buf == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	if (_self->size - _self->position < size)
		return error_new(VSTREAM_NOT_SUPPORTED, VSTREAM_NOT_SUPPORTED, NULL);
	memcpy(buf, _self->buf + _self->position, size);
	_self->position += size;
	return NULL;
}

static error_t impl_write(struct vStream *self, const void *buf, size_t size)
{
	if (_self->buf == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	error_rethrow(resize(_self, _self->position + size));
	memcpy(_self->buf + _self->position, buf, size);
	_self->position += size;
	if (_self->position > _self->size)
		_self->size = _self->position;
	return NULL;
}

static const struct vStream_vtable vtable = {
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

error_t vMemStream_init_buf(struct vMemStream *self, void *buf, size_t size)
{
	self->stream.vtable = &vtable;
	self->buf = buf;
	self->allocated = self->size = size;
	self->position = 0;
	self->resizeable = false;
	return NULL;
}

error_t vMemStream_init(struct vMemStream *self, size_t size)
{
	self->stream.vtable = &vtable;
	self->buf = size == 0 ? NULL : malloc(size);
	self->allocated = self->size = size;
	self->position = 0;
	self->resizeable = true;
	return NULL;
}
