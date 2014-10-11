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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "rbtree.h"

// Red-black tree mechanics implemented after the public-domain algorithm
// found at: http://en.literateprograms.org/Red-black_tree_(C)

enum rbcolor { RED, BLACK };

typedef struct __rbnode_t {
	struct __rbnode_t * left;
	struct __rbnode_t * right;
	struct __rbnode_t * parent;
	enum rbcolor color;
	void * key;
	void * value;
} rbnode_t;

typedef struct __rbtree_t {
	rbnode_t * root;
	rbtree_compare compare;
	rbtree_free free;
	size_t size;
} rbtree_t;

rbnode_t * __rbtree_grandparent(rbnode_t * n)
{
	assert(n != NULL);
	assert(n->parent != NULL);
	assert(n->parent->parent != NULL);
	return(n->parent->parent);
}

rbnode_t * __rbtree_sibling(rbnode_t * n)
{
	assert(n != NULL);
	assert(n->parent != NULL);
	if (n == n->parent->left)
		return(n->parent->right);
	else
		return(n->parent->left);
}

rbnode_t * __rbtree_uncle(rbnode_t * n)
{
	assert(n != NULL);
	assert(n->parent != NULL);
	assert(n->parent->parent != NULL);
	return(__rbtree_sibling(n->parent));
}

enum rbcolor __rbnode_t_color(rbnode_t * n)
{
	return( n == NULL ? BLACK : n->color );
}

#ifdef DEBUG
void __rbtree_verify_property_1(rbnode_t * n)
{
	assert(__rbnode_t_color(n) == RED || __rbnode_t_color(n) == BLACK);
	if (n == NULL) return;
	__rbtree_verify_property_1(n->left);
	__rbtree_verify_property_1(n->right);
}

void __rbtree_verify_property_2(rbnode_t * n)
{
	assert(__rbnode_t_color(n) == BLACK);
}

void __rbtree_verify_property_4(rbnode_t * n)
{
	if (__rbnode_t_color(n) == RED) {
		assert(__rbnode_t_color(n->left) == BLACK);
		assert(__rbnode_t_color(n->right) == BLACK);
		assert(__rbnode_t_color(n->parent) == BLACK);
	}
	if (n == NULL) return;
	__rbtree_verify_property_4(n->left);
	__rbtree_verify_property_4(n->right);
}

void __rbtree_verify_property_5b(rbnode_t * n, int black_count, int * path_black_count) {
	if (__rbnode_t_color(n) == BLACK) {
		black_count++;
	}
	if (n == NULL) {
		if (*path_black_count == -1) {
			*path_black_count = black_count;
		}
		else {
			assert(black_count == *path_black_count);
		}
		return;
	}
	__rbtree_verify_property_5b(n->left, black_count, path_black_count);
	__rbtree_verify_property_5b(n->right, black_count, path_black_count);
}

void __rbtree_verify_property_5(rbnode_t * n)
{
	int black_count_path = -1;
	__rbtree_verify_property_5b(n, 0, &black_count_path);
}

void __rbtree_verify_properties(rbtree_t * t)
{
	__rbtree_verify_property_1(t->root);
	__rbtree_verify_property_2(t->root);
	// Property 3 is implicit
	__rbtree_verify_property_4(t->root);
	__rbtree_verify_property_5(t->root);
}
#else
#define __rbtree_verify_properties(t)
#endif

rbtree_t * rbtree_create(rbtree_compare compare, rbtree_free free)
{
	rbtree_t * t = NULL;

	assert(compare != NULL);
	t = (rbtree_t *)malloc(sizeof(rbtree_t));
	t->root = NULL;
	t->compare = compare;
	t->free = free;
	t->size = 0;
	__rbtree_verify_properties(t);
	return(t);
}

