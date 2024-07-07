#include <stdio.h>
#include <stdlib.h>
#include "cutils.h"

/*
 * linked list
 * dynamic list
 * fixed allocator
 * heap allocator
 * arena allocator
 * hashmap <T, M>
 * hashmap -> hashmap to list convertor
 * iterator -> list, allocator, map
 *
 * ultimate goal:
 * 	BTree
 * 	B+Tree
 * 	KV disk persistant
*/

struct Vector3 {
	char *name;
	int age;
};

int main(void)
{
	list_T *list = init_list(sizeof(struct Vector3));

	struct Vector3 pos = { .name = "Test", .age = 10 };

	for (int i = 0; i < 4096; i++)
		list_push(list, &pos);

	list_free(list);

	return 0;
}
