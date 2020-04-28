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

struct vial_filestream {
	struct vial_stream stream;
	FILE *file;
};

vial_error vial_filestream_init(struct vial_filestream *self, FILE *file);

#endif