int rbtree_clear(rbtree_t * t)
{
	rbnode_t * n;
	rbnode_t * p;

	n = rbtree_post_order_first(t);
	while (n != NULL) {
		p = n->parent;
		if (!rbtree_remove(t, n))
			return(0);
		n = p;
		n = rbtree_post_order_next(n);
	}
	return(1);
}

rbnode_t * __rbtree_new_node(
	void * key, void * value, enum rbcolor color, rbnode_t * left,
	rbnode_t * right)
{
	rbnode_t * result = NULL;

	result = malloc(sizeof(rbnode_t));
	assert(result != NULL);
	result->key = key;
	result->value = value;
	result->color = color;
	result->left = left;
	result->right = right;
	if (left != NULL)
		left->parent = result;
	if (right != NULL)
		right->parent = result;
	result->parent = NULL;
	return(result);
}

rbnode_t * rbtree_find(rbtree_t * t, void * key)
{
	rbnode_t * n = NULL;

	n = t->root;
	while (n != NULL) {
		int comp_result = t->compare(key, n->key);
		if (comp_result == 0) {
			return(n);
		}
		else if (comp_result < 0) {
			n = n->left;
		}
		else {
			assert(comp_result > 0);
			n = n->right;
		}
	}
	return(n);
}

/*
void * rbtree_find(rbtree_t * t, void * key, rbtree_compare compare)
{
	rbnode_t * n = NULL;
	n = find(t, key, compare);
	return(n == NULL ? NULL : n->value);
}
*/

void __rbtree_replace_node(rbtree_t * t, rbnode_t * oldn, rbnode_t * newn)
{
	if (oldn->parent == NULL) {
		t->root = newn;
	}
	else {
		if (oldn == oldn->parent->left)
			oldn->parent->left = newn;
		else
			oldn->parent->right = newn;
	}
	if (newn != NULL) {
		newn->parent = oldn->parent;
	}
}

void __rbtree_rotate_left(rbtree_t * t, rbnode_t * n)
{
	rbnode_t * r = NULL;

	r = n->right;
	__rbtree_replace_node(t, n, r);
	n->right = r->left;
	if (r->left != NULL) {
		r->left->parent = n;
	}
	r->left = n;
	n->parent = r;
}

void __rbtree_rotate_right(rbtree_t * t, rbnode_t * n)
{
	rbnode_t * l = NULL;

	l = n->left;
	__rbtree_replace_node(t, n, l);
	n->left = l->right;
	if (l->right != NULL) {
		l->right->parent = n;
	}
	l->right = n;
	n->parent = l;
}

void __rbtree_insert_case1(rbtree_t * t, rbnode_t * n);
void __rbtree_insert_case2(rbtree_t * t, rbnode_t * n);
void __rbtree_insert_case3(rbtree_t * t, rbnode_t * n);
void __rbtree_insert_case4(rbtree_t * t, rbnode_t * n);
void __rbtree_insert_case5(rbtree_t * t, rbnode_t * n);

void __rbtree_insert_case1(rbtree_t * t, rbnode_t * n)
{
	if (n->parent == NULL)
		n->color = BLACK;
	else
		__rbtree_insert_case2(t, n);
}

void __rbtree_insert_case2(rbtree_t * t, rbnode_t * n)
{
	if (__rbnode_t_color(n->parent) == BLACK)
		return;
	else
		__rbtree_insert_case3(t, n);
}

void __rbtree_insert_case3(rbtree_t * t, rbnode_t * n)
{
	if (__rbnode_t_color(__rbtree_uncle(n)) == RED) {
		n->parent->color = BLACK;
		__rbtree_uncle(n)->color = BLACK;
		__rbtree_grandparent(n)->color = RED;
		__rbtree_insert_case1(t, __rbtree_grandparent(n));
	}
	else {
		__rbtree_insert_case4(t, n);
	}
}

