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
		vial_error_throw(vial_notsup_error_new(NULL, NULL));
	size_t size2 = self->allocated * 2;
	if (size2 > size) size = size2;
	self->buf = realloc(self->buf, size);
	self->allocated = size;
	return NULL;
}

static void impl_dispose(void *self)
{
	if (_self->resizeable)
		free(_self->buf);
	_self->buf = NULL;
}

static vial_error impl_capabilities(void *self, int *capabilities)
{
	if (_self->buf == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	*capabilities = VIAL_STREAM_CAN_ALL;
	return NULL;
}

static vial_error impl_close(void *self)
{
	if (_self->buf == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	free(_self->buf);
	_self->buf = NULL;
	_self->allocated = _self->size = _self->position = 0;
	return NULL;
}

static vial_error impl_flush(void *self) { return NULL; }

static vial_error impl_seek(void *self, long offset, enum vial_stream_seek origin)
{
	if (_self->buf == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	bool back = offset < 0;
	if (back) offset = -offset;
	switch (origin) {
	case VIAL_STREAM_SEEK_SET:
		if (back)
			vial_error_throw(vial_notsup_error_new(NULL, NULL));
		vial_error_rethrow(resize(_self, offset));
		_self->position = offset;
		if (_self->position > _self->size)
			_self->size = _self->position;
		break;
	case VIAL_STREAM_SEEK_CUR:
		if (back) {
			if (offset > _self->position)
				vial_error_throw(vial_notsup_error_new(NULL, NULL));
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
			vial_error_throw(vial_notsup_error_new(NULL, NULL));
		if (offset > _self->size)
			vial_error_throw(vial_notsup_error_new(NULL, NULL));
		_self->position = _self->size - offset;
		break;
	}
	return NULL;
}

static vial_error impl_position(void*self, size_t *position)
{
	if (_self->buf == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	*position = _self->position;
	return NULL;
}

static vial_error impl_available(void *self, size_t *available)
{
	if (_self->buf == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	*available = _self->size - _self->position;
	return NULL;
}

static vial_error impl_read(void *self, void *buf, size_t size)
{
	if (_self->buf == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	if (_self->size - _self->position < size)
		vial_error_throw(vial_notsup_error_new(NULL, NULL));
	memcpy(buf, _self->buf + _self->position, size);
	_self->position += size;
	return NULL;
}

static vial_error impl_write(void *self, const void *buf, size_t size)
{
	if (_self->buf == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	vial_error_rethrow(resize(_self, _self->position + size));
	memcpy(_self->buf + _self->position, buf, size);
	_self->position += size;
	if (_self->position > _self->size)
		_self->size = _self->position;
	return NULL;
}

VIAL_BEGIN_CLASS_DEF(vial_memstream, vial_object) VIAL_END_CLASS_DEF;

VIAL_BEGIN_IMPLEMENTATION(vial_memstream, vial_object)
	impl_dispose,
VIAL_END_IMPLEMENTATION(vial_memstream, vial_object);

VIAL_BEGIN_IMPLEMENTATION(vial_memstream, vial_stream)
	impl_capabilities,
	impl_close,
	impl_flush,
	impl_seek,
	impl_position,
	impl_available,
	impl_read,
	impl_write,
VIAL_END_IMPLEMENTATION(vial_memstream, vial_stream);

vial_error vial_memstream_init_buf(struct vial_memstream *self, void *buf, size_t size)
{
	self->as_vial_object.typeinfo = &vial_memstream_typeinfo;
	self->as_vial_object.vtable = &vial_memstream_as_vial_object;
	self->as_vial_stream.vtable = &vial_memstream_as_vial_stream;
	self->buf = buf;
	self->allocated = self->size = size;
	self->position = 0;
	self->resizeable = false;
	return NULL;
}

vial_error vial_memstream_init(struct vial_memstream *self, size_t size)
{
	self->as_vial_object.typeinfo = &vial_memstream_typeinfo;
	self->as_vial_object.vtable = &vial_memstream_as_vial_object;
	self->as_vial_stream.vtable = &vial_memstream_as_vial_stream;
	self->buf = size == 0 ? NULL : malloc(size);
	self->allocated = self->size = size;
	self->position = 0;
	self->resizeable = true;
	return NULL;
}
