/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_IO_STATSTREAM_H
#define VIAL_IO_STATSTREAM_H

#include <stddef.h>

#include <vial/io/stream/stream.h>

struct vStatStream {
	struct vStream stream;
	size_t size, position;
};

error_t vStatStream_init(struct vStatStream *self);

#endif