void __rbtree_insert_case4(rbtree_t * t, rbnode_t * n)
{
	if (n == n->parent->right && n->parent == __rbtree_grandparent(n)->left) {
		__rbtree_rotate_left(t, n->parent);
		n = n->left;
	}
	else if (n == n->parent->left && n->parent == __rbtree_grandparent(n)->right) {
		__rbtree_rotate_right(t, n->parent);
		n = n->right;
	}
	__rbtree_insert_case5(t, n);
}

void __rbtree_insert_case5(rbtree_t * t, rbnode_t * n)
{
	n->parent->color = BLACK;
	__rbtree_grandparent(n)->color = RED;
	if (n == n->parent->left && n->parent == __rbtree_grandparent(n)->left) {
		__rbtree_rotate_right(t, __rbtree_grandparent(n));
	}
	else {
		assert(n == n->parent->right && n->parent == __rbtree_grandparent(n)->right);
		__rbtree_rotate_left(t, __rbtree_grandparent(n));
	}
}

/*
int OLD___rbtree_insert(
	rbtree_t * t, void * key, void * value)
{
	rbnode_t * inserted_node = NULL;

	if (t->root == NULL) {
		inserted_node = __rbtree_new_node(key, value, RED, NULL, NULL);
		if (inserted_node == NULL)
			return(0);
	
		t->root = inserted_node;
	}
	else {
		rbnode_t * n = NULL;

		n = t->root;
		while (1) {
			int comp_result = 0;
			
			comp_result = t->compare(key, n->key);
			if (comp_result == 0) {
				if (!rbtree_remove(t, n))
					return(0);
				return(rbtree_insert(t, key, value));
			}
			else if (comp_result < 0) {
				if (n->left == NULL) {
					inserted_node = __rbtree_new_node(key, value, RED, NULL, NULL);
					if (inserted_node == NULL)
						return(0);
	
					n->left = inserted_node;
					break;
				}
				else {
					n = n->left;
				}
			}
			else {
				assert(comp_result > 0);
				if (n->right == NULL) {
					inserted_node = __rbtree_new_node(key, value, RED, NULL, NULL);
					if (inserted_node == NULL)
						return(0);
	
					n->right = inserted_node;
					break;
				}
				else {
					n = n->right;
				}
			}
		}
		inserted_node->parent = n;
	}
	t->size++;
	__rbtree_insert_case1(t, inserted_node);
	__rbtree_verify_properties(t);

	return(1);
}
*/

rbnode_t * rbtree_attach_node(rbtree_t * t, rbnode_t * node)
{
	rbnode_t * n = NULL;
	int comp_result = 0;

	if (t == NULL)
		return(NULL);

	node->color = RED;
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;

	if (t->root == NULL) {
		t->root = node;
	}
	else {
		n = t->root;
		while(1) {
			comp_result = t->compare(node->key, n->key);
			if (comp_result == 0) {
				if (t->free != NULL)
					t->free(n->key, n->value);
				n->key = node->key;
				n->value = node->value;
				free(node);
				return(n);
			}
			else if (comp_result < 0) {
				if (n->left == NULL) {
					n->left = node;
					break;
				}
				else {
					n = n->left;
				}
			}
			else {
				assert(comp_result > 0);
				if (n->right == NULL) {
					n->right = node;
					break;
				}
				else {
					n = n->right;
				}
			}
		}
		node->parent = n;
	}
	t->size++;
	__rbtree_insert_case1(t, node);
	__rbtree_verify_properties(t);

	return(node);
}

int rbtree_insert(
	rbtree_t * t, void * key, void * value)
{
	rbnode_t * n = NULL;

	n = __rbtree_new_node(key, value, RED, NULL, NULL);
	if (n == NULL)
		return(0);

	if (rbtree_attach_node(t, n) == NULL)
		return(0);
	
	return(1);
}

rbnode_t * __rbtree_maximum_node(rbnode_t * n)
{
	assert(n != NULL);
	while (n->right != NULL) {
		n = n->right;
	}
	return(n);
}

