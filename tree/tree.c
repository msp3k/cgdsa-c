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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree.h"

typedef struct __trnode_t {
	struct __trnode_t * left;
	struct __trnode_t * right;
	struct __trnode_t * parent;
	void * key;
	void * value;
} trnode_t;

typedef struct __tree_t {
	trnode_t * root;
	tree_compare compare;
	tree_free free;
	size_t size;
} tree_t;

int __tree_is_leaf(trnode_t * n)
{
	if (n->left == NULL && n->right == NULL)
		return(1);
	return(0);
}

trnode_t * __tree_grandparent(trnode_t * n)
{
	assert(n != NULL);
	assert(n->parent != NULL);
	assert(n->parent->parent != NULL);
	return(n->parent->parent);
}

trnode_t * __tree_sibling(trnode_t * n)
{
	assert(n != NULL);
	assert(n->parent != NULL);
	if (n == n->parent->left)
		return(n->parent->right);
	else
		return(n->parent->left);
}

trnode_t * __tree_uncle(trnode_t * n)
{
	assert(n != NULL);
	assert(n->parent != NULL);
	assert(n->parent->parent != NULL);
	return(__tree_sibling(n->parent));
}

tree_t * tree_create(tree_compare compare, tree_free free)
{
	tree_t * t = NULL;

	assert(compare != NULL);
	t = (tree_t *)malloc(sizeof(tree_t));
	t->root = NULL;
	t->compare = compare;
	t->free = free;
	t->size = 0;
	return(t);
}

int tree_clear(tree_t * t)
{
	trnode_t * n;
	trnode_t * p;

	n = tree_post_order_first(t);
	while (n != NULL) {
		p = n->parent;
		if (!tree_remove(t, n))
			return(0);
		n = p;
		n = tree_post_order_next(n);
	}
	return(1);
}

trnode_t * __tree_new_node(
	void * key, void * value, trnode_t * left,
	trnode_t * right)
{
	trnode_t * result = NULL;

	result = malloc(sizeof(trnode_t));
	assert(result != NULL);
	result->key = key;
	result->value = value;
	result->left = left;
	result->right = right;
	if (left != NULL)
		left->parent = result;
	if (right != NULL)
		right->parent = result;
	result->parent = NULL;
	return(result);
}

