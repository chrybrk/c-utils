#include "cutils.h"

#define new malloc

struct Vec3 {
	float x, y, z;
};

typedef struct NODE_STRUCT {
	struct NODE_STRUCT *next;
	void *data;
} node_T;

int main(void)
{
	llist_T *list = unwrap(init_llist());

	node_T *head = llist_push(list, &(struct Vec3){ .x = 10.0f, .y = 20.0f, .z = 30.0f });
	llist_push(list, &(struct Vec3){ .x = 20.0f, .y = 30.0f, .z = 40.0f });
	llist_push(list, &(struct Vec3){ .x = 30.0f, .y = 40.0f, .z = 50.0f });
	llist_push(list, &(struct Vec3){ .x = 40.0f, .y = 50.0f, .z = 60.0f });

	struct Vec3 *p = unwrap((struct Vec3*)llist_get(list, 4));
	printf("%f, %f, %f\n", p->x, p->y, p->z);

	return 0;
}