void __rbtree_delete_case1(rbtree_t * t, rbnode_t * n);
void __rbtree_delete_case2(rbtree_t * t, rbnode_t * n);
void __rbtree_delete_case3(rbtree_t * t, rbnode_t * n);
void __rbtree_delete_case4(rbtree_t * t, rbnode_t * n);
void __rbtree_delete_case5(rbtree_t * t, rbnode_t * n);
void __rbtree_delete_case6(rbtree_t * t, rbnode_t * n);

void __rbtree_delete_case1(rbtree_t * t, rbnode_t * n)
{
	if (n->parent == NULL)
		return;
	else
		__rbtree_delete_case2(t, n);
}

void __rbtree_delete_case2(rbtree_t * t, rbnode_t * n)
{
	if (__rbnode_t_color(__rbtree_sibling(n)) == RED) {
		n->parent->color = RED;
		__rbtree_sibling(n)->color = BLACK;
		if (n == n->parent->left)
			__rbtree_rotate_left(t, n->parent);
		else
			__rbtree_rotate_right(t, n->parent);
	}
	__rbtree_delete_case3(t, n);
}

void __rbtree_delete_case3(rbtree_t * t, rbnode_t * n)
{
	if (
		__rbnode_t_color(n->parent) == BLACK &&
		__rbnode_t_color(__rbtree_sibling(n)) == BLACK &&
		__rbnode_t_color(__rbtree_sibling(n)->left) == BLACK &&
		__rbnode_t_color(__rbtree_sibling(n)->right) == BLACK
		)
	{
		__rbtree_sibling(n)->color = RED;
		__rbtree_delete_case1(t, n->parent);
	}
	else
		__rbtree_delete_case4(t, n);
}

void __rbtree_delete_case4(rbtree_t * t, rbnode_t * n)
{
	if (
		__rbnode_t_color(n->parent) == RED &&
		__rbnode_t_color(__rbtree_sibling(n)) == BLACK &&
		__rbnode_t_color(__rbtree_sibling(n)->left) == BLACK &&
		__rbnode_t_color(__rbtree_sibling(n)->right) == BLACK
		)
	{
		__rbtree_sibling(n)->color = RED;
		n->parent->color = BLACK;
	}
	else
		__rbtree_delete_case5(t, n);
}

void __rbtree_delete_case5(rbtree_t * t, rbnode_t * n)
{
	if (
		n == n->parent->left &&
		__rbnode_t_color(__rbtree_sibling(n)) == BLACK &&
		__rbnode_t_color(__rbtree_sibling(n)->left) == RED &&
		__rbnode_t_color(__rbtree_sibling(n)->right) == BLACK
		)
	{
		__rbtree_sibling(n)->color = RED;
		__rbtree_sibling(n)->left->color = BLACK;
		__rbtree_rotate_right(t, __rbtree_sibling(n));
	}
	else if (
		n == n->parent->right &&
		__rbnode_t_color(__rbtree_sibling(n)) == BLACK &&
		__rbnode_t_color(__rbtree_sibling(n)->right) == RED &&
		__rbnode_t_color(__rbtree_sibling(n)->left) == BLACK
		)
	{
		__rbtree_sibling(n)->color = RED;
		__rbtree_sibling(n)->right->color = BLACK;
		__rbtree_rotate_left(t, __rbtree_sibling(n));
	}
	__rbtree_delete_case6(t, n);
}

void __rbtree_delete_case6(rbtree_t * t, rbnode_t * n)
{
	__rbtree_sibling(n)->color = __rbnode_t_color(n->parent);
	n->parent->color = BLACK;
	if (n == n->parent->left) {
		assert(__rbnode_t_color(__rbtree_sibling(n)->right) == RED);
		__rbtree_sibling(n)->right->color = BLACK;
		__rbtree_rotate_left(t, n->parent);
	}
	else {
		assert(__rbnode_t_color(__rbtree_sibling(n)->left) == RED);
		__rbtree_sibling(n)->left->color = BLACK;
		__rbtree_rotate_right(t, n->parent);
	}
}