trnode_t * tree_find(tree_t * t, void * key)
{
	trnode_t * n = NULL;

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
void * tree_find(tree_t * t, void * key, tree_compare compare)
{
	trnode_t * n = NULL;
	n = find(t, key, compare);
	return(n == NULL ? NULL : n->value);
}
*/

void replace_node(tree_t * t, trnode_t * oldn, trnode_t * newn)
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

/*
int tree_rotate_left(tree_t * t, trnode_t * n)
{
	trnode_t * r = NULL;

	if (t == NULL) return(0);
	if (n == NULL) return(0);
	if (t->root == NULL) return(0);
	if (n->right == NULL) return(0);
	r = n->right;
	replace_node(t, n, r);
	n->right = r->left;
	if (r->left != NULL) {
		r->left->parent = n;
	}
	r->left = n;
	n->parent = r;
	return(1);
}

int tree_rotate_right(tree_t * t, trnode_t * n)
{
	trnode_t * l = NULL;

	if (t == NULL) return(0);
	if (n == NULL) return(0);
	if (t->root == NULL) return(0);
	if (n->left == NULL) return(0);
	l = n->left;
	replace_node(t, n, l);
	n->left = l->right;
	if (l->right != NULL) {
		l->right->parent = n;
	}
	l->right = n;
	n->parent = l;
	return(1);
}
*/

int tree_rotate_left(trnode_t * n)
{
	/*
	trnode_t * r = NULL;

	r = n->right;
	__tree_replace_node(t, n, r);
	n->right = r->left;
	if (r->left != NULL) {
		r->left->parent = n;
	}
	r->left = n;
	n->parent = r;
	*/

	trnode_t * l = NULL;
	trnode_t * p = NULL;
	trnode_t * g = NULL;

	if (n == NULL) return(0);

	l = n->left;
	p = n->parent;
	if (p == NULL) return(0);
	g = p->parent;
	if (g == NULL) return(0);

	if (g->left == p)
		g->left = n;
	else
		g->right = n;
	n->parent = g;
	n->left = p;
	p->parent = n;
	p->right = l;
	if (l != NULL)
		l->parent = p;
	return(1);
}

int tree_rotate_right(trnode_t * n)
{
	/*
	trnode_t * l = NULL;

	l = n->left;
	__tree_replace_node(t, n, l);
	n->left = l->right;
	if (l->right != NULL) {
		l->right->parent = n;
	}
	l->right = n;
	n->parent = l;
	*/

	trnode_t * r = NULL;
	trnode_t * p = NULL;
	trnode_t * g = NULL;

	if (n == NULL) return(0);

	r = n->right;
	p = n->parent;
	if (p == NULL) return(0);
	g = p->parent;
	if (g == NULL) return(0);

	if (g->right == p)
		g->right = n;
	else
		g->left = n;
	n->parent = g;
	n->right = p;
	p->parent = n;
	p->left = r;
	if (r != NULL)
		r->parent = p;
	return(1);
}


trnode_t * tree_attach_node(tree_t * t, trnode_t * node)
{
	trnode_t * n = NULL;
	int comp_result = 0;

	if (t == NULL)
		return(NULL);

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

	return(node);
}

int tree_insert(
	tree_t * t, void * key, void * value)
{
	trnode_t * n = NULL;

	n = __tree_new_node(key, value, NULL, NULL);
	if (n == NULL)
		return(0);

	if (tree_attach_node(t, n) == NULL)
		return(0);
	
	return(1);
}

trnode_t * __tree_maximum_node(trnode_t * n)
{
	assert(n != NULL);
	while (n->right != NULL) {
		n = n->right;
	}
	return(n);
}

trnode_t * tree_detach_node(tree_t * t, trnode_t * n)
{
	trnode_t * child = NULL;

	if (n == NULL)
		return(NULL);

	if (n->left != NULL && n->right != NULL) {
		trnode_t * pred = NULL;

		pred = __tree_maximum_node(n->left);
		n->key = pred->key;
		n->value = pred->value;
		n = pred;
	}

	assert(n->left == NULL || n->right == NULL);
	child = n->right == NULL ? n->left : n->right;
	replace_node(t, n, child);
	t->size--;

	n->parent = NULL;
	n->left = NULL;
	n->right = NULL;
	return(n);
}

int tree_remove(tree_t * t, trnode_t * n)
{
	if ((n = tree_detach_node(t, n)) == NULL)
		return(0);
	if (t->free != NULL)
		t->free(n->key, n->value);
	free(n);
	return(1);
}

size_t tree_size(tree_t * t)
{
	return(t == NULL ? 0 : t->size);
}

trnode_t * __tree_apply_function(trnode_t * n, tree_func func)
{
	trnode_t * r = NULL;

	if (n == NULL)
		return(NULL);
	if (!func(n->key, n->value))
		return(n);
	r = __tree_apply_function(n->left, func);
	if (r != NULL)
		return(r);
	r = __tree_apply_function(n->right, func);
	if (r != NULL)
		return(r);
	return(NULL);
}

void __tree_destroy_nodes(tree_t * t, trnode_t * n)
{
	if (n == NULL)
		return;
	__tree_destroy_nodes(t, n->left);
	__tree_destroy_nodes(t, n->right);
	if (t->free != NULL)
		t->free(n->key, n->value);
	free(n);
}

tree_t * tree_destroy(tree_t * t)
{
	if (t == NULL)
		return(NULL);
	__tree_destroy_nodes(t, t->root);
	free(t);
	return(NULL);
}

int __tree_clone_node(tree_t * t, trnode_t * n)
{
	assert(t != NULL);
	assert(n != NULL);
	if (tree_insert(t, tree_key(n), tree_value(n)) == 0)
		return(0);
	if (tree_left(n) != NULL)
		if (!__tree_clone_node(t, tree_left(n)))
			return(0);
	if (tree_right(n) != NULL)
		if (!__tree_clone_node(t, tree_right(n)))
			return(0);
	return(1);
}

tree_t * tree_clone(tree_t * t)
{
	tree_t * new_t = NULL;

	if (t == NULL)
		return(NULL);
	new_t = tree_create(t->compare, t->free);
	if (new_t == NULL)
		return(NULL);
	if (!__tree_clone_node(new_t, tree_top(t))) {
		tree_destroy(new_t);
		return(NULL);
	}
	return(new_t);
}

trnode_t * tree_apply(tree_t * t, tree_func func)
{
	trnode_t * n = NULL;

	if (t == NULL) return(NULL);
	n = t->root;
	return(__tree_apply_function(n, func));
}

int __tree_write_gv_node(trnode_t * n, FILE * fout,
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
		if (!__tree_write_gv_node(n->left, fout, write_node_key, write_node_value))
			return(0);
	}
	if (n->right != NULL) {
		if (!__tree_write_gv_node(n->right, fout, write_node_key, write_node_value))
			return(0);
	}

	return(1);
}

