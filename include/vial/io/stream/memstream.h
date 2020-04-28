/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_IO_MEMSTREAM_H
#define VIAL_IO_MEMSTREAM_H

#include <stddef.h>
#include <stdbool.h>

#include <vial/io/stream/stream.h>

struct vial_memstream {
	struct vial_stream stream;
	char *buf;
	size_t allocated, size, position;
	bool resizeable;
};

vial_error vial_memstream_init_buf(struct vial_memstream *self, void *buf, size_t size);

vial_error vial_memstream_init(struct vial_memstream *self, size_t size);

static inline vial_error vial_memstream_copyto(struct vial_memstream *self, struct vial_stream *dst)
{
	return dst->vtable->write(dst, self->buf, self->size);
}

#endif