rbnode_t * rbtree_detach_node(rbtree_t * t, rbnode_t * n)
{
	rbnode_t * child = NULL;

	if (n == NULL)
		return(NULL);

	if (n->left != NULL && n->right != NULL) {
		rbnode_t * pred = NULL;

		pred = __rbtree_maximum_node(n->left);
		n->key = pred->key;
		n->value = pred->value;
		n = pred;
	}

	assert(n->left == NULL || n->right == NULL);
	child = n->right == NULL ? n->left : n->right;
	if (__rbnode_t_color(n) == BLACK) {
		n->color = __rbnode_t_color(child);
		__rbtree_delete_case1(t, n);
	}
	__rbtree_replace_node(t, n, child);
	if (n->parent == NULL && child != NULL)
		child->color = BLACK;
	t->size--;

	__rbtree_verify_properties(t);
	n->color = RED;
	n->parent = NULL;
	n->left = NULL;
	n->right = NULL;
	return(n);
}

int rbtree_remove(rbtree_t * t, rbnode_t * n)
{
	if ((n = rbtree_detach_node(t, n)) == NULL)
		return(0);
	if (t->free != NULL)
		t->free(n->key, n->value);
	free(n);
	return(1);
}

size_t rbtree_size(rbtree_t * t)
{
	return(t == NULL ? 0 : t->size);
}

rbnode_t * __rbtree_apply_function(rbnode_t * n, rbtree_func func)
{
	rbnode_t * r = NULL;

	if (n == NULL)
		return(NULL);
	if (!func(n->key, n->value))
		return(n);
	r = __rbtree_apply_function(n->left, func);
	if (r != NULL)
		return(r);
	r = __rbtree_apply_function(n->right, func);
	if (r != NULL)
		return(r);
	return(NULL);
}

void __rbtree_destroy_nodes(rbtree_t * t, rbnode_t * n)
{
	if (n == NULL)
		return;
	__rbtree_destroy_nodes(t, n->left);
	__rbtree_destroy_nodes(t, n->right);
	if (t->free != NULL)
		t->free(n->key, n->value);
	free(n);
}

rbtree_t * rbtree_destroy(rbtree_t * t)
{
	if (t == NULL)
		return(NULL);
	__rbtree_destroy_nodes(t, t->root);
	free(t);
	return(NULL);
}

int __rbtree_clone_node(rbtree_t * t, rbnode_t * n)
{
	assert(t != NULL);
	assert(n != NULL);
	if (rbtree_insert(t, rbtree_key(n), rbtree_value(n)) == 0)
		return(0);
	if (rbtree_left(n) != NULL)
		if (!__rbtree_clone_node(t, rbtree_left(n)))
			return(0);
	if (rbtree_right(n) != NULL)
		if (!__rbtree_clone_node(t, rbtree_right(n)))
			return(0);
	return(1);
}

rbtree_t * rbtree_clone(rbtree_t * t)
{
	rbtree_t * new_t = NULL;

	if (t == NULL)
		return(NULL);
	new_t = rbtree_create(t->compare, t->free);
	if (new_t == NULL)
		return(NULL);
	if (!__rbtree_clone_node(new_t, rbtree_top(t))) {
		rbtree_destroy(new_t);
		return(NULL);
	}
	return(new_t);
}

rbnode_t * rbtree_apply(rbtree_t * t, rbtree_func func)
{
	rbnode_t * n = NULL;

	if (t == NULL) return(NULL);
	n = t->root;
	return(__rbtree_apply_function(n, func));
}

