/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/stream/filestream.h>

#include <stdbool.h>
#include <limits.h>
#include <stdio.h>

#define _self ((struct vial_filestream *) self)

static void impl_dispose(void *self)
{
	_self->file = NULL;
}

static vial_error impl_capabilities(void *self, int *capabilities)
{
	if (_self->file == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	*capabilities = VIAL_STREAM_CAN_ALL;
	return NULL;
}

static vial_error impl_close(void *self)
{
	FILE *file = _self->file;
	if (file == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	if (fclose(file))
		vial_error_throw(vial_stream_io_error_new(NULL, NULL));
	_self->file = NULL;
	return NULL;
}

static vial_error impl_flush(void *self)
{
	FILE *file = _self->file;
	if (file == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	if (fflush(file))
		vial_error_throw(vial_stream_io_error_new(NULL, NULL));
	return NULL;
}

static vial_error impl_seek(void *self, long offset, enum vial_stream_seek origin)
{
	FILE *file = _self->file;
	int file_origin;
	if (file == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	switch (origin) {
	case VIAL_STREAM_SEEK_SET:
		file_origin = SEEK_SET;
		break;
	case VIAL_STREAM_SEEK_CUR:
		file_origin = SEEK_CUR;
		break;
	case VIAL_STREAM_SEEK_END:
		file_origin = SEEK_END;
		break;
	}
	if (fseek(file, offset, file_origin))
		vial_error_throw(vial_stream_io_error_new(NULL, NULL));
	return NULL;
}

static vial_error impl_position(void *self, size_t *position)
{
	FILE *file = _self->file;
	if (file == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	long res = ftell(file);
	if (res < 0)
		vial_error_throw(vial_stream_io_error_new(NULL, NULL));
	*position = res;
	return NULL;
}

static vial_error impl_available(void *self, size_t *available)
{
	FILE *file = _self->file;
	if (file == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	long cpos = ftell(file);
	if (cpos < 0)
		vial_error_throw(vial_stream_io_error_new(NULL, NULL));
	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	if (len < 0)
		vial_error_throw(vial_stream_io_error_new(NULL, NULL));
	if (fseek(file, cpos, SEEK_SET))
		vial_error_throw(vial_stream_io_error_new(NULL, NULL));
	*available = len - cpos;
	return NULL;
}

static vial_error impl_read(void *self, void *buf, size_t size)
{
	FILE *file = _self->file;
	if (file == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	if (size == 0)
		return NULL;
	if (fread(buf, size, 1, file))
		return NULL;
	vial_error_throw(vial_stream_io_error_new(NULL, NULL));
}

static vial_error impl_write(void *self, const void *buf, size_t size)
{
	FILE *file = _self->file;
	if (file == NULL)
		vial_error_throw(vial_disposed_error_new(NULL, NULL));
	if (size == 0)
		return NULL;
	if (fwrite(buf, size, 1, file))
		return NULL;
	vial_error_throw(vial_stream_io_error_new(NULL, NULL));
}

VIAL_BEGIN_CLASS_DEF(vial_filestream, vial_object) VIAL_END_CLASS_DEF;

VIAL_BEGIN_IMPLEMENTATION(vial_filestream, vial_object)
	impl_dispose,
VIAL_END_IMPLEMENTATION(vial_filestream, vial_object);

VIAL_BEGIN_IMPLEMENTATION(vial_filestream, vial_stream)
	impl_capabilities,
	impl_close,
	impl_flush,
	impl_seek,
	impl_position,
	impl_available,
	impl_read,
	impl_write,
VIAL_END_IMPLEMENTATION(vial_filestream, vial_stream);

vial_error vial_filestream_init(struct vial_filestream *self, FILE *file)
{
	self->as_vial_object.typeinfo = &vial_filestream_typeinfo;
	self->as_vial_object.vtable = &vial_filestream_as_vial_object;
	self->as_vial_stream.vtable = &vial_filestream_as_vial_stream;
	self->file = file;
	return NULL;
}
