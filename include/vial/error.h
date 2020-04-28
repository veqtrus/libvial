/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_ERROR_H
#define VIAL_ERROR_H

typedef struct vial_error *vial_error;

struct vial_error {
	char *id, *message;
	vial_error cause;
	const char *file;
	int line;
};

vial_error vial_error_make(const char *id, const char *message, vial_error cause, const char *file, int line);

void vial_error_free(vial_error self);

void vial_error_print(vial_error self);

#define vial_error_new(id, message, cause) vial_error_make((id), (message), (cause), __FILE__, __LINE__)

#define vial_error_rethrow(err) for (vial_error vial_error_x = (err); vial_error_x;) \
	return vial_error_make(vial_error_x->id, vial_error_x->message, vial_error_x, __FILE__, __LINE__)

#endif
