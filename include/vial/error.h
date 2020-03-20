/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_ERROR_H
#define VIAL_ERROR_H

typedef struct vError {
	char *id, *message;
	struct vError *cause;
	const char *file;
	int line;
} *error_t;

error_t error_make(const char *id, const char *message, error_t cause, const char *file, int line);

void error_free(error_t self);

void error_print(error_t self);

#define error_new(id, message, cause) error_make((id), (message), (cause), __FILE__, __LINE__)

#define error_rethrow(err) for (error_t error_x = (err); error_x;) \
	return error_make(error_x->id, error_x->message, error_x, __FILE__, __LINE__)

#endif
