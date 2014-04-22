/* A Double-Linked List
 * by: Michael Peek
 * Date: Sun, 13 Apr 2014 20:23:11 -0400
 *
 * License:
 *
 * Copyright (c) 2014, Michael Peek <peek3000@gmail.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 		* Redistributions of source code must retain the above copyright
 * 			notice, this list of conditions and the following disclaimer.
 * 		* Redistributions in binary form must reproduce the above copyright
 * 			notice, this list of conditions and the following disclaimer in the
 * 			documentation and/or other materials provided with the distribution.
 * 		* The name of the author may not be used to endorse or promote products
 * 		  derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */

#ifndef __LIST_H__
#define __LIST_H__

/** Double-Linked Lists
 *
 * \mainpagesection dllist_t Double-Linked List
 * https://github.com/msp3k/cgdsa-c/tree/master
 *
 */

//---[ Data Structures ]-----------------------------------------------------

/** A pointer to a structure used for a single node in the list.  It contains
 * a void * pointer to an associated value.
 */
typedef struct __dllist_t dllist_t;

/* A compare function used for insersion, sort, and find.  This function should
 * compare the two values given as const void * pointers, and return:
 * - if (left < right) return -1
 * - if (left > right) return 1
 * - otherwise return 0
 */
typedef int (*dllist_compare)(const void * a1, const void * a2);

/** A free function used for node removal.  This function should properly
 * destroy a value, if necessary.  It should return 1 on success, and 0 on
 * failure.
 */
typedef int (*dllist_free)(void * a1);

/** A function used to apply a transformation to each value associated with
 * each node.
 */
typedef int (*dllist_func)(void * arg);

/** A convenience function for the developer.  This function applies free() to
 * the value of a node.
 */
int dllist_free_node(void * arg);

/** Create a new list.  Compare is a pointer to a function used to compare
 * void * values.  Compare may be NULL.  Free is a pointer to a function to
 * call when destroying nodes.  Free is responsible for properly releasing any
 * memory used for the value stored by the node.  Free may be NULL.
 */
dllist_t * dllist_create(dllist_compare compare, dllist_free free);

/** Empty a list by destroying all of it's nodes.
 *
 * Return value:
 * - Success: 1
 * - Failure: 0
 */
int dllist_clear(dllist_t * list);

/** Apply a function to every node in the list.
 *
 * Returns:
 * - Success: NULL
 * - Failure: dllist_t * pointer to the node for which dllist_func() failed
 */
dllist_t * dllist_apply(dllist_t * list, dllist_func arg);

/** Destroy a list.
 *
 * Returns: NULL
 */
dllist_t * dllist_destroy(dllist_t * list);

/** Given a node from some list, return the sentinel (header node)
 *
 * Returns:
 * - Success: dllist_t * pointer to the list's sentinel
 * - Failure: NULL
 */
dllist_t * dllist_sentinel(dllist_t * node);

/** Create a new node containing value and insert it before node
 *
 * Returns:
 * - Success: dllist_t * pointer to the newly created node
 * - Failure: NULL
 */
dllist_t * dllist_insert_before(dllist_t * node, void * value);

/** Create a new node containing value and insert it after node
 *
 * Returns:
 * - Success: dllist_t * pointer to the newly created node
 * - Failure: NULL
 */
dllist_t * dllist_insert_after(dllist_t * node, void * value);

/** Create a new node containing value and insert it into the list.  If
 * the list was created with a non-NULL value for a compare function, then
 * dllist_insert() acts like insertion sort, inserting the new node into it's
 * correct position in the list.
 *
 * Returns:
 * - Success: dllist_t * pointer to the newly created node
 * - Failure: NULL
 */
dllist_t * dllist_insert(dllist_t * list, void * value);

/** Create a new node containing value and insert it at the beginning of the
 * list
 *
 * Returns:
 * - Success: dllist_t * pointer to the newly created node
 * - Failure: NULL
 */
dllist_t * dllist_push_front(dllist_t * list, void * value);

/** Create a new node containing value and insert it at the end of the list
 *
 * Returns:
 * - Success: dllist_t * pointer to the newly created node
 * - Failure: NULL
 */
dllist_t * dllist_push_back(dllist_t * list, void * value);

/** Make a copy of a list
 *
 * Return value:
 * - Success: dllist_t * pointer to a new list
 * - Failure: NULL
 *
 * Notes: Be aware that cloning a list merely copies the value pointers.  It
 * does not create copies of what the value pointers point to.
 */
dllist_t * dllist_clone(dllist_t * list);

/** Search for the value in the list.
 * 
 * Returns:
 * - Success: dllist_t * pointer to the found node
 * - Failure: NULL
 *
 * Notes: If the list was created with a NULL compare function pointer, then
 * only the addresses of the value pointer is compares with the value pointer
 * of the nodes.  If the list has a non-NULL compare function pointer, then
 * the compare function is used.
 *
 */
dllist_t * dllist_find(dllist_t * list, void * value);

/** Return the value from a node
 *
 * Returns: void *
 */
void * dllist_value(dllist_t * node);

/** Remove a node from a list
 *
 * Returns:
 * - Success: 1
 * - Failure: 0
 */
int dllist_remove(dllist_t * node);

/** Return the first node in a list
 *
 * Returns:
 * - Success: dllist_t * pointer to the first node
 * - Failure: NULL -- the list is empty
 */
dllist_t * dllist_first(dllist_t * list);

/** Return the last node in the list
 *
 * Returns:
 * - Success: dllist_t * pointer to the last node
 * - Failure: NULL -- the list is empty
 */
dllist_t * dllist_last(dllist_t * list);

/** Return the next node in the list
 *
 * Returns:
 * - Success: dllist_t * pointer to the next node
 * - Failure: NULL -- you have reached the end of the list
 */
dllist_t * dllist_next(dllist_t * node);

/** Return the previous node in the list
 *
 * Returns:
 * - Success: dllist_t * pointer to the previous node
 * - Failure: NULL -- you have reached the beginning of the list
 */
dllist_t * dllist_prev(dllist_t * node);

/** Discern if two lists are identical
 *
 * Returns:
 * - Success: 1 -- the lists are identical
 * - Failure: 0 -- the lists differ
 *
 * Notes:
 * - If the lists were created with NULL compare function pointers, then
 *   only the addresses of the nodes values are compared.  Otherwise, the
 *   compare function will be used.
 * - The comparison algorithm
 *   - If both lists are NULL, then return 1
 *   - If one list is NULL and the other non-NULL, then return 0
 *   - If the two lists do not have the same compare function pointer, return 0
 *   - If the nth node from list1 != the nth node from list2, return 0
 *   - If the lists are not the same size, return 0
 *   - Otherwise, return 1
 */
int dllists_are_identical(dllist_t * list1, dllist_t * list2);

/** Return the number of nodes in the list
 *
 * Returns:
 * - Success: size_t number of nodes in the list (a 0 or greater value)
 * - Failure: 0 -- if the list pointer is NULL
 */
size_t dllist_size(dllist_t * list);

/** Sort the list using a quicksort algorithm
 *
 * Returns:
 * - Success: 1
 * - Failure: 0
 */
int dllist_sort(dllist_t * list);

/** Remove a node from the list
 *
 * Returns:
 * - Success: 1
 * - Failure: 0
 */
int dllist_detach_node(dllist_t * list, dllist_t * node);

/** Attach a node to the end of a list
 *
 * Returns:
 * - Success: 1
 * - Failure: 0
 */
int dllist_attach_node(dllist_t * list, dllist_t * node);

#endif
