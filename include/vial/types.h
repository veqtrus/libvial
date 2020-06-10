/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_TYPES_H
#define VIAL_TYPES_H

#include <stddef.h>
#include <stdbool.h>

#include <vial/func.h>

struct vial_typeinfo {
	const struct vial_typeinfo *extends;
	const char *name;
};

bool vial_extends(const struct vial_typeinfo *child, const struct vial_typeinfo *parent);

#define VIAL_BEGIN_CLASS_DECL(name, base) extern const struct vial_typeinfo name##_typeinfo; \
	struct name { struct base as_##base; const struct name##_vtable *vtable;

#define VIAL_END_CLASS_DECL }

#define VIAL_BEGIN_VIRTUALS(name) struct name##_vtable {

#define VIAL_END_VIRTUALS size_t offset; }

#define VIAL_BEGIN_CLASS_DEF(name, base) \
	const struct vial_typeinfo name##_typeinfo = { &base##_typeinfo, #name }; \
	static const struct name##_vtable name##_vtable = {

#define VIAL_END_CLASS_DEF 0 }

#define VIAL_INTERFACE_DECL(name) struct name { const struct name##_vtable *vtable; }

#define VIAL_IMPLEMENTS(ifname) struct ifname as_##ifname

#define VIAL_BEGIN_IMPLEMENTATION(name, base) static const struct base##_vtable name##_as_##base = {

#define VIAL_END_IMPLEMENTATION(name, base) offsetof(struct name, as_##base) }

#define vial_interface_ptr(p_obj) ((void *) (((char *) (p_obj)) - (p_obj)->vtable->offset))
#define vial_interface_cptr(p_obj) ((const void *) (((const char *) (p_obj)) - (p_obj)->vtable->offset))

VIAL_BEGIN_VIRTUALS(vial_object)
	vial_dispose_f dispose;
VIAL_END_VIRTUALS;
struct vial_object {
	const struct vial_object_vtable *vtable;
	const struct vial_typeinfo *typeinfo;
};

extern const struct vial_typeinfo vial_object_typeinfo;

void vial_object_init(struct vial_object *self);

static inline
bool vial_instanceof(const struct vial_object *obj, const struct vial_typeinfo *typeinfo)
{
	return (obj != NULL) ? vial_extends(obj->typeinfo, typeinfo) : false;
}

#endif
