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

#define VIAL_STREAM_CAN_READ 1
#define VIAL_STREAM_CAN_WRITE (1 << 1)
#define VIAL_STREAM_CAN_SEEK (1 << 2)
#define VIAL_STREAM_CAN_POSITION (1 << 3)
#define VIAL_STREAM_CAN_AVAILABLE (1 << 4)
#define VIAL_STREAM_CAN_ALL ((1 << 5) - 1)

extern const char *const VIAL_STREAM_IO_ERROR;
extern const char *const VIAL_STREAM_DISPOSED;
extern const char *const VIAL_STREAM_NOT_SUPPORTED;

enum vial_stream_seek {
	VIAL_STREAM_SEEK_SET,
	VIAL_STREAM_SEEK_CUR,
	VIAL_STREAM_SEEK_END
};

struct vial_stream;

struct vial_stream_vtable {
	void (*dispose)(struct vial_stream *self);
	vial_error_t (*capabilities)(struct vial_stream *self, int *capabilities);
	vial_error_t (*close)(struct vial_stream *self);
	vial_error_t (*flush)(struct vial_stream *self);
	vial_error_t (*seek)(struct vial_stream *self, long offset, enum vial_stream_seek origin);
	vial_error_t (*position)(struct vial_stream *self, size_t *position);
	vial_error_t (*available)(struct vial_stream *self, size_t *available);
	vial_error_t (*read)(struct vial_stream *self, void *buf, size_t size);
	vial_error_t (*write)(struct vial_stream *self, const void *buf, size_t size);
};

struct vial_stream {
	const struct vial_stream_vtable *vtable;
};

static inline void vial_stream_dispose(struct vial_stream *self)
{
	self->vtable->dispose(self);
}

static inline vial_error_t vial_stream_capabilities(struct vial_stream *self, int *capabilities)
{
	return self->vtable->capabilities(self, capabilities);
}

static inline vial_error_t vial_stream_close(struct vial_stream *self)
{
	return self->vtable->close(self);
}

static inline vial_error_t vial_stream_flush(struct vial_stream *self)
{
	return self->vtable->flush(self);
}

static inline vial_error_t vial_stream_seek(struct vial_stream *self, long offset, enum vial_stream_seek origin)
{
	return self->vtable->seek(self, offset, origin);
}

static inline vial_error_t vial_stream_position(struct vial_stream *self, size_t *position)
{
	return self->vtable->position(self, position);
}

static inline vial_error_t vial_stream_available(struct vial_stream *self, size_t *available)
{
	return self->vtable->available(self, available);
}

static inline vial_error_t vial_stream_read(struct vial_stream *self, void *buf, size_t size)
{
	return self->vtable->read(self, buf, size);
}

static inline vial_error_t vial_stream_write(struct vial_stream *self, const void *buf, size_t size)
{
	return self->vtable->write(self, buf, size);
}

vial_error_t vial_stream_print(struct vial_stream *self, const char *str);

#endif
