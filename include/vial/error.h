/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_ERROR_H
#define VIAL_ERROR_H

#include <vial/types.h>

#include <stddef.h>

typedef struct vial_error *vial_error;

struct vial_error_trace {
	struct vial_error_trace *prev;
	const char *file;
	int line;
};

VIAL_BEGIN_CLASS_DECL(vial_error, vial_object)
	char *message;
	vial_error cause;
	struct vial_error_trace *trace;
VIAL_END_CLASS_DECL;

VIAL_BEGIN_VIRTUALS(vial_error) VIAL_END_VIRTUALS;

void vial_error_add_frame(vial_error err, const char *file, int line);

void vial_error_free(vial_error self);

void vial_error_print(vial_error self);

#define vial_error_throw(err) do { \
	vial_error vial_error_x = (err); \
	if (vial_error_x == NULL) \
		vial_error_x = vial_null_error_new(NULL, NULL); \
	vial_error_add_frame(vial_error_x, __FILE__, __LINE__); \
	return vial_error_x; \
} while (0)

#define vial_error_rethrow(err) do { \
	vial_error vial_error_x = (err); \
	if (vial_error_x != NULL) { \
		vial_error_add_frame(vial_error_x, __FILE__, __LINE__); \
		return vial_error_x; \
	} \
} while (0)

VIAL_BEGIN_CLASS_DECL(vial_arg_error, vial_error) VIAL_END_CLASS_DECL; VIAL_BEGIN_VIRTUALS(vial_arg_error) VIAL_END_VIRTUALS;
VIAL_BEGIN_CLASS_DECL(vial_argnull_error, vial_error) VIAL_END_CLASS_DECL; VIAL_BEGIN_VIRTUALS(vial_argnull_error) VIAL_END_VIRTUALS;
VIAL_BEGIN_CLASS_DECL(vial_argrange_error, vial_error) VIAL_END_CLASS_DECL; VIAL_BEGIN_VIRTUALS(vial_argrange_error) VIAL_END_VIRTUALS;
VIAL_BEGIN_CLASS_DECL(vial_disposed_error, vial_error) VIAL_END_CLASS_DECL; VIAL_BEGIN_VIRTUALS(vial_disposed_error) VIAL_END_VIRTUALS;
VIAL_BEGIN_CLASS_DECL(vial_format_error, vial_error) VIAL_END_CLASS_DECL; VIAL_BEGIN_VIRTUALS(vial_format_error) VIAL_END_VIRTUALS;
VIAL_BEGIN_CLASS_DECL(vial_index_error, vial_error) VIAL_END_CLASS_DECL; VIAL_BEGIN_VIRTUALS(vial_index_error) VIAL_END_VIRTUALS;
VIAL_BEGIN_CLASS_DECL(vial_invop_error, vial_error) VIAL_END_CLASS_DECL; VIAL_BEGIN_VIRTUALS(vial_invop_error) VIAL_END_VIRTUALS;
VIAL_BEGIN_CLASS_DECL(vial_io_error, vial_error) VIAL_END_CLASS_DECL; VIAL_BEGIN_VIRTUALS(vial_io_error) VIAL_END_VIRTUALS;
VIAL_BEGIN_CLASS_DECL(vial_key_error, vial_error) VIAL_END_CLASS_DECL; VIAL_BEGIN_VIRTUALS(vial_key_error) VIAL_END_VIRTUALS;
VIAL_BEGIN_CLASS_DECL(vial_notimpl_error, vial_error) VIAL_END_CLASS_DECL; VIAL_BEGIN_VIRTUALS(vial_notimpl_error) VIAL_END_VIRTUALS;
VIAL_BEGIN_CLASS_DECL(vial_notsup_error, vial_error) VIAL_END_CLASS_DECL; VIAL_BEGIN_VIRTUALS(vial_notsup_error) VIAL_END_VIRTUALS;
VIAL_BEGIN_CLASS_DECL(vial_null_error, vial_error) VIAL_END_CLASS_DECL; VIAL_BEGIN_VIRTUALS(vial_null_error) VIAL_END_VIRTUALS;
VIAL_BEGIN_CLASS_DECL(vial_posix_error, vial_error) VIAL_END_CLASS_DECL; VIAL_BEGIN_VIRTUALS(vial_posix_error) VIAL_END_VIRTUALS;

vial_error vial_error_new(const char *message, vial_error cause);
vial_error vial_arg_error_new(const char *message, vial_error cause);
vial_error vial_argnull_error_new(const char *message, vial_error cause);
vial_error vial_argrange_error_new(const char *message, vial_error cause);
vial_error vial_disposed_error_new(const char *message, vial_error cause);
vial_error vial_format_error_new(const char *message, vial_error cause);
vial_error vial_index_error_new(const char *message, vial_error cause);
vial_error vial_invop_error_new(const char *message, vial_error cause);
vial_error vial_io_error_new(const char *message, vial_error cause);
vial_error vial_key_error_new(const char *message, vial_error cause);
vial_error vial_notimpl_error_new(const char *message, vial_error cause);
vial_error vial_notsup_error_new(const char *message, vial_error cause);
vial_error vial_null_error_new(const char *message, vial_error cause);
vial_error vial_posix_error_new(const char *message, vial_error cause);

vial_error vial_throws_notimpl_error();
vial_error vial_throws_notsup_error();

#endif
