/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/stream.h>

#include <string.h>

const char *const VSTREAM_IO_ERROR = "STREAM_IO_ERROR";
const char *const VSTREAM_DISPOSED = "STREAM_DISPOSED";
const char *const VSTREAM_NOT_SUPPORTED = "STREAM_NOT_SUPPORTED";

error_t vStream_print(struct vStream *self, const char *str)
{
	return self->vtable->write(self, str, strlen(str));
}
