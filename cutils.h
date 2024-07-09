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
 *
 * I may or may not use `internet` for illegal stuff,
 * but if you wanna be legal.
 * Make sure you copy this code under MIT.
 *
 * @copyright - chrybrk, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
			 █████████                               
			███░░░░░███                              
		 ███     ░░░   ██████  ████████   ██████   
		░███          ███░░███░░███░░███ ███░░███  
		░███         ░███ ░███ ░███ ░░░ ░███████   
		░░███     ███░███ ░███ ░███     ░███░░░    
		 ░░█████████ ░░██████  █████    ░░██████   
			░░░░░░░░░   ░░░░░░  ░░░░░      ░░░░░░    
																							 
																							 
																							 
		 █████  █████  █████     ███  ████         
		░░███  ░░███  ░░███     ░░░  ░░███         
		 ░███   ░███  ███████   ████  ░███   █████ 
		 ░███   ░███ ░░░███░   ░░███  ░███  ███░░  
		 ░███   ░███   ░███     ░███  ░███ ░░█████ 
		 ░███   ░███   ░███ ███ ░███  ░███  ░░░░███
		 ░░████████    ░░█████  █████ █████ ██████ 
			░░░░░░░░      ░░░░░  ░░░░░ ░░░░░ ░░░░░░  
*/                                                                                               


/*
 * Unwrap -
 * It will unwrap pointer, and check for NULL value.
*/
void *__unwrap_log__(void*, char*, int, char*);
#define unwrap(value)	\
	__unwrap_log__(value, (char*)__FILE__, __LINE__, (char*)__FUNCTION__)


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

/*
 * List -
 *
 * It will store everything inside a buffer,
 * which is a basic void** chained pointer buffer.
 *
 * We can store n-data until it fills itself and extend.
 *
*/

#define __LIST_MAX_MEMEORY_CONSUME__ 16000000 // 16MB
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

/*
 * This hash library is inspired (copied) from this reference.
 * https://benhoyt.com/writings/hash-table-in-c/
 *
 * Legal Notice:
 * You may use however you want to, including the right godfather.
 *
 * It also uses `djb2` hashing function. 
 * reference: http://www.cse.yorku.ca/~oz/hash.html
*/

/*
 * hash struct consist of capacity, length and buffer.
 * it is a pretty simple and straight forward hash library.
*/
#define MAX_HASH_TABLE_CAPACITY 1024
typedef struct HASH_STRUCT hash_T;

/*
 * init hash table with default capacity,
 * it still needs improvement for allocating more than default capacity.
 *
 * TODO: More capacity.
*/
hash_T *init_hash(void);

/*
 * it will free the buffer first and then free the hash itself.
 * It may need to free all the keys, as in `benhoyt` implementation.
 *
 * But, for my use case i think it is fine to leave it as it is.
*/
void hash_free(hash_T*);

/*
 * It will return list of keys,
 * that are present int the hashmap.
*/
const char **hash_bucket(hash_T*);

/*
 * It will set the value if it founds the key in entry,
 * else it will create new entry.
*/
void hash_set(hash_T*, const char*, void*);

/*
 * it will loop through from hash_index till capacity.
 * if it found non-null value, it will return.
 * else null will be returned
*/
void *hash_get(hash_T*, const char*);


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

/*
 * Linked list -
 *
 * It consist of nodes and pointer to next node, with the data.
 * Linked list will have a structure which will also include few metadata.
 *
 * Linear search on linked list is our only option,
 * we can surely improve linear search by front-move linear search.
 *
 * A front-move linear search is moving the found element in O(n) and,
 * moving the element to the front.
*/
typedef struct NODE_STRUCT node_T;
typedef struct LINKED_LIST_STRUCT llist_T;

/*
 * init_linked_list -
 * create linked list and return pointer of it.
*/
llist_T *init_llist();

/*
 * linked_list_insert -
 * takes llist_T* list, ssize_t index, void* element
 * add element to the linked list at a index.
*/
void llist_insert(llist_T *, ssize_t, void *);

/*
 * linked_list_get -
 * takes llist_T* list, ssize_t index
 * returns element from the linked_list.
 *
 * It uses linear search so it will be O(n) time, O(1) space
*/
void *llist_get(llist_T *, ssize_t);

/*
 * linked_list_delete -
 * takes llist_T* list, ssize_t index
 * deletes element from the list if it exist.
 * returns deleted element or NULL.
*/
void *llist_delete(llist_T *, ssize_t);

/*
 * linked_list_push -
 * takes llist* list, void* element
 * add element to the linked list at end.
*/
void *llist_push(llist_T *, void *);

/*
 * linked_list_search - 
 * takes llist* list, ssize_t index 
 * returns element if found in list else returns NULL
 *
 * It uses binary search which reduces time complexity O(log n);
*/
void *llist_search(llist_T *, ssize_t);

#endif
