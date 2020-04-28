/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/error.h>

#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static char *error_strdup(const char *s)
{
	if (s == NULL) return NULL;
	size_t size = strlen(s) + 1;
	char *d = malloc(size);
	memcpy(d, s, size);
	return d;
}

vial_error vial_error_make(const char *id, const char *message, vial_error cause, const char *file, int line)
{
	vial_error error = malloc(sizeof(*error));
	error->id = error_strdup(id);
	error->message = error_strdup(message);
	error->cause = cause;
	error->file = file;
	error->line = line;
	return error;
}

void vial_error_free(vial_error self)
{
	for (vial_error next; self; self = next) {
		next = self->cause;
		free(self->id);
		free(self->message);
		free(self);
	}
}

void vial_error_print(vial_error self)
{
	for (; self; self = self->cause)
		fprintf(stderr, "\nIn \"%s\" line %d:\n%s\n%s\n\n",
			self->file, self->line, self->id, self->message);
}
