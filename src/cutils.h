#ifndef __CUTILS_H__
#define __CUTILS_H__

/*
 * cutils -
 *
 * List of functions for writing better `C` code,
 * with pre-existing things.
 *
 * That are faster, efficient and takes less memory.
 * Some of them might be true for some of the features implemented in this.
 *
 * 1. Dynamic allocator
 * 2. Fixed Size allocator
 * 3. Arena allocator
 * 4. Linked List
 * 5. BTree
 * 6. B+Tree
 * 7. Binary Search
 * 8. K-way
 * 9. Hash-map
 * 10. Iterator
 * 11. Dealocator
 *
 * I may or may not use `internet` for illegal stuff,
 * but if you wanna be legal.
 * Make sure you copy this code under MIT.
 *
 * @copyright - chrybrk, 2024
*/

#include <stdio.h>
#include <stdlib.h>

/*
 * Allocator and Dealocator -
 *
 * Allocator will keep track of the malloc'd stuff.
 * Dealocator will free those malloc'd stuff.
 *
 * Pretty Simple Stuff.
*/
void *__alloc_function__(ssize_t nbytes);
void dealloc();
#define alloc(T, size) (T)__alloc_function__(size)

/*
 * Unwrap -
 * It will unwrap pointer, and check for NULL value.
*/
void *__unwrap_log__(void*, char*, int, char*);
#define unwrap(value)	\
	__unwrap_log__(value, (char*)__FILE__, __LINE__, (char*)__FUNCTION__)

/*
 * List -
 *
 * It will store everything inside a buffer,
 * which is a basic void** chained pointer buffer.
 *
 * We can store n-data until it fills itself and extend.
 *
*/
typedef struct LIST_STRUCT list_T;

/*
 * init_list - takes size of element returns new list;
 *
 * init will ensure that buffer is already allocated with n-size.
*/
list_T *init_list(ssize_t);

/*
 * list_push - takes list and value;
 *
 * It will try to push until the buffer gets filled,
 * then it will try to extend it with power of 2 of existing size.
 *
 * It will unwrap itself if fails to do so.
*/
void list_push(list_T*, void*);

/*
 * list_pop - takes list and index, returns element;
 *
 * it might return null as well, so be sure to unwrap it.
*/
void *list_pop(list_T*, ssize_t);

/*
 * list_get - takes list and index, returns element;
 *
 * it is like pop, but it would not align itself.
*/
void *list_get(list_T*, ssize_t);

/*
 * list_extend - takes two list, merge listB -> listA;
*/
void list_extend(list_T*, list_T*);

/*
 * list_free - takes list;
 *
 * It will free everything from the list.
*/
void list_free(list_T*);

#endif