int __rbtree_write_gv_node(rbnode_t * n, FILE * fout,
	int (*write_node_key)(FILE * fout, void * key),
	int (*write_node_value)(FILE * fout, void * value))
{
	if (n == NULL) return(0);
	assert(write_node_key != NULL);
	fprintf(fout, "  ");
	if (!write_node_key(fout, n->key)) return(0);
	fprintf(fout, " [ ");
	if (write_node_value != NULL) {
		fprintf(fout, " label=\"");
		if (!write_node_value(fout, n->value)) return(0);
		fprintf(fout, "\"");
	}
	fprintf(fout, " color=\"%s\"", n->color == RED ? "red" : "black");
	fprintf(fout, " ];\n");

	if (n->left != NULL) {
		fprintf(fout, "  ");
		if (!write_node_key(fout, n->key)) return(0);
		fprintf(fout, " -> ");
		if (!write_node_key(fout, n->left->key)) return(0);
		fprintf(fout, " [ label=\"l\" ];\n");
	}
	if (n->right != NULL) {
		fprintf(fout, "  ");
		if (!write_node_key(fout, n->key)) return(0);
		fprintf(fout, " -> ");
		if (!write_node_key(fout, n->right->key)) return(0);
		fprintf(fout, " [ label=\"r\" ];\n");
	}
	if (n->parent != NULL) {
		fprintf(fout, "  ");
		if (!write_node_key(fout, n->key)) return(0);
		fprintf(fout, " -> ");
		if (!write_node_key(fout, n->parent->key)) return(0);
		fprintf(fout, " [ label=\"p\" ];\n");
	}

	if (n->left != NULL) {
		if (!__rbtree_write_gv_node(n->left, fout, write_node_key, write_node_value))
			return(0);
	}
	if (n->right != NULL) {
		if (!__rbtree_write_gv_node(n->right, fout, write_node_key, write_node_value))
			return(0);
	}

	return(1);
}

int rbtree_write_gv(rbtree_t * t, char * filename,
	int (*write_node_key)(FILE * fout, void * key),
	int (*write_node_value)(FILE * fout, void * value))
{
	FILE * fout = NULL;

	fout = fopen(filename, "w");
	if (!fout) return(0);

	fprintf(fout, "// Visuallize this file with the following command:\n");
	fprintf(fout, "// dot -Tsvgz '%s' | display - &\n", filename);
	fprintf(fout, "\n");
	fprintf(fout, "digraph G {\n");
	// fprintf(fout, "  graph [ ordering=\"out\" ];\n");
	fprintf(fout, "  nodesep=0.3;\n");
	fprintf(fout, "  ranksep=0.2;\n");
	fprintf(fout, "  margin=0.1;\n");
	fprintf(fout, "  node [ style=filled, fillcolor=yellow, shape=circle ];\n");
	fprintf(fout, "  edge [ arrowsize=0.8 ];\n");
	fprintf(fout, "\n");
	if (!__rbtree_write_gv_node(t->root, fout, write_node_key, write_node_value)) {
		fclose(fout);
		return(0);
	}
	fprintf(fout, "}\n");
	fclose(fout);
	return(1);
}

void * rbtree_key(rbnode_t * n)
{
	return(n == NULL ? NULL : n->key);
}

void * rbtree_value(rbnode_t * n)
{
	return(n == NULL ? NULL : n->value);
}

rbnode_t * rbtree_top(rbtree_t * t)
{
	return(t == NULL ? NULL : t->root);
}

rbnode_t * rbtree_left(rbnode_t * n)
{
	return(n == NULL ? NULL : n->left);
}

rbnode_t * rbtree_right(rbnode_t * n)
{
	return(n == NULL ? NULL : n->right);
}

rbnode_t * rbtree_parent(rbnode_t * n)
{
	return(n == NULL ? NULL : n->parent);
}

rbnode_t * rbtree_in_order_first(rbtree_t * t)
{
	rbnode_t * n = NULL;

	if (t == NULL)
		return(NULL);
	if (t->root == NULL)
		return(NULL);
	
	n = t->root;
	while (n->left != NULL)
		n = n->left;
	return(n);
}

