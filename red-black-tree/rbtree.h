/* A Red-Black Tree
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

#ifndef __RBTREE_H__
#define __RBTREE_H__

/* SECTION: dllist_t Red-Black Tree
 */

//---[ Data Structures ]-----------------------------------------------------

/** A data structure used for a single node in the tree.  It contains a void *
 * pointer to a key, and a void * pointer to an associated value.
 *
 * Example:
 *
 * @code
 * rbnode_t * n = rbtree_find(t, k);
 * if (n != NULL) {
 * 	printf("Found it\n");
 * 	printf("Key: %s\n", (char *)rbtree_key(n));
 * 	printf("Value: %d\n", *(int *)rbtree_value(n));
 * } else {
 * 	printf("Not found\n");
 * }
 * @endcode
 */
typedef struct __rbnode_t rbnode_t;

/** A data structure used for the tree.  It contains a pointer to the root
 * rbnode_t, a pointer to a compare function, a pointer to a free function,
 * and a size_t value containing the number of nodes in the tree.
 *
 * Example:
 *
 * @code
 * rbtree_t * t = rbtree_create(rbnode_data_cmp, NULL);
 * assert(t != NULL);
 * // ... stuff ...
 * t = rbtree_destroy(t); // t will be set to NULL on success
 * @endcode
 */
typedef struct __rbtree_t rbtree_t;

//---[ Supporting Functions ]------------------------------------------------

/** A compare function used for insersion and find.  This function should
 * compare the two keys given as const void * pointers, and return:
 * - if (left < right) return -1
 * - if (left > right) return 1
 * - otherwise return 0
 *
 * See also:
 * - rbtree_create()
 *
 * Example: The following code demonstrates how to create a function that
 * compares two integers passed as (void *) pointers.
 *
 * @code
 * int rbnode_data_cmp(const void * left, const void * right)
 * {
 * 	int l = 0;
 * 	int r = 0;
 * 
 * 	l = *(int *)left;
 * 	r = *(int *)right;
 * 	if (l < r) return(-1);
 * 	if (l > r) return(1);
 * 	return(0);
 * }
 * @endcode
 *
 * Example: To use the above compare function, pass the function name as the
 * first argument to rbtree_create().
 *
 * @code
 * rbtree_t * t = rbtree_create(rbnode_data_cmp, NULL);
 * assert(t != NULL);
 * @endcode
 */
typedef int (*rbtree_compare)(const void * left, const void * right);

/** A free function used for node removal.  This function should properly
 * destroy a key and it's associated value, if necessary.  It should return 1
 * on success, and 0 on failure.
 *
 * See also:
 * - rbtree_create()
 *
 * Example: The following code demonstrates how to create a free function that
 * will be called on all key and value pairs in the tree.
 *
 * @code
 * int free_rbnode_data(void * key, void * value) {
 * 	free(key);
 * 	free(value);
 * 	return(1);
 * }
 * @endcode
 */
typedef int (*rbtree_free)(void * key, void * value);

/** A processing function used by rbtree_apply() to apply a transformation to
 * each value of each node in the tree.
 *
 * Expected return value:
 * * Success: 1
 * * Failure: 0
 *
 * See also:
 * - rbtree_apply()
 *
 * Example: The following code demonstrates how to create an apply function
 * that will be called on all key/value pairs in a tree.
 *
 * @code
 * int apply(void * key, void * value)
 * {
 * 	// 1) Typecast the (void *) value to an (int *)
 * 	// 2) De-reference it from an (int *) to an (int)
 * 	// 3) Increment it's value by one
 * 	(*(int *)value)++;
 * 	return(1);
 * }
 * @endcode
 */
typedef int (*rbtree_func)(const void * key, void * value);

//---[ Tree Functions ]------------------------------------------------------

/** Create a new tree.  Compare is a pointer to a function used to compare void
 * keys.  Compare cannot be NULL.  Free is a pointer to a function to call
 * when destroying nodes.  Free is responsible for properly releasing any
 * memory used for the key and value stored by the node.
 *
 * Return value:
 * - Success: A rbtree * pointer
 * - Failure: NULL
 *
 * See also:
 * - rbtree_compare()
 * - rbtree_free()
 *
 * Example:
 *
 * @code
 * rbtree_t * t = rbtree_create(rbnode_data_cmp, NULL);
 * assert(t != NULL);
 * // ... stuff ...
 * t = rbtree_destroy(t); // t will be set to NULL on success
 * @endcode
 */
