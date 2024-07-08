#include <stdio.h>
#include <stdlib.h>
#include "cutils.h"

struct Vector3 {
	char *name;
	int age;
};

int main(void)
{
	list_T *list = unwrap(init_list(sizeof(struct Vector3)));

	struct Vector3 pos = { .name = "Test", .age = 10 };

	printf("%ld\n", sizeof(struct Vector3) * 16000000);

	for (int i = 0; i < 16000000; i++)
		list_push(list, &pos);

	list_free(list);

	return 0;
}
