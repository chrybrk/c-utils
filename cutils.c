#include "cutils.h"

/*
			 █████████                                         █████  █████  █████     ███  ████         
			███░░░░░███                                       ░░███  ░░███  ░░███     ░░░  ░░███         
		 ███     ░░░   ██████  ████████   ██████             ░███   ░███  ███████   ████  ░███   █████ 
		░███          ███░░███░░███░░███ ███░░███ ██████████ ░███   ░███ ░░░███░   ░░███  ░███  ███░░  
		░███         ░███ ░███ ░███ ░░░ ░███████ ░░░░░░░░░░  ░███   ░███   ░███     ░███  ░███ ░░█████ 
		░░███     ███░███ ░███ ░███     ░███░░░              ░███   ░███   ░███ ███ ░███  ░███  ░░░░███
		 ░░█████████ ░░██████  █████    ░░██████             ░░████████    ░░█████  █████ █████ ██████ 
			░░░░░░░░░   ░░░░░░  ░░░░░      ░░░░░░               ░░░░░░░░      ░░░░░  ░░░░░ ░░░░░ ░░░░░░  
*/                                                                                               

void *__unwrap_log__(void *value, char *file, int line, char *function)
{
	if (value == NULL)
	{
		fprintf(stdout, "%s:%d :: In %s: \n\tExpected not NULL value, but got NULL.\n", file, line, function);
		exit(EXIT_FAILURE);
	}
	
	return value;
}

/*
			 █████        ███           █████   
			░░███        ░░░           ░░███    
			 ░███        ████   █████  ███████  
			 ░███       ░░███  ███░░  ░░░███░   
			 ░███        ░███ ░░█████   ░███    
			 ░███      █ ░███  ░░░░███  ░███ ███
			 ███████████ █████ ██████   ░░█████ 
			░░░░░░░░░░░ ░░░░░ ░░░░░░     ░░░░░  
*/

typedef struct LIST_STRUCT
{
	void **buffer;
	ssize_t index;
	ssize_t buffer_size;
	ssize_t item_size;
} list_T;

list_T *init_list(ssize_t item_size)
{
	list_T *list = malloc(sizeof(struct LIST_STRUCT));
	unwrap(list);

	list->buffer_size = 16;
	list->item_size = item_size;
	list->index = 0;

	list->buffer = malloc(list->buffer_size * list->item_size);
	unwrap(list->buffer);

	return list;
}

