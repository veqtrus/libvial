/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/types.h>

bool vial_extends(const struct vial_typeinfo *child, const struct vial_typeinfo *parent)
{
	while (child != NULL) {
		if (child == parent)
			return true;
		child = child->extends;
	}
	return false;
}

const struct vial_typeinfo vial_object_typeinfo = { NULL, "vial_object" };

static const struct vial_object_vtable vtable = {
	vial_dispose_nop
};

void vial_object_init(struct vial_object *self)
{
	self->vtable = &vtable;
	self->typeinfo = &vial_object_typeinfo;
}
