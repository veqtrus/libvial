/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_IO_MEMSTREAM_H
#define VIAL_IO_MEMSTREAM_H

#include <stddef.h>
#include <stdbool.h>

#include <vial/io/stream.h>

struct vMemStream {
	struct vStream stream;
	char *buf;
	size_t allocated, size, position;
	bool resizeable;
};

error_t vMemStream_init_buf(struct vMemStream *self, void *buf, size_t size);

error_t vMemStream_init(struct vMemStream *self, size_t size);

static inline error_t vMemStream_copyto(struct vMemStream *self, struct vStream *dst)
{
	return dst->vtable->write(dst, self->buf, self->size);
}

#endif
