/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/collections/map.h>

#include <stdlib.h>
#include <string.h>

void _vial_map_dispose(vial_map_of_voidp_voidp *self, struct vial_map_node *node)
{
	if (node == NULL)
		return;
	_vial_map_dispose(self, node->left);
	_vial_map_dispose(self, node->right);
	if (self->dispose_key != NULL)
		self->dispose_key(((char *) node) + self->key_offset);
	if (self->dispose_value != NULL)
		self->dispose_value(((char *) node) + self->value_offset);
	free(node);
}

void *_vial_map_get(vial_map_of_voidp_voidp *self, struct vial_map_node *node, const void *key)
{
	int cmp;
	while (node != NULL) {
		cmp = self->comp(key, ((char *) node) + self->key_offset);
		if (cmp < 0)
			node = node->left;
		else if (cmp > 0)
			node = node->right;
		else
			return node;
	}
	return NULL;
}

static int node_height(struct vial_map_node *node)
{
	int l, r;
	if (node == NULL)
		return 0;
	l = node->left != NULL ? node->left->height : 0;
	r = node->right != NULL ? node->right->height : 0;
	return 1 + (l > r ? l : r);
}

enum rotation {
	ROTATION_NONE,
	ROTATION_LEFT,
	ROTATION_RIGHT
};

static void node_rotate(struct vial_map_node **p_node, enum rotation rot)
{
	struct vial_map_node *old_top, *new_top;
	old_top = *p_node;
	if (rot == ROTATION_RIGHT) {
		new_top = old_top->left;
		old_top->left = new_top->right;
		new_top->right = old_top;
	} else if (rot == ROTATION_LEFT) {
		new_top = old_top->right;
		old_top->right = new_top->left;
		new_top->left = old_top;
	} else {
		return;
	}
	old_top->height = node_height(old_top);
	new_top->height = node_height(new_top);
	*p_node = new_top;
}

static void node_rebalance(struct vial_map_node **p_node)
{
	struct vial_map_node **p_child, *node, *child;
	int l, r, d;
	enum rotation first, second;
	node = *p_node;
	if (node == NULL)
		return;
	l = node->left != NULL ? node->left->height : 0;
	r = node->right != NULL ? node->right->height : 0;
	if (l > r) {
		d = l - r;
		node->height = l + 1;
		p_child = &node->left;
		second = ROTATION_RIGHT;
	} else {
		d = r - l;
		node->height = r + 1;
		p_child = &node->right;
		second = ROTATION_LEFT;
	}
	if (d < 2)
		return;
	child = *p_child;
	l = child->left != NULL ? child->left->height : 0;
	r = child->right != NULL ? child->right->height : 0;
	if (l > r)
		first = (second == ROTATION_LEFT) ? ROTATION_RIGHT : ROTATION_NONE;
	else
		first = (second == ROTATION_RIGHT) ? ROTATION_LEFT : ROTATION_NONE;
	node_rotate(p_child, first);
	node_rotate(p_node, second);
}

void *_vial_map_put(vial_map_of_voidp_voidp *self, struct vial_map_node **p_node, const void *key)
{
	void *result;
	struct vial_map_node *node = *p_node;
	if (node == NULL) {
		*p_node = node = malloc(self->node_size);
		node->height = 1;
		node->right = node->left = NULL;
		self->size++;
		return node;
	}
	int cmp = self->comp(key, ((char *) node) + self->key_offset);
	if (cmp < 0) {
		result = _vial_map_put(self, &node->left, key);
	} else if (cmp > 0) {
		result = _vial_map_put(self, &node->right, key);
	} else {
		if (self->dispose_key != NULL)
			self->dispose_key(((char *) node) + self->key_offset);
		if (self->dispose_value != NULL)
			self->dispose_value(((char *) node) + self->value_offset);
		return node;
	}
	node_rebalance(p_node);
	return result;
}

void _vial_map_remove(vial_map_of_voidp_voidp *self, struct vial_map_node **p_node, const void *key)
{
	struct vial_map_node *node = *p_node;
	if (node == NULL)
		return;
	int cmp = self->comp(key, ((char *) node) + self->key_offset);
	if (cmp < 0) {
		_vial_map_remove(self, &node->left, key);
	} else if (cmp > 0) {
		_vial_map_remove(self, &node->right, key);
	} else {
		if (self->dispose_key != NULL)
			self->dispose_key(((char *) node) + self->key_offset);
		if (self->dispose_value != NULL)
			self->dispose_value(((char *) node) + self->value_offset);
		free(node);
		*p_node = NULL;
		self->size--;
		return;
	}
	node_rebalance(p_node);
}

void _vial_map_foreach(vial_map_of_voidp_voidp *self, struct vial_map_node *node, vial_biconsumer_f action, void *ctx)
{
	if (node == NULL)
		return;
	_vial_map_foreach(self, node->left, action, ctx);
	action(((char *) node) + self->key_offset, ((char *) node) + self->value_offset, ctx);
	_vial_map_foreach(self, node->right, action, ctx);
}
