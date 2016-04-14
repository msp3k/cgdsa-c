/* A Tree
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

#ifndef __TREE_H__
#define __TREE_H__

/* SECTION: tree_t Tree
 */

//---[ Data Structures ]-----------------------------------------------------

/** A data structure used for a single node in the tree.  It contains a void *
 * pointer to a key, and a void * pointer to an associated value.
 */
typedef struct __trnode_t trnode_t;

/** A data structure used for the tree.  It contains a pointer to the root
 * trnode_t, a pointer to a compare function, a pointer to a free function,
 * and a size_t value containing the number of nodes in the tree.
 */
typedef struct __tree_t tree_t;

//---[ Supporting Functions ]------------------------------------------------

/** A compare function used for insersion and find.  This function should
 * compare the two keys given as const void * pointers, and return:
 * if (left < right) return -1
 * if (left > right) return 1
 * otherwise return 0
 */
typedef int (*tree_compare)(const void * left, const void * right);

/** A free function used for node removal.  This function should properly
 * destroy a key and it's associated value, if necessary.  It should return 1
 * on success, and 0 on failure.
 */
typedef int (*tree_free)(void * key, void * value);

/** A processing function used by tree_apply() to apply a transformation to
 * each value of each node in the tree.
 */
typedef int (*tree_func)(const void * key, void * value);

//---[ Tree Functions ]------------------------------------------------------

/** Create a new tree.  Compare is a pointer to a function used to compare void
 * keys.  Compare cannot be NULL.  Free is a pointer to a function to call
 * when destroying nodes.  Free is responsible for properly releasing any
 * memory used for the key and value stored by the node.
 *
 * Return value:
 * - Success: A tree * pointer
 * - Failure: NULL
 */
tree_t * tree_create(tree_compare compare, tree_free free);

/** Empty the tree by destroying all of it's nodes.
 *
 * Return value:
 * - Success: 1
 * - Failure: 0
 */
int tree_clear(tree_t * t);

/** Create a new node with the given key/value pair, and insert it into the
 * tree.
 *
 * Return value:
 * - Success: 1
 * - Failure: 0
 *
 * Notes: Be aware that when inserting a duplicate key, the original node is
 * removed and replaced with the new key/value pair.
 */
int tree_insert(tree_t * t, void * key, void * value);

/** Given a tree and a key, search the tree for a node with the given key.
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL
 */
trnode_t * tree_find(tree_t * t, void * key);

/** Given a tree and a node, remove the node from the tree.
 *
 * Return value:
 * - Success: 1
 * - Failure: 0
 */
int tree_remove(tree_t * t, trnode_t * n);

/** Return the number of nodes in the tree.
 */
size_t tree_size(tree_t * t);

/** Apply a transformation to each value of each node in the tree
 *
 * Return value:
 * - Success: NULL
 * - Failure: trnode_t * pointer to the node on which func() failed
 */
trnode_t * tree_apply(tree_t * t, tree_func func);

/** Destroy a tree and free all nodes, keys, and values
 *
 * Return value: NULL
 */
tree_t * tree_destroy(tree_t * t);

/** Make a copy of a tree
 *
 * Return value:
 * - Success: tree_t * pointer to the cloned tree
 * - Failure: NULL
 *
 * Notes: Be aware that cloning a tree merely copies the key and value
 * pointers.  It does not create copies of what they key and value pointers
 * point to.
 */
tree_t * tree_clone(tree_t * t);

/** Determine if two trees are identical
 *
 * Return value:
 * - Success: 1 -- Trees are identical
 * - Failure: 0 -- Trees differ
 */
int trees_are_identical(tree_t * t1, tree_t * t2);

/** Write a tree as a graphviz dot file
 *
 * Return value:
 * - Success: 1
 * - Failure: 0
 */
int tree_write_gv(tree_t * t, char * filename,
	int (*write_node_key)(FILE * fout, void * key),
	int (*write_node_value)(FILE * fout, void * value));

/** Return the key from a node
 */
void * tree_key(trnode_t * n);

/** Return the value from a node
 */
void * tree_value(trnode_t * n);

/** Detach the given node from the tree
 *
 * Return value:
 * - Success: trnode_t * pointer to the new detached node
 * - Failure: NULL
 *
 * Notes: Be aware that the trnode_t * pointer passed back may have a
 * different address than the pointer passed in.  Therefore proper usage
 * should be:
 * 
 *  n = tree_detach_node(t, n);
 *  if (n == NULL) { // Detach failed }
 */
trnode_t * tree_detach_node(tree_t * t, trnode_t * n);

/** Attach a node to the tree
 *
 * Return value:
 * - Success: tree_t * pointer to the newly attached node
 * - Failure: NULL
 *
 * Notes: Be aware that the trnode_t * pointer passed back may have a
 * different address than the pointer passed in.  Therefore proper usage
 * should be:
 *
 *  n = tree_attach_node(t, n);
 *  if (n == NULL) { // Attach failed }
 *
 * Notes: Be aware that when inserting a node with a duplicate key, the
 * original node's key/value pair are removed and replaced with the new
 * key/value pair.
 */
trnode_t * tree_attach_node(tree_t * t, trnode_t * n);

