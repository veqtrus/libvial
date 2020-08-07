/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_COLLECTIONS_MAP_H
#define VIAL_COLLECTIONS_MAP_H

#include <stddef.h>

#include <vial/def.h>
#include <vial/func.h>

struct vial_map_node {
	struct vial_map_node *left, *right;
	int height;
};

#define vial_map(K, V) struct { \
	size_t size; \
	unsigned node_size, key_offset, value_offset; \
	vial_comp_f comp; \
	vial_dispose_f dispose_key, dispose_value; \
	struct vial_map_node *root; \
	struct { \
		struct vial_map_node node; \
		K key; \
		V value; \
	} *tmp_node; \
	K tmp_key; \
}

typedef vial_map(void *, void *) vial_map_of_voidp_voidp;
typedef vial_map(const char *, const char *) vial_map_of_cstr_cstr;

#define vial_map_init(self, cmp) do { \
	(self).size = 0; \
	(self).node_size = sizeof(*(self).tmp_node); \
	(self).key_offset = vial_ptr_diff(&(self).tmp_node->key, (self).tmp_node); \
	(self).value_offset = vial_ptr_diff(&(self).tmp_node->value, (self).tmp_node); \
	(self).comp = (cmp); \
	(self).dispose_value = (self).dispose_key = NULL; \
	(self).root = NULL; \
} while (0)

void _vial_map_dispose(vial_map_of_voidp_voidp *self, struct vial_map_node *node);

#define vial_map_dispose(self) _vial_map_dispose((vial_map_of_voidp_voidp *) &(self), (self).root)

void *_vial_map_get(vial_map_of_voidp_voidp *self, const void *key);

#define vial_map_get(self, k) ((self).tmp_key = (k), \
	(((self).tmp_node = _vial_map_get((vial_map_of_voidp_voidp *) &(self), &(self).tmp_key)) != NULL \
	? &(self).tmp_node->value : NULL))

#define vial_map_contains(self, k) ((self).tmp_key = (k), \
	_vial_map_get((vial_map_of_voidp_voidp *) &(self), &(self).tmp_key) != NULL)

void *_vial_map_put(vial_map_of_voidp_voidp *self, struct vial_map_node **p_node, const void *key);

#define vial_map_put(self, k, v) do { \
	(self).tmp_key = (k); \
	(self).tmp_node = _vial_map_put((vial_map_of_voidp_voidp *) &(self), &(self).root, &(self).tmp_key); \
	(self).tmp_node->key = (self).tmp_key; (self).tmp_node->value = (v); \
} while (0)

void _vial_map_remove(vial_map_of_voidp_voidp *self, struct vial_map_node **p_node, const void *key);

#define vial_map_remove(self, k) do { \
	(self).tmp_key = (k); \
	_vial_map_remove((vial_map_of_voidp_voidp *) &(self), &(self).root, &(self).tmp_key); \
} while (0)

void _vial_map_foreach(vial_map_of_voidp_voidp *self, struct vial_map_node *node, vial_biconsumer_f action, void *ctx);

#define vial_map_foreach(self, action, ctx) _vial_map_foreach((vial_map_of_voidp_voidp *) &(self), (self).root, (action), (ctx))

#endif