void list_push(list_T *list, void *value)
{
	if (list->index >= list->buffer_size)
	{
		if (list->buffer_size == __LIST_MAX_MEMEORY_CONSUME__)
			printf("list :: exceeded 16MB limit.\n"), exit(EXIT_FAILURE);

		list->buffer_size = list->buffer_size * list->buffer_size;
		list->buffer_size = list->buffer_size > __LIST_MAX_MEMEORY_CONSUME__ ? __LIST_MAX_MEMEORY_CONSUME__ : list->buffer_size;
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

/*
		 █████   █████                   █████                 ██████   ██████                    
		░░███   ░░███                   ░░███                 ░░██████ ██████                     
		 ░███    ░███   ██████    █████  ░███████              ░███░█████░███   ██████   ████████ 
		 ░███████████  ░░░░░███  ███░░   ░███░░███  ██████████ ░███░░███ ░███  ░░░░░███ ░░███░░███
		 ░███░░░░░███   ███████ ░░█████  ░███ ░███ ░░░░░░░░░░  ░███ ░░░  ░███   ███████  ░███ ░███
		 ░███    ░███  ███░░███  ░░░░███ ░███ ░███             ░███      ░███  ███░░███  ░███ ░███
		 █████   █████░░████████ ██████  ████ █████            █████     █████░░████████ ░███████ 
		░░░░░   ░░░░░  ░░░░░░░░ ░░░░░░  ░░░░ ░░░░░            ░░░░░     ░░░░░  ░░░░░░░░  ░███░░░  
																																										 ░███     
																																										 █████    
																																										░░░░░     
*/

typedef struct {
	const char *key;
	void *value;
} hash_entry;

typedef struct HASH_STRUCT {
	hash_entry *buffer;
	ssize_t capacity;
	ssize_t length;
	char **keys;
} hash_T;

/*
 * Hashing function
 * 
 * It is a djb2 hashing function.
 * reference: http://www.cse.yorku.ca/~oz/hash.html
 *
 * Legal Notice:
 * I am not the godfather of this hashing function but if it works on your project, use it.
*/
unsigned long long
hash_function(const char *string)
{
	// magic number is supposed to be `33`.
	unsigned long hash = 5381;
	int c;

	while ((c = *string++) != 0)
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

hash_T *init_hash(void)
{
	hash_T *hash = malloc(sizeof(struct HASH_STRUCT));
	unwrap(hash);

	hash->length = 0;
	hash->capacity = MAX_HASH_TABLE_CAPACITY;
	hash->buffer = malloc(sizeof(hash_entry) * hash->capacity);
	unwrap(hash);

	return hash;
}

void hash_free(hash_T *hash)
{
	free(hash->keys);
	free(hash->buffer);
	free(hash);
}

void hash_set(hash_T *hash, const char *key, void *value)
{
	unsigned long long magic_number = hash_function(key);
	ssize_t index = (ssize_t)(magic_number & (MAX_HASH_TABLE_CAPACITY - 1));

	int if_found = 0;

	while (hash->buffer[index].key != NULL)
	{
		if (strcmp(key, hash->buffer[index].key) == 0)
		{
			hash->buffer[index].value = value;
			if_found = 1;
			break;
		}

		index++;
		if (index >= MAX_HASH_TABLE_CAPACITY) index = 0;
	}

	if (if_found == 0)
	{
		hash->buffer[index].key = (const char*)strdup(key);
		hash->buffer[index].value = value;
		hash->length++;

		if (hash->keys)
			hash->keys = realloc(hash->keys, sizeof(char*) * hash->length);
		else
			hash->keys = calloc(1, sizeof(char*));

		hash->keys[hash->length - 1] = (char*)strdup(key);
	}
}

void* hash_get(hash_T *hash, const char *key)
{
	unsigned long long magic_number = hash_function(key);
	ssize_t index = (ssize_t)(magic_number & (MAX_HASH_TABLE_CAPACITY - 1));

	while (hash->buffer[index].key != NULL)
	{
		if (strcmp(key, hash->buffer[index].key) == 0)
			return hash->buffer[index].value;

		index++;
		if (index >= MAX_HASH_TABLE_CAPACITY) index = 0;
	}
	
	return NULL;
}

const char **hash_bucket(hash_T *hash)
{
	return (const char**)hash->keys;
}

/*
		 █████        ███             █████                   █████
		░░███        ░░░             ░░███                   ░░███ 
		 ░███        ████  ████████   ░███ █████  ██████   ███████ 
		 ░███       ░░███ ░░███░░███  ░███░░███  ███░░███ ███░░███ 
		 ░███        ░███  ░███ ░███  ░██████░  ░███████ ░███ ░███ 
		 ░███      █ ░███  ░███ ░███  ░███░░███ ░███░░░  ░███ ░███ 
		 ███████████ █████ ████ █████ ████ █████░░██████ ░░████████
		░░░░░░░░░░░ ░░░░░ ░░░░ ░░░░░ ░░░░ ░░░░░  ░░░░░░   ░░░░░░░░ 
																															 
																															 
																															 
		 █████        ███           █████                          
		░░███        ░░░           ░░███                           
		 ░███        ████   █████  ███████                         
		 ░███       ░░███  ███░░  ░░░███░                          
		 ░███        ░███ ░░█████   ░███                           
		 ░███      █ ░███  ░░░░███  ░███ ███                       
		 ███████████ █████ ██████   ░░█████                        
		░░░░░░░░░░░ ░░░░░ ░░░░░░     ░░░░░                         
*/

// modern computer are faster to loop through smaller that this limit.
#define __MAX_ELEMENT_FOR_BSEARCH__ 100000

/*
 * Node -
 * Stores data and pointer to the next node.
*/
typedef struct NODE_STRUCT {
	struct NODE_STRUCT *next;	// pointer to next node
	void *data;	// contains user data
} node_T;

typedef struct LINKED_LIST_STRUCT {
	node_T *head; // where linked list starts
	ssize_t length; // total length of linked list
	
	// metadata for binary search
	struct searching_list {
		node_T *mid;
		node_T *end;
		ssize_t where;
	} store;
} llist_T;

llist_T *init_llist()
{
	llist_T *llist = malloc(sizeof(struct LINKED_LIST_STRUCT));
	unwrap(llist);

	llist->head = NULL;
	llist->length = 0;

	llist->store.mid = NULL;
	llist->store.end = NULL;
	llist->store.where = 0;

	return llist;
}

void llist_insert(llist_T *llist, ssize_t index, void *element)
{
}

void *llist_get(llist_T *llist, ssize_t index)
{
	if (index >= llist->length || index < 0)
		return NULL;

	node_T *p = llist->head;
	while (p->next != NULL && index > 0)
	{
		p = p->next;
		index--;
	}

	return p->data;
}

void *llist_delete(llist_T *llist, ssize_t index)
{
	return NULL;
}

void *llist_push(llist_T *llist, void *element)
{
	// if element is null, we don't need to store it.
	unwrap(element);

	// head is a special case, right?
	if (llist->head == NULL)
	{
		llist->head = malloc(sizeof(struct NODE_STRUCT));
		unwrap(llist->head);

		llist->head->next = NULL;
		llist->head->data = element;

		llist->length++;

		// we don't have many blocks to work with,
		// so mid and end both are same.

		llist->store.mid = llist->head;
		llist->store.end = llist->head;
		llist->store.where = 0;

		// returning head can mess - up things from user end,
		// but that's just user.
		// In a hope that every user who uses this library takes
		// fine pre-caution over memory and space.
		return llist->head;
	}

	// let we are not adding element to head.
	node_T *_np = llist->head;

	// loop through until we reach the Node that hasn't been allocated.
	while (_np->next != NULL)
		_np = _np->next;

	// we need alloc size for that new node.
	_np->next = malloc(sizeof(struct NODE_STRUCT));

	// make sure we unwrap it, so we know `malloc` fed-up.
	unwrap(_np->next);

	_np->next->data = element;
	_np->next->next = NULL;

	llist->length++;

	// last element is the end. 
	llist->store.end = _np->next;

	// we need middle element, we already have last middle
	// we just need to loop until we get to another middle element.
	// it should'nt really be that costly.
	while (llist->store.where < (llist->length / 2))
	{
		llist->store.mid = llist->store.mid->next;
		llist->store.where++;
	}

	// we return whatever mess user have put.
	return _np->next;
}

void *llist_search(llist_T *llist, ssize_t index)
{
}