rbnode_t * rbtree_in_order_last(rbtree_t * t)
{
	rbnode_t * n = NULL;

	if (t == NULL)
		return(NULL);
	if (t->root == NULL)
		return(NULL);
	
	n = t->root;
	while (n->right != NULL)
		n = n->right;
	return(n);
}

rbnode_t * rbtree_in_order_next(rbnode_t * n)
{
	rbnode_t * l = NULL;

	if (n == NULL)
		return(NULL);

	if (n->right != NULL) {
		n = n->right;
		while (n->left != NULL)
			n = n->left;
		return(n);
	}

	if (n->parent != NULL && n->parent->left == n) {
		n = n->parent;
		return(n);
	}

	l = n;
	n = n->parent;
	while (n != NULL && n->right == l) {
		l = n;
		n = n->parent;
	}
	return(n);
}

rbnode_t * rbtree_in_order_prev(rbnode_t * n)
{
	rbnode_t * l = NULL;

	if (n == NULL)
		return(NULL);
	
	if (n->left != NULL) {
		n = n->left;
		while (n->right != NULL)
			n = n->right;
		return(n);
	}

	if (n->parent != NULL && n->parent->right == n) {
		n = n->parent;
		return(n);
	}

	l = n;
	n = n->parent;
	while (n != NULL && n->left == l) {
		l = n;
		n = n->parent;
	}
	return(n);
}

rbnode_t * rbtree_pre_order_first(rbtree_t * t)
{
	if (t == NULL)
		return(NULL);
	if (t->root == NULL)
		return(NULL);
	return(t->root);
}

rbnode_t * rbtree_pre_order_last(rbtree_t * t)
{
	rbnode_t * n = NULL;

	if (t == NULL)
		return(NULL);
	if (t->root == NULL)
		return(NULL);
	n = t->root;
	while (n->right != NULL)
		n = n->right;
	return(n);
}

rbnode_t * rbtree_pre_order_next(rbnode_t * n)
{
	rbnode_t * l = NULL;

	if (n == NULL)
		return(NULL);
	
	if (n->left != NULL)
		return(n->left);
	
	if (n->right != NULL)
		return(n->right);
	
	if (n->parent == NULL)
		return(NULL);
	
	l = n;
	n = n->parent;
	while (l == n->right) {
		l = n;
		n = n->parent;
		if (n == NULL)
			return(NULL);
	}
	n = n->right;
	return(n);
}

rbnode_t * rbtree_pre_order_prev(rbnode_t * n)
{
	rbnode_t * l = NULL;

	if (n == NULL)
		return(NULL);
	
	l = n;
	n = n->parent;
	if (n == NULL)
		return(NULL);
	if (l == n->right) {
		if (n->left == NULL)
			return(n);
		n = n->left;
		while (n->right != NULL)
			n = n->right;
	}
	return(n);
}


rbnode_t * rbtree_post_order_first(rbtree_t * t)
{
	rbnode_t * n = NULL;

	if (t == NULL)
		return(NULL);
	if (t->root == NULL)
		return(NULL);
	
	n = t->root;
	while (n->left != NULL)
		n = n->left;
	return(n);
}

rbnode_t * rbtree_post_order_last(rbtree_t * t)
{
	if (t == NULL)
		return(NULL);
	return(t->root);
}

rbnode_t * rbtree_post_order_next(rbnode_t * n)
{
	rbnode_t * l = NULL;

	if (n == NULL)
		return(NULL);

	l = n;
	n = n->parent;
	if (n == NULL)
		return(NULL);
	if (l == n->left) {
		if (n->right == NULL)
			return(n);
		n = n->right;
		while (n->left != NULL)
			n = n->left;
	}
	return(n);
}

