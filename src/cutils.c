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
	const char* key;
	void* value;
} hash_entry;

typedef struct HASH_STRUCT {
	hash_entry* buffer;
	ssize_t capacity;
	ssize_t length;
	char** keys;
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
hash_function(const char* string)
{
	// magic number is supposed to be `33`.
	unsigned long hash = 5381;
	int c;

	while ((c = *string++) != 0)
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

hash_T* init_hash(void)
{
	hash_T* hash = malloc(sizeof(struct HASH_STRUCT));
	if (hash == NULL) return NULL;

	hash->length = 0;
	hash->capacity = MAX_HASH_TABLE_CAPACITY;
	hash->buffer = malloc(sizeof(hash_entry) * hash->capacity);
	if (hash->buffer == NULL)
	{
		free(hash);
		return NULL;
	}

	return hash;
}

void hash_free(hash_T* hash)
{
	free(hash->keys);
	free(hash->buffer);
	free(hash);
}

void hash_set(hash_T* hash, const char* key, void* value)
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

void* hash_get(hash_T* hash, const char* key)
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

const char** hash_bucket(hash_T* hash)
{
	return (const char**)hash->keys;
}
