/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_IO_FILESTREAM_H
#define VIAL_IO_FILESTREAM_H

#include <stdio.h>

#include <vial/io/stream.h>

struct vFileStream {
	struct vStream stream;
	FILE *file;
};

error_t vFileStream_init(struct vFileStream *self, FILE *file);

#endif