rbnode_t * rbtree_post_order_prev(rbnode_t * n)
{
	rbnode_t * l = NULL;

	if (n == NULL)
		return(NULL);
	
	if (n->right != NULL)
		return(n->right);
	
	if (n->left != NULL)
		return(n->left);
	
	if (n->parent == NULL)
		return(NULL);
	
	l = n;
	n = n->parent;
	while (l == n->left) {
		l = n;
		n = n->parent;
		if (n == NULL)
			return(NULL);
	}
	n = n->left;
	return(n);
}

int rbtrees_are_identical(rbtree_t * t1, rbtree_t * t2)
{
	rbnode_t * n1 = NULL;
	rbnode_t * n2 = NULL;

	if (t1 == t2)
		return(1);
	if ((t1 == NULL) && (t2 != NULL))
		return(0);
	if ((t1 != NULL) && (t2 == NULL))
		return(0);
	if (t1->size != t2->size)
		return(0);
	if (t1->compare != t2->compare) // !!!
		return(0);
	
	n1 = rbtree_in_order_first(t1);
	n2 = rbtree_in_order_first(t2);
	while ((n1 != NULL) && (n2 != NULL)) {
		if (t1->compare(n1->value, n2->value) != 0)
			return(0);
		n1 = rbtree_in_order_next(n1);
		n2 = rbtree_in_order_next(n2);
	}
	// For this to fail, then either t1->size or t2->size must be corrupt
	assert(n1 == NULL && n2 == NULL);
	return(1);
}

rbnode_t * rbtree_level_order_first(rbtree_t * t)
{
	if (t == NULL)
		return(NULL);
	return(t->root);
}

size_t __rbnode_t_depth(rbnode_t * n)
{
	size_t depth = 0;

	while (n != NULL) {
		n = rbtree_parent(n);
		depth++;
	}
	return(depth);
}

rbnode_t * rbtree_level_order_last(rbtree_t * t)
{
	rbnode_t * n = NULL;
	rbnode_t * deepest_right = NULL;
	size_t deepest_depth = 0;
	size_t this_depth = 0;

	// This method is awful

	deepest_right = rbtree_top(t);
	deepest_depth = 0;

	n = rbtree_post_order_last(t);
	while (n != NULL) {
		this_depth = __rbnode_t_depth(n);
		if (this_depth > deepest_depth) {
			deepest_depth = this_depth;
			deepest_right = n;
		}
		n = rbtree_post_order_prev(n);
	}
	return(deepest_right);
}

rbnode_t * rbtree_level_order_next(rbnode_t * n)
{
	size_t my_depth = 0;
	rbnode_t * s = n;

	if (n == NULL)
		return(NULL);
	
	// This method is awful

	my_depth = __rbnode_t_depth(n);
	s = n;
	n = rbtree_post_order_next(n);
	while (n != NULL && __rbnode_t_depth(n) != my_depth)
		n = rbtree_post_order_next(n);
	if (n != NULL)
		return(n);
	n = s;
	my_depth++;
	while (n->parent != NULL)
		n = n->parent;
	while (n->left != NULL)
		n = n->left;
	while (n != NULL && __rbnode_t_depth(n) != my_depth) {
		n = rbtree_in_order_next(n);
	}

	return(n);
}

rbnode_t * rbtree_level_order_prev(rbnode_t * n)
{
	size_t my_depth = 0;
	rbnode_t * s = n;

	if (n == NULL)
		return(NULL);

	// This method is awful

	my_depth = __rbnode_t_depth(n);
	s = n;
	n = rbtree_post_order_prev(n);
	while (n != NULL && __rbnode_t_depth(n) != my_depth)
		n = rbtree_post_order_prev(n);
	if (n != NULL)
		return(n);
	n = s;
	my_depth--;
	while (n->parent != NULL)
		n = n->parent;
	while (n->right != NULL)
		n = n->right;
	while (n != NULL && __rbnode_t_depth(n) != my_depth) {
		n = rbtree_in_order_prev(n);
	}

	return(n);
}

