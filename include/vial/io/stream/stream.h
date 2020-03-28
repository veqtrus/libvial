/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_IO_STREAM_H
#define VIAL_IO_STREAM_H

#include <stddef.h>

#include <vial/error.h>

#define VSTREAM_SUPPORTS_READ 1
#define VSTREAM_SUPPORTS_WRITE (1 << 1)
#define VSTREAM_SUPPORTS_SEEK (1 << 2)
#define VSTREAM_SUPPORTS_POSITION (1 << 3)
#define VSTREAM_SUPPORTS_AVAILABLE (1 << 4)
#define VSTREAM_SUPPORTS_ALL ((1 << 5) - 1)

extern const char *const VSTREAM_IO_ERROR;
extern const char *const VSTREAM_DISPOSED;
extern const char *const VSTREAM_NOT_SUPPORTED;

enum vStreamSeek {
	vStreamSeek_SET,
	vStreamSeek_CUR,
	vStreamSeek_END
};

struct vStream;

struct vStream_vtable {
	void (*dispose)(struct vStream *self);
	error_t (*capabilities)(struct vStream *self, int *capabilities);
	error_t (*close)(struct vStream *self);
	error_t (*flush)(struct vStream *self);
	error_t (*seek)(struct vStream *self, long offset, enum vStreamSeek origin);
	error_t (*position)(struct vStream *self, size_t *position);
	error_t (*available)(struct vStream *self, size_t *available);
	error_t (*read)(struct vStream *self, void *buf, size_t size);
	error_t (*write)(struct vStream *self, const void *buf, size_t size);
};

struct vStream {
	const struct vStream_vtable *vtable;
};

static inline void vStream_dispose(struct vStream *self)
{
	self->vtable->dispose(self);
}

static inline error_t vStream_capabilities(struct vStream *self, int *capabilities)
{
	return self->vtable->capabilities(self, capabilities);
}

static inline error_t vStream_close(struct vStream *self)
{
	return self->vtable->close(self);
}

static inline error_t vStream_flush(struct vStream *self)
{
	return self->vtable->flush(self);
}

static inline error_t vStream_seek(struct vStream *self, long offset, enum vStreamSeek origin)
{
	return self->vtable->seek(self, offset, origin);
}

static inline error_t vStream_position(struct vStream *self, size_t *position)
{
	return self->vtable->position(self, position);
}

static inline error_t vStream_available(struct vStream *self, size_t *available)
{
	return self->vtable->available(self, available);
}

static inline error_t vStream_read(struct vStream *self, void *buf, size_t size)
{
	return self->vtable->read(self, buf, size);
}

static inline error_t vStream_write(struct vStream *self, const void *buf, size_t size)
{
	return self->vtable->write(self, buf, size);
}

error_t vStream_print(struct vStream *self, const char *str);

#endif