int tree_write_gv(tree_t * t, char * filename,
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
	if (!__tree_write_gv_node(t->root, fout, write_node_key, write_node_value)) {
		fclose(fout);
		return(0);
	}
	fprintf(fout, "}\n");
	fclose(fout);
	return(1);
}

void * tree_key(trnode_t * n)
{
	return(n == NULL ? NULL : n->key);
}

void * tree_value(trnode_t * n)
{
	return(n == NULL ? NULL : n->value);
}

trnode_t * tree_top(tree_t * t)
{
	return(t == NULL ? NULL : t->root);
}

trnode_t * tree_left(trnode_t * n)
{
	return(n == NULL ? NULL : n->left);
}

trnode_t * tree_right(trnode_t * n)
{
	return(n == NULL ? NULL : n->right);
}

trnode_t * tree_parent(trnode_t * n)
{
	return(n == NULL ? NULL : n->parent);
}

trnode_t * tree_in_order_first(tree_t * t)
{
	trnode_t * n = NULL;

	if (t == NULL)
		return(NULL);
	if (t->root == NULL)
		return(NULL);
	
	n = t->root;
	while (n->left != NULL)
		n = n->left;
	return(n);
}

trnode_t * tree_in_order_last(tree_t * t)
{
	trnode_t * n = NULL;

	if (t == NULL)
		return(NULL);
	if (t->root == NULL)
		return(NULL);
	
	n = t->root;
	while (n->right != NULL)
		n = n->right;
	return(n);
}

