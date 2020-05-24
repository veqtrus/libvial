/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <stdio.h>
#include <string.h>

#include <vial/collections/map.h>

typedef vial_map(char, char) char_map;

static int comp_char(const char *a, const char *b)
{
	const int va = *a, vb = *b;
	return va - vb;
}

static void print_avl(char_map *map, struct vial_map_node *node, int level)
{
	if (node == NULL)
		return;
	print_avl(map, node->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("    ");
	map->tmp_node = (void *) node;
	printf("%c(%d)\n", map->tmp_node->key, node->height);
	print_avl(map, node->left, level + 1);
}

static void test_modification()
{
	char_map map;
	char key;
	vial_map_init(map, (vial_comp_f) comp_char);
	for (key = 'a'; key <= 'z'; key++)
		vial_map_put(map, key, key);
	print_avl(&map, map.root, 0);
	for (key = 'k'; key <= 'p'; key++) {
		printf("Removing '%c'\n", key);
		vial_map_remove(map, key);
	}
	print_avl(&map, map.root, 0);
}

int main()
{
	test_modification();
	return 0;
}
