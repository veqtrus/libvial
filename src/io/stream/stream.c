/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/stream/stream.h>

#include <string.h>

const char *const VIAL_STREAM_IO_ERROR = "VIAL_STREAM_IO_ERROR";
const char *const VIAL_STREAM_DISPOSED = "VIAL_STREAM_DISPOSED";
const char *const VIAL_STREAM_NOT_SUPPORTED = "VIAL_STREAM_NOT_SUPPORTED";

vial_error vial_stream_print(struct vial_stream *self, const char *str)
{
	return self->vtable->write(self, str, strlen(str));
}