trnode_t * tree_in_order_next(trnode_t * n)
{
	trnode_t * l = NULL;

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

trnode_t * tree_in_order_prev(trnode_t * n)
{
	trnode_t * l = NULL;

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

trnode_t * tree_pre_order_first(tree_t * t)
{
	if (t == NULL)
		return(NULL);
	if (t->root == NULL)
		return(NULL);
	return(t->root);
}

trnode_t * tree_pre_order_last(tree_t * t)
{
	trnode_t * n = NULL;

	if (t == NULL)
		return(NULL);
	if (t->root == NULL)
		return(NULL);
	n = t->root;
	while (n->right != NULL)
		n = n->right;
	return(n);
}

trnode_t * tree_pre_order_next(trnode_t * n)
{
	trnode_t * l = NULL;

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

trnode_t * tree_pre_order_prev(trnode_t * n)
{
	trnode_t * l = NULL;

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


trnode_t * tree_post_order_first(tree_t * t)
{
	trnode_t * n = NULL;

	if (t == NULL)
		return(NULL);
	if (t->root == NULL)
		return(NULL);
	
	n = t->root;
	while (n->left != NULL)
		n = n->left;
	return(n);
}

trnode_t * tree_post_order_last(tree_t * t)
{
	if (t == NULL)
		return(NULL);
	return(t->root);
}

trnode_t * tree_post_order_next(trnode_t * n)
{
	trnode_t * l = NULL;

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

trnode_t * tree_post_order_prev(trnode_t * n)
{
	trnode_t * l = NULL;

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

int trees_are_identical(tree_t * t1, tree_t * t2)
{
	trnode_t * n1 = NULL;
	trnode_t * n2 = NULL;

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
	
	n1 = tree_in_order_first(t1);
	n2 = tree_in_order_first(t2);
	while ((n1 != NULL) && (n2 != NULL)) {
		if (t1->compare(n1->value, n2->value) != 0)
			return(0);
		n1 = tree_in_order_next(n1);
		n2 = tree_in_order_next(n2);
	}
	// For this to fail, then either t1->size or t2->size must be corrupt
	assert(n1 == NULL && n2 == NULL);
	return(1);
}

trnode_t * tree_level_order_first(tree_t * t)
{
	if (t == NULL)
		return(NULL);
	return(t->root);
}

size_t __trnode_t_depth(trnode_t * n)
{
	size_t depth = 0;

	while (n != NULL) {
		n = tree_parent(n);
		depth++;
	}
	return(depth);
}

trnode_t * tree_level_order_last(tree_t * t)
{
	trnode_t * n = NULL;
	trnode_t * deepest_right = NULL;
	size_t deepest_depth = 0;
	size_t this_depth = 0;

	// This method is awful

	deepest_right = tree_top(t);
	deepest_depth = 0;

	n = tree_post_order_last(t);
	while (n != NULL) {
		this_depth = __trnode_t_depth(n);
		if (this_depth > deepest_depth) {
			deepest_depth = this_depth;
			deepest_right = n;
		}
		n = tree_post_order_prev(n);
	}
	return(deepest_right);
}

trnode_t * tree_level_order_next(trnode_t * n)
{
	size_t my_depth = 0;
	trnode_t * s = n;

	if (n == NULL)
		return(NULL);
	
	// This method is awful

	my_depth = __trnode_t_depth(n);
	s = n;
	n = tree_post_order_next(n);
	while (n != NULL && __trnode_t_depth(n) != my_depth)
		n = tree_post_order_next(n);
	if (n != NULL)
		return(n);
	n = s;
	my_depth++;
	while (n->parent != NULL)
		n = n->parent;
	while (n->left != NULL)
		n = n->left;
	while (n != NULL && __trnode_t_depth(n) != my_depth) {
		n = tree_in_order_next(n);
	}

	return(n);
}

trnode_t * tree_level_order_prev(trnode_t * n)
{
	size_t my_depth = 0;
	trnode_t * s = n;

	if (n == NULL)
		return(NULL);

	// This method is awful

	my_depth = __trnode_t_depth(n);
	s = n;
	n = tree_post_order_prev(n);
	while (n != NULL && __trnode_t_depth(n) != my_depth)
		n = tree_post_order_prev(n);
	if (n != NULL)
		return(n);
	n = s;
	my_depth--;
	while (n->parent != NULL)
		n = n->parent;
	while (n->right != NULL)
		n = n->right;
	while (n != NULL && __trnode_t_depth(n) != my_depth) {
		n = tree_in_order_prev(n);
	}

	return(n);
}

size_t __tree_max_depth(trnode_t * n)
{
	int l = 0;
	int r = 0;
	int m = 0;

	if (n == NULL) return(0);
	l += __tree_max_depth(n->left);
	r += __tree_max_depth(n->right);
	m = l;
	if (m < r)
		m = r;
	m++;
	return(m);
}

size_t tree_max_depth(trnode_t * n)
{
	int m = 0;

	if (n == NULL) return(0);
	m = __tree_max_depth(n);
	return(m);
}

size_t __tree_min_depth(trnode_t * n)
{
	int l = 0;
	int r = 0;
	int m = 0;

	if (n == NULL) return(0);
	l += __tree_min_depth(n->left);
	r += __tree_min_depth(n->right);
	m = l;
	if (m > r)
		m = r;
	m++;
	return(m);
}

size_t tree_min_depth(trnode_t * n)
{
	int m = 0;

	if (n == NULL) return(0);
	m = __tree_min_depth(n);
	return(m);
}

int tree_is_full(tree_t * t)
{
	trnode_t * n = NULL;

	for (n = tree_in_order_first(t); n != NULL; n = tree_in_order_next(n)) {
		if (n->left == NULL && n->right == NULL) continue;
		if (n->left == NULL) return(0);
		if (n->right == NULL) return(0);
	}
	return(1);
}

/*
int tree_is_proper(tree_t * t)
{
	return(tree_is_full(t));
}
*/

int tree_is_perfect(tree_t * t)
{
	if (!tree_is_full(t)) return(0);
	if (tree_max_depth(tree_top(t)) != tree_min_depth(tree_top(t))) return(0);
	return(1);
}

int tree_is_complete(tree_t * t)
{
	trnode_t * n = NULL;
	trnode_t * last_n = NULL;
	size_t current_depth = 0;
	size_t last_depth = 0;
	size_t original_depth = 0;
	size_t min_depth = 0;
	size_t depth_right = 0;
	size_t depth_difference = 0;

	n = tree_top(t);
	while (tree_left(n) != NULL) {
		n = tree_left(n);
		current_depth++;
	}
	original_depth = current_depth;
	min_depth = current_depth;

	last_depth = current_depth;
	while (n != NULL) {
		if (tree_parent(n) == NULL) break;
		if (
			(n == tree_left(tree_parent(n)))
			&&
			(tree_right(tree_parent(n)) != NULL)
			)
		{
			n = tree_parent(n);
			n = tree_right(n);
		}
		else {
			last_n = n;
			n = tree_parent(n);
			current_depth--;
			while (
				(n != NULL)
				&&
					(
						tree_right(n) == NULL
						||
						tree_right(n) == last_n
					)
				)
			{
				last_n = n;
				n = tree_parent(n);
				current_depth--;
			}
			if (n != NULL) {
				n = tree_right(n);
				current_depth++;
			}
		}
		while (n != NULL && !__tree_is_leaf(n)) {
			if (tree_left(n) != NULL) {
				n = tree_left(n);
				current_depth++;
			}
			else {
				n = tree_right(n);
				current_depth++;
			}
		}
		if (n != NULL) {
			if ((current_depth > 0) && (current_depth < min_depth))
				min_depth = current_depth;

			if ((current_depth > 0) && (current_depth > last_depth))
				return(0);
		}
	}

	if ((original_depth > min_depth) && (original_depth - min_depth > 1))
		return(0);
	if ((original_depth < min_depth) && (min_depth - original_depth > 1))
		return(0);
	
	n = tree_top(t);
	while (tree_right(n) != NULL) {
		n = tree_right(n);
		depth_right++;
	}
	if (original_depth > depth_right)
		depth_difference = original_depth - depth_right;
	else
		depth_difference = depth_right - original_depth;
	if (depth_difference > 1)
		return(0);

	return(1);
}

int tree_is_balanced(tree_t * t)
{
	trnode_t * n = NULL;
	size_t l = 0;
	size_t r = 0;
	size_t m = 0;

	for (n = tree_in_order_first(t); n != NULL; n = tree_in_order_next(n)) {
		if (__tree_is_leaf(n)) continue;
		l = tree_max_depth(tree_left(n));
		r = tree_max_depth(tree_right(n));
		if (l > r)
			m = l - r;
		else
			m = r - l;
		if (m > 1)
			return(0);
	}
	return(1);
}

int tree_is_degenerate(tree_t * t)
{
	if (tree_max_depth(tree_top(t)) != tree_size(t))
		return(0);
	return(1);
}