//---[ Traversal Functions: By Node ]----------------------------------------

/** Return the root node of a tree
 * 
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- The tree is empty
 */
trnode_t * tree_top(tree_t * t);

/** Return the left child of a node
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- The node has no left child
 */
trnode_t * tree_left(trnode_t * n);

/** Return the right child of a node
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- The node has no right child
 */
trnode_t * tree_right(trnode_t * n);

/** Return the parent node of a node
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- The node has no parent
 */
trnode_t * tree_parent(trnode_t * n);

//---[ Traversal Functions: In-Order ]---------------------------------------

/** Given a tree, return the first node for in-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- The tree is empty
 */
trnode_t * tree_in_order_first(tree_t * t);

/** Given a tree, return the first node for reverse in-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- The tree is empty
 */
trnode_t * tree_in_order_last(tree_t * t);

/** Given a node, return the next node via in-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- There is no next node
 */
trnode_t * tree_in_order_next(trnode_t * n);

/** Given a node, return the previous node via reverse in-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- There is no previous node
 */
trnode_t * tree_in_order_prev(trnode_t * n);

//---[ Traversal Functions: Pre-Order ]--------------------------------------

/** Given a tree, return the first node for pre-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- The tree is empty
 */
trnode_t * tree_pre_order_first(tree_t * t);

/** Given a tree, return the first node for reverse pre-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- The tree is empty
 */
trnode_t * tree_pre_order_last(tree_t * t);

/** Given a node, return the next node via pre-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- There is no next node
 */
trnode_t * tree_pre_order_next(trnode_t * n);

/** Given a node, return the previous node via reverse in-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- There is no previous node
 */
trnode_t * tree_pre_order_prev(trnode_t * n);

//---[ Traversal Functions: Post-Order ]-------------------------------------

/** Given a tree, return the first node for post-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- The tree is empty
 */
trnode_t * tree_post_order_first(tree_t * t);

/** Given a tree, return the first node for reverse post-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- The tree is empty
 */
trnode_t * tree_post_order_last(tree_t * t);

/** Given a node, return the next node via post-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- There is no next node
 */
trnode_t * tree_post_order_next(trnode_t * n);

/** Given a node, return the previous node via reverse post-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- There is no previous node
 */
trnode_t * tree_post_order_prev(trnode_t * n);

//---[ Traversal Functions: Level-Order ]------------------------------------

/** Given a tree, return the first node for level-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- The tree is empty
 */
trnode_t * tree_level_order_first(tree_t * t);

/** Given a tree, return the first node for reverse level-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- The tree is empty
 */
trnode_t * tree_level_order_last(tree_t * t);

/** Given a node, return the next node via level-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- There is no next node
 */
trnode_t * tree_level_order_next(trnode_t * n);

/** Given a node, return the previous node via reverse level-order traversal
 *
 * Return value:
 * - Success: trnode_t *
 * - Failure: NULL -- There is no previous node
 */
trnode_t * tree_level_order_prev(trnode_t * n);

/** Given a node, rotate left about that node so that the given node becomes
 * the left parent of what was once it's parent
 *
 * Return value:
 * - Success: 1
 * - Failure: 0
 */
int tree_rotate_left(trnode_t * n);

/** Given a node, rotate right about that node so that the given node becomes
 * the right parent of what was once it's parent
 *
 * Return value:
 * - Success: 1
 * - Failure: 0
 */
int tree_rotate_right(trnode_t * n);

/** Return the maximum depth of the tree
 *
 * Return value:
 * - Success: >= 0
 * - Failure: 0
 */
size_t tree_max_depth(trnode_t * n);

/** Return the minimum depth of the tree
 *
 * Return value:
 * - Success: >= 0
 * - Failure: 0
 */
size_t tree_min_depth(trnode_t * n);

/** Determine if the tree is full, meaning that all nodes have either two
 * children, or no children
 *
 * Return value:
 * - Success: 1 -- tree is full
 * - Failure: 0 -- tree is not full
 */
int tree_is_full(tree_t * t);

/** Determine if the tree is perfect, meaning that the tree is full and has a
 * full bottom level
 *
 * Return value:
 * - Success: 1 -- tree is perfect
 * - Failure: 0 -- tree is not perfect
 */
int tree_is_perfect(tree_t * t);

/** Determine if the tree is complete, meaning that the tree is full down to
 * the bottom level, and all the nodes on the bottom level are as far left as
 * possible
 *
 * Return value:
 * - Success: 1 -- tree is complete
 * - Failure: 0 -- tree is not complete
 */
int tree_is_complete(tree_t * t);

/** Determine if the tree is balanced, meaning that the depth of the left and
 * right subtrees under the top node differ by at most 1
 *
 * Return value:
 * - Success: 1 -- tree is balanced
 * - Failure: 0 -- tree is not balanced
 */
int tree_is_balanced(tree_t * t);

/** Determine if the tree is degenerate, meaning that each parent node has
 * only one child node
 *
 * Return value:
 * - Success: 1 -- tree is degenerate
 * - Failure: 0 -- tree is not degenerate
 */
int tree_is_degenerate(tree_t * t);

#endif

