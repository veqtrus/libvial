/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_IO_FILESTREAM_H
#define VIAL_IO_FILESTREAM_H

#include <stdio.h>

#include <vial/io/stream/stream.h>

VIAL_BEGIN_CLASS_DECL(vial_filestream, vial_object)
	VIAL_IMPLEMENTS(vial_stream);
	FILE *file;
VIAL_END_CLASS_DECL;
VIAL_BEGIN_VIRTUALS(vial_filestream) VIAL_END_VIRTUALS;

vial_error vial_filestream_init(struct vial_filestream *self, FILE *file);

#endif
