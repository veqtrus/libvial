/*
Copyright (c) 2019-20 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_IO_STREAM_H
#define VIAL_IO_STREAM_H

#include <stddef.h>

#include <vial/types.h>
#include <vial/error.h>

#define VIAL_STREAM_CAN_READ 1
#define VIAL_STREAM_CAN_WRITE (1 << 1)
#define VIAL_STREAM_CAN_SEEK (1 << 2)
#define VIAL_STREAM_CAN_POSITION (1 << 3)
#define VIAL_STREAM_CAN_AVAILABLE (1 << 4)
#define VIAL_STREAM_CAN_ALL ((1 << 5) - 1)

enum vial_stream_seek {
	VIAL_STREAM_SEEK_SET,
	VIAL_STREAM_SEEK_CUR,
	VIAL_STREAM_SEEK_END
};

VIAL_INTERFACE_DECL(vial_stream);
VIAL_BEGIN_VIRTUALS(vial_stream)
	vial_error (*capabilities)(void *self, int *capabilities);
	vial_error (*close)(void *self);
	vial_error (*flush)(void *self);
	vial_error (*seek)(void *self, long offset, enum vial_stream_seek origin);
	vial_error (*position)(void *self, size_t *position);
	vial_error (*available)(void *self, size_t *available);
	vial_error (*read)(void *self, void *buf, size_t size);
	vial_error (*write)(void *self, const void *buf, size_t size);
VIAL_END_VIRTUALS;

VIAL_BEGIN_CLASS_DECL(vial_stream_io_error, vial_io_error) VIAL_END_CLASS_DECL;
VIAL_BEGIN_VIRTUALS(vial_stream_io_error) VIAL_END_VIRTUALS;

vial_error vial_stream_io_error_new(const char *message, vial_error cause);

static inline void vial_stream_dispose(struct vial_stream *self)
{
	struct vial_object *obj = vial_interface_ptr(self);
	obj->vtable->dispose(obj);
}

static inline vial_error vial_stream_capabilities(struct vial_stream *self, int *capabilities)
{
	return self->vtable->capabilities(vial_interface_ptr(self), capabilities);
}

static inline vial_error vial_stream_close(struct vial_stream *self)
{
	return self->vtable->close(vial_interface_ptr(self));
}

static inline vial_error vial_stream_flush(struct vial_stream *self)
{
	return self->vtable->flush(vial_interface_ptr(self));
}

static inline vial_error vial_stream_seek(struct vial_stream *self, long offset, enum vial_stream_seek origin)
{
	return self->vtable->seek(vial_interface_ptr(self), offset, origin);
}

static inline vial_error vial_stream_position(struct vial_stream *self, size_t *position)
{
	return self->vtable->position(vial_interface_ptr(self), position);
}

static inline vial_error vial_stream_available(struct vial_stream *self, size_t *available)
{
	return self->vtable->available(vial_interface_ptr(self), available);
}

static inline vial_error vial_stream_read(struct vial_stream *self, void *buf, size_t size)
{
	return self->vtable->read(vial_interface_ptr(self), buf, size);
}

static inline vial_error vial_stream_write(struct vial_stream *self, const void *buf, size_t size)
{
	return self->vtable->write(vial_interface_ptr(self), buf, size);
}

vial_error vial_stream_print(struct vial_stream *self, const char *str);

#endif
