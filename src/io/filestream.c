/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/filestream.h>

#include <stdbool.h>
#include <limits.h>
#include <stdio.h>

#define _self ((struct vFileStream *) self)

static void impl_dispose(struct vStream *self)
{
	_self->file = NULL;
}

static error_t impl_capabilities(struct vStream *self, int *capabilities)
{
	if (_self->file == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	*capabilities = VSTREAM_SUPPORTS_ALL;
	return NULL;
}

static error_t impl_close(struct vStream *self)
{
	FILE *file = _self->file;
	if (file == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	if (fclose(file))
		return error_new(VSTREAM_IO_ERROR, VSTREAM_IO_ERROR, NULL);
	_self->file = NULL;
	return NULL;
}

static error_t impl_flush(struct vStream *self)
{
	FILE *file = _self->file;
	if (file == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	if (fflush(file))
		return error_new(VSTREAM_IO_ERROR, VSTREAM_IO_ERROR, NULL);
	return NULL;
}

static error_t impl_seek(struct vStream *self, long offset, enum vStreamSeek origin)
{
	FILE *file = _self->file;
	int file_origin;
	if (file == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	switch (origin) {
	case vStreamSeek_SET:
		file_origin = SEEK_SET;
		break;
	case vStreamSeek_CUR:
		file_origin = SEEK_CUR;
		break;
	case vStreamSeek_END:
		file_origin = SEEK_END;
		break;
	}
	if (fseek(file, offset, file_origin))
		return error_new(VSTREAM_IO_ERROR, VSTREAM_IO_ERROR, NULL);
	return NULL;
}

static error_t impl_position(struct vStream *self, size_t *position)
{
	FILE *file = _self->file;
	if (file == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	long res = ftell(file);
	if (res < 0)
		return error_new(VSTREAM_IO_ERROR, VSTREAM_IO_ERROR, NULL);
	*position = res;
	return NULL;
}

static error_t impl_available(struct vStream *self, size_t *available)
{
	FILE *file = _self->file;
	if (file == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	long cpos = ftell(file);
	if (cpos < 0)
		return error_new(VSTREAM_IO_ERROR, VSTREAM_IO_ERROR, NULL);
	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	if (len < 0)
		return error_new(VSTREAM_IO_ERROR, VSTREAM_IO_ERROR, NULL);
	if (fseek(file, cpos, SEEK_SET))
		return error_new(VSTREAM_IO_ERROR, VSTREAM_IO_ERROR, NULL);
	*available = len - cpos;
	return NULL;
}

static error_t impl_read(struct vStream *self, void *buf, size_t size)
{
	FILE *file = _self->file;
	if (file == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	if (size == 0)
		return NULL;
	if (fread(buf, size, 1, file))
		return NULL;
	return error_new(VSTREAM_IO_ERROR, VSTREAM_IO_ERROR, NULL);
}

static error_t impl_write(struct vStream *self, const void *buf, size_t size)
{
	FILE *file = _self->file;
	if (file == NULL)
		return error_new(VSTREAM_DISPOSED, VSTREAM_DISPOSED, NULL);
	if (size == 0)
		return NULL;
	if (fwrite(buf, size, 1, file))
		return NULL;
	return error_new(VSTREAM_IO_ERROR, VSTREAM_IO_ERROR, NULL);
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

error_t vFileStream_init(struct vFileStream *self, FILE *file)
{
	self->stream.vtable = &vtable;
	self->file = file;
	return NULL;
}