rbtree_t * rbtree_create(rbtree_compare compare, rbtree_free free);

/** Empty the tree by destroying all of it's nodes.
 * 
 * Notes: The key/value pairs stored in the nodes are not destroyed.
 *
 * Return value:
 * - Success: 1
 * - Failure: 0
 *
 * @code
 * if (!rbtree_clear(t)) {
 * 	printf("Failed\n");
 * }
 * @endcode
 */
int rbtree_clear(rbtree_t * t);

/** Create a new node with the given key/value pair, and insert it into the
 * tree.
 *
 * Return value:
 * - Success: 1
 * - Failure: 0
 *
 * Notes: Be aware that when inserting a duplicate key, the original node is
 * removed and replaced with the new key/value pair.
 *
 * @code
 * int * key = malloc(sizeof(int));
 * int * value = malloc(sizeof(int));
 * *key = 5;
 * *value = 7;
 * if (!rbtree_insert(t, (void *)key, (void *)value)) {
 * 	printf("Failed\n");
 * }
 * @endcode
 */
int rbtree_insert(rbtree_t * t, void * key, void * value);

/** Given a tree and a key, search the tree for a node with the given key.
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL
 *
 * Example:
 *
 * @code
 * rbnode_t * n = rbtree_find(t, (void *)&key);
 * if (n == NULL) {
 * 	printf("Not found\n");
 * }
 * else {
 * 	printf("Found: key = %d, value = %d\n", *(int *)rbtree_key(n), *(int *)rbtree_value(n));
 * }
 * @endcode
 */
rbnode_t * rbtree_find(rbtree_t * t, void * key);

/** Given a tree and a node, remove the node from the tree.
 *
 * Return value:
 * - Success: 1
 * - Failure: 0
 *
 * Example:
 *
 * @code
 * rbnode_t * n = rbtree_find(t, k);
 * if (n != NULL) {
 * 	printf("Found it\n");
 * 	printf("Key: %s\n", (char *)rbtree_key(n));
 * 	printf("Value: %d\n", *(int *)rbtree_value(n));
 * } else {
 * 	printf("Not found\n");
 * }
 * @endcode
 */
int rbtree_remove(rbtree_t * t, rbnode_t * n);

/** Return the number of nodes in the tree.
 *
 * Example:
 *
 * @code
 * size_t num_elements = rbtree_size(t);
 * @endcode
 */
size_t rbtree_size(rbtree_t * t);

/** Apply a transformation to each value of each node in the tree
 *
 * Return value:
 * - Success: NULL
 * - Failure: rbnode_t * pointer to the node on which func() failed
 *
 * See also:
 * - rbtree_func()
 *
 * Example: The following code demonstrates how to create and use an apply
 * function that will be called on all key/value pairs in a tree.
 *
 * @code
 * rbnode_t * n = NULL;
 * n = rbtree_apply(t, apply);
 * // On failure, n will point to the node on which the apply function failed
 * // On success, n will be NULL
 * assert(n == NULL);
 * @endcode
 */
rbnode_t * rbtree_apply(rbtree_t * t, rbtree_func func);

/** Destroy a tree and free all nodes, keys, and values
 *
 * Return value: NULL
 *
 * Example:
 *
 * @code
 * rbtree_t * t = rbtree_create(cmp, NULL);
 * // ... stuff ...
 * t = rbtree_destroy(t);
 * @endcode
 */
rbtree_t * rbtree_destroy(rbtree_t * t);

/** Make a copy of a tree
 *
 * Return value:
 * - Success: rbtree_t * pointer to the cloned tree
 * - Failure: NULL
 *
 * Notes: Be aware that cloning a tree merely copies the key and value
 * pointers.  It does not create copies of what they key and value pointers
 * point to.
 *
 * Example:
 *
 * @code
 * rbtree_t * t2 = rbtree_clone(t);
 * @endcode
 */
rbtree_t * rbtree_clone(rbtree_t * t);

/** Determine if two trees are identical
 *
 * Return value:
 * - Success: 1 -- Trees are identical
 * - Failure: 0 -- Trees differ
 *
 * Example:
 *
 * @code
 * if (rbtrees_are_identical(t1, t2)) {
 * 	printf("Identical\n");
 * }
 * else {
 * 	printf("Differ\n");
 * }
 * @endcode
 */
int rbtrees_are_identical(rbtree_t * t1, rbtree_t * t2);

