/*
Copyright (c) 2019-20 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/error.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vial/string.h>

VIAL_BEGIN_CLASS_DEF(vial_error, vial_object) VIAL_END_CLASS_DEF;

void vial_error_add_frame(vial_error err, const char* file, int line)
{
	struct vial_error_trace *frame = malloc(sizeof(*frame));
	frame->prev = err->trace;
	frame->file = file;
	frame->line = line;
	err->trace = frame;
}

void vial_error_free(vial_error self)
{
	if (self != NULL) {
		self->as_vial_object.vtable->dispose(self);
		free(self);
	}
}

void vial_error_print(vial_error self)
{
	struct vial_error_trace *frame;
	fputs("\nERROR\n-----\n", stderr);
	for (; self != NULL; self = self->cause) {
		fprintf(stderr, "Type: %s\n", self->as_vial_object.typeinfo->name);
		if (self->message != NULL)
			fprintf(stderr, "Message: \"%s\"\n", self->message);
		fputs("Trace:", stderr);
		for (frame = self->trace; frame != NULL; frame = frame->prev)
			fprintf(stderr, "%s:%d\n", frame->file, frame->line);
		if (self->cause != NULL)
			fputs("\nCause:\n", stderr);
	}
}

VIAL_BEGIN_CLASS_DEF(vial_arg_error, vial_error) VIAL_END_CLASS_DEF;
VIAL_BEGIN_CLASS_DEF(vial_argnull_error, vial_error) VIAL_END_CLASS_DEF;
VIAL_BEGIN_CLASS_DEF(vial_argrange_error, vial_error) VIAL_END_CLASS_DEF;
VIAL_BEGIN_CLASS_DEF(vial_disposed_error, vial_error) VIAL_END_CLASS_DEF;
VIAL_BEGIN_CLASS_DEF(vial_format_error, vial_error) VIAL_END_CLASS_DEF;
VIAL_BEGIN_CLASS_DEF(vial_index_error, vial_error) VIAL_END_CLASS_DEF;
VIAL_BEGIN_CLASS_DEF(vial_invop_error, vial_error) VIAL_END_CLASS_DEF;
VIAL_BEGIN_CLASS_DEF(vial_io_error, vial_error) VIAL_END_CLASS_DEF;
VIAL_BEGIN_CLASS_DEF(vial_key_error, vial_error) VIAL_END_CLASS_DEF;
VIAL_BEGIN_CLASS_DEF(vial_notimpl_error, vial_error) VIAL_END_CLASS_DEF;
VIAL_BEGIN_CLASS_DEF(vial_notsup_error, vial_error) VIAL_END_CLASS_DEF;
VIAL_BEGIN_CLASS_DEF(vial_null_error, vial_error) VIAL_END_CLASS_DEF;
VIAL_BEGIN_CLASS_DEF(vial_posix_error, vial_error) VIAL_END_CLASS_DEF;

static void impl_dispose(void *p)
{
	struct vial_error_trace *frame, *next_frame;
	vial_error err = p;
	free(err->message);
	for (frame = err->trace; frame != NULL; frame = next_frame) {
		next_frame = frame->prev;
		free(frame);
	}
	vial_error_free(err->cause);
}

static const struct vial_object_vtable vtable = {
	impl_dispose
};

vial_error vial_error_new(const char *message, vial_error cause)
{
	vial_error err = malloc(sizeof(*err));
	err->message = (message != NULL) ? vial_strdup(message) : NULL;
	err->as_vial_object.typeinfo = &vial_error_typeinfo;
	err->as_vial_object.vtable = &vtable;
	err->cause = cause;
	err->trace = NULL;
	return err;
}

vial_error vial_arg_error_new(const char *message, vial_error cause)
{
	vial_error err = vial_error_new(message, cause);
	err->as_vial_object.typeinfo = &vial_arg_error_typeinfo;
	return err;
}

vial_error vial_argnull_error_new(const char *message, vial_error cause)
{
	vial_error err = vial_error_new(message, cause);
	err->as_vial_object.typeinfo = &vial_argnull_error_typeinfo;
	return err;
}

vial_error vial_argrange_error_new(const char *message, vial_error cause)
{
	vial_error err = vial_error_new(message, cause);
	err->as_vial_object.typeinfo = &vial_argrange_error_typeinfo;
	return err;
}

vial_error vial_disposed_error_new(const char *message, vial_error cause)
{
	vial_error err = vial_error_new(message, cause);
	err->as_vial_object.typeinfo = &vial_disposed_error_typeinfo;
	return err;
}

vial_error vial_format_error_new(const char *message, vial_error cause)
{
	vial_error err = vial_error_new(message, cause);
	err->as_vial_object.typeinfo = &vial_format_error_typeinfo;
	return err;
}

vial_error vial_index_error_new(const char *message, vial_error cause)
{
	vial_error err = vial_error_new(message, cause);
	err->as_vial_object.typeinfo = &vial_index_error_typeinfo;
	return err;
}

vial_error vial_invop_error_new(const char *message, vial_error cause)
{
	vial_error err = vial_error_new(message, cause);
	err->as_vial_object.typeinfo = &vial_invop_error_typeinfo;
	return err;
}

vial_error vial_io_error_new(const char *message, vial_error cause)
{
	vial_error err = vial_error_new(message, cause);
	err->as_vial_object.typeinfo = &vial_io_error_typeinfo;
	return err;
}

vial_error vial_key_error_new(const char *message, vial_error cause)
{
	vial_error err = vial_error_new(message, cause);
	err->as_vial_object.typeinfo = &vial_key_error_typeinfo;
	return err;
}

vial_error vial_notimpl_error_new(const char *message, vial_error cause)
{
	vial_error err = vial_error_new(message, cause);
	err->as_vial_object.typeinfo = &vial_notimpl_error_typeinfo;
	return err;
}

vial_error vial_notsup_error_new(const char *message, vial_error cause)
{
	vial_error err = vial_error_new(message, cause);
	err->as_vial_object.typeinfo = &vial_notsup_error_typeinfo;
	return err;
}

vial_error vial_null_error_new(const char *message, vial_error cause)
{
	vial_error err = vial_error_new(message, cause);
	err->as_vial_object.typeinfo = &vial_null_error_typeinfo;
	return err;
}

vial_error vial_posix_error_new(const char *message, vial_error cause)
{
	vial_error err = vial_error_new(message, cause);
	err->as_vial_object.typeinfo = &vial_posix_error_typeinfo;
	return err;
}

vial_error vial_throws_notimpl_error(void);
vial_error vial_throws_notimpl_error(void)
{
	return vial_notimpl_error_new(NULL, NULL);
}

vial_error vial_throws_notsup_error(void);
vial_error vial_throws_notsup_error(void)
{
	return vial_notsup_error_new(NULL, NULL);
}
