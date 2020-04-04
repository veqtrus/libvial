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

static void impl_dispose(struct vial_stream *self)
{
	_self->file = NULL;
}

static vial_error_t impl_capabilities(struct vial_stream *self, int *capabilities)
{
	if (_self->file == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
	*capabilities = VIAL_STREAM_CAN_ALL;
	return NULL;
}

static vial_error_t impl_close(struct vial_stream *self)
{
	FILE *file = _self->file;
	if (file == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
	if (fclose(file))
		return vial_error_new(VIAL_STREAM_IO_ERROR, VIAL_STREAM_IO_ERROR, NULL);
	_self->file = NULL;
	return NULL;
}

static vial_error_t impl_flush(struct vial_stream *self)
{
	FILE *file = _self->file;
	if (file == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
	if (fflush(file))
		return vial_error_new(VIAL_STREAM_IO_ERROR, VIAL_STREAM_IO_ERROR, NULL);
	return NULL;
}

static vial_error_t impl_seek(struct vial_stream *self, long offset, enum vial_stream_seek origin)
{
	FILE *file = _self->file;
	int file_origin;
	if (file == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
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
		return vial_error_new(VIAL_STREAM_IO_ERROR, VIAL_STREAM_IO_ERROR, NULL);
	return NULL;
}

static vial_error_t impl_position(struct vial_stream *self, size_t *position)
{
	FILE *file = _self->file;
	if (file == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
	long res = ftell(file);
	if (res < 0)
		return vial_error_new(VIAL_STREAM_IO_ERROR, VIAL_STREAM_IO_ERROR, NULL);
	*position = res;
	return NULL;
}

static vial_error_t impl_available(struct vial_stream *self, size_t *available)
{
	FILE *file = _self->file;
	if (file == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
	long cpos = ftell(file);
	if (cpos < 0)
		return vial_error_new(VIAL_STREAM_IO_ERROR, VIAL_STREAM_IO_ERROR, NULL);
	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	if (len < 0)
		return vial_error_new(VIAL_STREAM_IO_ERROR, VIAL_STREAM_IO_ERROR, NULL);
	if (fseek(file, cpos, SEEK_SET))
		return vial_error_new(VIAL_STREAM_IO_ERROR, VIAL_STREAM_IO_ERROR, NULL);
	*available = len - cpos;
	return NULL;
}

static vial_error_t impl_read(struct vial_stream *self, void *buf, size_t size)
{
	FILE *file = _self->file;
	if (file == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
	if (size == 0)
		return NULL;
	if (fread(buf, size, 1, file))
		return NULL;
	return vial_error_new(VIAL_STREAM_IO_ERROR, VIAL_STREAM_IO_ERROR, NULL);
}

static vial_error_t impl_write(struct vial_stream *self, const void *buf, size_t size)
{
	FILE *file = _self->file;
	if (file == NULL)
		return vial_error_new(VIAL_STREAM_DISPOSED, VIAL_STREAM_DISPOSED, NULL);
	if (size == 0)
		return NULL;
	if (fwrite(buf, size, 1, file))
		return NULL;
	return vial_error_new(VIAL_STREAM_IO_ERROR, VIAL_STREAM_IO_ERROR, NULL);
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

vial_error_t vial_filestream_init(struct vial_filestream *self, FILE *file)
{
	self->stream.vtable = &vtable;
	self->file = file;
	return NULL;
}