/** Write a tree as a graphviz dot file
 *
 * Return value:
 * - Success: 1
 * - Failure: 0
 *
 * TODO: Write an example for this
 */
int rbtree_write_gv(rbtree_t * t, char * filename,
	int (*write_node_key)(FILE * fout, void * key),
	int (*write_node_value)(FILE * fout, void * value));

/** Return the key from a node
 *
 * TODO: Write an example for this
 */
void * rbtree_key(rbnode_t * n);

/** Return the value from a node
 *
 * TODO: Write an example for this
 */
void * rbtree_value(rbnode_t * n);

/** Detach the given node from the tree
 *
 * Return value:
 * - Success: rbnode_t * pointer to the new detached node
 * - Failure: NULL
 *
 * Notes: Be aware that the rbnode_t * pointer passed back may have a
 * different address than the pointer passed in.  Therefore proper usage
 * should be:
 * 
 * @code
 * n = rbtree_detach_node(t, n);
 * if (n == NULL) {
 * 	printf("Detach failed\n");
 * }
 * @endcode
 */
rbnode_t * rbtree_detach_node(rbtree_t * t, rbnode_t * n);

/** Attach a node to the tree
 *
 * Return value:
 * - Success: rbtree_t * pointer to the newly attached node
 * - Failure: NULL
 *
 * Notes: Be aware that the rbnode_t * pointer passed back may have a
 * different address than the pointer passed in.  Therefore proper usage
 * should be:
 *
 * @code
 * n = rbtree_attach_node(t, n);
 * if (n == NULL) {
 * 	printf("Attach failed\n");
 * }
 * @endcode
 *
 * Notes: Be aware that when inserting a node with a duplicate key, the
 * original node's key/value pair are removed and replaced with the new
 * key/value pair.
 */
rbnode_t * rbtree_attach_node(rbtree_t * t, rbnode_t * n);

//---[ Traversal Functions: By Node ]----------------------------------------

/** Return the root node of a tree
 * 
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- The tree is empty
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_top(rbtree_t * t);

/** Return the left child of a node
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- The node has no left child
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_left(rbnode_t * n);

/** Return the right child of a node
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- The node has no right child
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_right(rbnode_t * n);

/** Return the parent node of a node
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- The node has no parent
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_parent(rbnode_t * n);

//---[ Traversal Functions: In-Order ]---------------------------------------

/** Given a tree, return the first node for in-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- The tree is empty
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_in_order_first(rbtree_t * t);

/** Given a tree, return the first node for reverse in-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- The tree is empty
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_in_order_last(rbtree_t * t);

/** Given a node, return the next node via in-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- There is no next node
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_in_order_next(rbnode_t * n);

/** Given a node, return the previous node via reverse in-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- There is no previous node
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_in_order_prev(rbnode_t * n);

//---[ Traversal Functions: Pre-Order ]--------------------------------------

/** Given a tree, return the first node for pre-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- The tree is empty
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_pre_order_first(rbtree_t * t);

/** Given a tree, return the first node for reverse pre-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- The tree is empty
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_pre_order_last(rbtree_t * t);

/** Given a node, return the next node via pre-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- There is no next node
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_pre_order_next(rbnode_t * n);

/** Given a node, return the previous node via reverse in-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- There is no previous node
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_pre_order_prev(rbnode_t * n);

//---[ Traversal Functions: Post-Order ]-------------------------------------

/** Given a tree, return the first node for post-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- The tree is empty
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_post_order_first(rbtree_t * t);

/** Given a tree, return the first node for reverse post-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- The tree is empty
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_post_order_last(rbtree_t * t);

/** Given a node, return the next node via post-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- There is no next node
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_post_order_next(rbnode_t * n);

/** Given a node, return the previous node via reverse post-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- There is no previous node
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_post_order_prev(rbnode_t * n);

//---[ Traversal Functions: Level-Order ]------------------------------------

/** Given a tree, return the first node for level-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- The tree is empty
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_level_order_first(rbtree_t * t);

/** Given a tree, return the first node for reverse level-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- The tree is empty
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_level_order_last(rbtree_t * t);

/** Given a node, return the next node via level-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- There is no next node
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_level_order_next(rbnode_t * n);

/** Given a node, return the previous node via reverse level-order traversal
 *
 * Return value:
 * - Success: rbnode_t *
 * - Failure: NULL -- There is no previous node
 *
 * TODO: Write an example for this
 */
rbnode_t * rbtree_level_order_prev(rbnode_t * n);

#endif

