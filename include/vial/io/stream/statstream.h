/*
Copyright (c) 2019-20 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_IO_STATSTREAM_H
#define VIAL_IO_STATSTREAM_H

#include <stddef.h>

#include <vial/io/stream/stream.h>

VIAL_BEGIN_CLASS_DECL(vial_statstream, vial_object)
	VIAL_IMPLEMENTS(vial_stream);
	struct vial_stream stream;
	size_t size, position;
VIAL_END_CLASS_DECL;
VIAL_BEGIN_VIRTUALS(vial_statstream) VIAL_END_VIRTUALS;

vial_error vial_statstream_init(struct vial_statstream *self);

#endif
