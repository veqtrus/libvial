/*
Copyright (c) 2019-20 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/stream/stream.h>

#include <string.h>

VIAL_BEGIN_CLASS_DEF(vial_stream_io_error, vial_io_error) VIAL_END_CLASS_DEF;

vial_error vial_stream_io_error_new(const char *message, vial_error cause)
{
	vial_error err = vial_error_new(message, cause);
	err->as_vial_object.typeinfo = &vial_stream_io_error_typeinfo;
	return err;
}

vial_error vial_stream_print(struct vial_stream *self, const char *str)
{
	return self->vtable->write(self, str, strlen(str));
}
