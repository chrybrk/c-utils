#include "cutils.h"

void **__alloc_buffer__;
ssize_t __alloc_index__ = 0;

void *__alloc_function__(ssize_t nbytes)
{
	__alloc_index__++;

	if (__alloc_buffer__)
	{
		__alloc_buffer__ = realloc(__alloc_buffer__, __alloc_index__ * nbytes);
	}
	else __alloc_buffer__ = malloc(nbytes);

	printf("malloc: %ld bytes have been allocated.\n", nbytes);

	__alloc_buffer__[__alloc_index__ - 1] = malloc(nbytes);

	return unwrap(__alloc_buffer__[__alloc_index__ - 1]);
}

void *__unwrap_log__(void *value, char *file, int line, char *function)
{
	if (value == NULL)
	{
		fprintf(stdout, "%s:%d :: In %s: \n\tExpected not NULL value, but got NULL.\n", file, line, function);
		exit(EXIT_FAILURE);
	}
	
	return value;
}

typedef struct LIST_STRUCT
{
	void **buffer;
	ssize_t index;
	ssize_t buffer_size;
	ssize_t item_size;
} list_T;

list_T *init_list(ssize_t item_size)
{
	list_T *list = alloc(void*, sizeof(struct LIST_STRUCT));
	unwrap(list);

	list->buffer_size = 16;
	list->item_size = item_size;
	list->index = 0;

	list->buffer = alloc(void*, list->buffer_size * list->item_size);
	unwrap(list->buffer);

	return list;
}

void list_push(list_T *list, void *value)
{
	if (list->index >= list->buffer_size)
	{
		list->buffer_size = list->buffer_size * list->buffer_size;
		list->buffer = realloc(list->buffer, list->buffer_size * list->item_size);
	}

	list->buffer[list->index] = unwrap(value);
	list->index++;
}

void *list_pop(list_T *list, ssize_t index)
{
	if (index < list->index)
	{
		void *value = list->buffer[index];
		for (ssize_t i = index; i < list->index; i++)
			list->buffer[i] = list->buffer[i + 1];

		return value;
	}

	return NULL;
}

void *list_get(list_T *list, ssize_t index)
{
	return index < list->index ? list->buffer[index] : NULL;
}

void list_extend(list_T *listA, list_T *listB)
{
	for (ssize_t i = 0; i < listB->index; i++)
		list_push(listA, unwrap(list_get(listB, i)));
}

void list_free(list_T *list)
{
	free(list->buffer);
	free(list);
}
