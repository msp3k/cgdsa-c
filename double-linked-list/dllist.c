#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dllist.h"

struct __dllist_t {
	struct __dllist_t * prev;
	struct __dllist_t * next;
	struct __dllist_t * sentinel;
	size_t size;
	void * value;
	dllist_compare compare;
	dllist_free free;
};

int __dllist_is_sentinel(dllist_t * n)
{
	if (n == NULL) return(0);
	return((n->sentinel == n));
}

dllist_t * dllist_create(dllist_compare compare, dllist_free free)
{
	dllist_t * l = NULL;

	l = (dllist_t *)malloc(sizeof(dllist_t));
	if (l == NULL) return(NULL);
	l->prev = l;
	l->next = l;
	l->sentinel = l;
	l->size = 0;
	l->value = NULL;
	l->compare = compare;
	l->free = free;

	return(l);
}

int dllist_free_node(void * arg)
{
	free(arg);
	return(1);
}

dllist_t * dllist_apply(dllist_t * list, dllist_free func)
{
	dllist_t * n;

	if (list == NULL) return(NULL);
	if (func == NULL) return(NULL);
	list = dllist_sentinel(list);

	for (n = dllist_first(list); n != NULL; n = dllist_next(n)) {
		if (!func(dllist_value(n)))
			return(n);
	}
	return(NULL);
}

dllist_t * dllist_destroy(dllist_t * list)
{
	dllist_t * n;

	if (list == NULL) return(NULL);
	list = dllist_sentinel(list);

	while (dllist_size(list) > 0) {
		n = dllist_first(list);
		dllist_remove(n);
	}
	free(list);

	return(NULL);
}

dllist_t * dllist_sentinel(dllist_t * node)
{
	if (node == NULL) return(NULL);
	return(node->sentinel);
}

dllist_t * __dllist_make_new_node(dllist_t * node, void * value)
{
	dllist_t * new_node = NULL;

	assert(node != NULL);
	assert(value != NULL);
	
	new_node = (dllist_t *)malloc(sizeof(dllist_t));
	if (new_node == NULL) return(NULL);

	new_node->prev = NULL;
	new_node->next = NULL;
	new_node->sentinel = node->sentinel;
	new_node->size = 0;
	new_node->value = value;
	new_node->compare = node->compare;
	new_node->free = node->free;

	return(new_node);
}

dllist_t * dllist_insert_before(dllist_t * node, void * value)
{
	dllist_t * new_node = NULL;

	if (node == NULL) return(NULL);
	if (value == NULL) return(NULL);

	new_node = __dllist_make_new_node(node, value);
	if (new_node == NULL) return(NULL);

	node->prev->next = new_node;
	new_node->prev = node->prev;
	new_node->next = node;
	node->prev = new_node;
	node->sentinel->size++;

	return(new_node);
}

dllist_t * dllist_insert_after(dllist_t * node, void * value)
{
	dllist_t * new_node = NULL;

	if (node == NULL) return(NULL);
	if (value == NULL) return(NULL);

	new_node = __dllist_make_new_node(node, value);
	if (new_node == NULL) return(NULL);

	node->next->prev = new_node;
	new_node->next = node->next;
	new_node->prev = node;
	node->next = new_node;
	node->sentinel->size++;

	return(new_node);
}

dllist_t * dllist_insert(dllist_t * node, void * value)
{
	dllist_t * n = NULL;
	dllist_t * r = NULL;

	if (node == NULL) return(NULL);
	if (value == NULL) return(NULL);

	n = node;
	if (node->compare != NULL) {
		n = dllist_sentinel(node);
		n = dllist_first(n);
		while (
			(n != NULL)
			&& 
			(!__dllist_is_sentinel(n))
			&& 
			(n->compare(n->value, value) < 1)
			)
		{
			n = n->next;
		}
		if (n == NULL)
			n = dllist_sentinel(node);
	}
	r = dllist_insert_before(n, value);

	return(r);
}

dllist_t * dllist_push_front(dllist_t * list, void * value)
{
	dllist_t * r = NULL;

	if (list == NULL) return(NULL);
	list = dllist_sentinel(list);
	r = dllist_insert_after(list, value);

	return(r);
}

dllist_t * dllist_push_back(dllist_t * list, void * value)
{
	dllist_t * r = NULL;

	if (list == NULL) return(NULL);
	list = dllist_sentinel(list);
	r = dllist_insert_before(list, value);

	return(r);
}

dllist_t * dllist_clone(dllist_t * list)
{
	dllist_t * new_list = NULL;
	dllist_t * n = NULL;

	if (list == NULL) return(NULL);
	list = dllist_sentinel(list);

	new_list = dllist_create(list->compare, list->free);
	if (new_list == NULL) return(NULL);

	for (n = dllist_first(list); n != NULL; n = dllist_next(n)) {
		if (dllist_push_back(new_list, n->value) == NULL) {
			dllist_destroy(new_list);
			return(NULL);
		}
	}
	return(new_list);
}

dllist_t * dllist_find(dllist_t * list, void * value)
{
	dllist_t * n = NULL;

	if (list == NULL) return(NULL);
	if (value == NULL) return(NULL);
	list = dllist_sentinel(list);

	for (n = dllist_first(list); n != NULL; n = dllist_next(n)) {
		if (n->value == value)
			return(n);
		if (n->compare != NULL && (n->compare)(n->value, value) == 0)
			return(n);
	}
	return(NULL);
}

void * dllist_value(dllist_t * node)
{
	if (node == NULL) return(NULL);
	return(node->value);
}

int dllist_remove(dllist_t * node)
{
	if (__dllist_is_sentinel(node)) return(0);
	node->next->prev = node->prev;
	node->prev->next = node->next;
	node->sentinel->size--;
	if (node->free != NULL)
		node->free(node->value);
	free(node);
	return(1);
}

int dllist_clear(dllist_t * list)
{
	if (list == NULL) return(0);
	list = dllist_sentinel(list);
	while (dllist_size(list) != 0) {
		if (!dllist_remove(dllist_first(list)))
			return(0);
	}
	return(1);
}

dllist_t * dllist_first(dllist_t * list)
{
	if (list == NULL) return(NULL);
	list = dllist_sentinel(list);
	if (__dllist_is_sentinel(list->next)) return(NULL);
	return(list->next);
}

dllist_t * dllist_last(dllist_t * list)
{
	if (list == NULL) return(NULL);
	list = dllist_sentinel(list);
	if (__dllist_is_sentinel(list->prev)) return(NULL);
	return(list->prev);
}

dllist_t * dllist_next(dllist_t * node)
{
	if (node == NULL) return(NULL);
	if (__dllist_is_sentinel(node)) return(NULL);
	if (__dllist_is_sentinel(node->next)) return(NULL);
	return(node->next);
}

dllist_t * dllist_prev(dllist_t * node)
{
	if (node == NULL) return(NULL);
	if (__dllist_is_sentinel(node)) return(NULL);
	if (__dllist_is_sentinel(node->prev)) return(NULL);
	return(node->prev);
}

int dllists_are_identical(dllist_t * list1, dllist_t * list2)
{
	dllist_t * n1 = NULL;
	dllist_t * n2 = NULL;

	if (list1 == NULL && list2 == NULL) return(1);
	if (list1 == NULL) return(0);
	if (list2 == NULL) return(0);

	n1 = dllist_first(list1);
	n2 = dllist_first(list2);

	while (n1 != NULL && n2 != NULL) {
		if (
			(n1->value != n2->value)
			&&
				(
				(n1->compare == NULL)
				||
				(n1->compare != n2->compare)
				||
				((n1->compare)(n1->value, n2->value) != 0)
				)
			)
		{
			return(0);
		}
		n1 = dllist_next(n1);
		n2 = dllist_next(n2);
	}
	if (n1 != NULL) return(0);
	if (n2 != NULL) return(0);
	return(1);
}

size_t dllist_size(dllist_t * list)
{
	if (list == NULL) return(0);
	assert(list->sentinel != NULL);
	return(list->sentinel->size);
}

/*
int move_node(dllist_t * node, dllist_t * dst)
{
	dllist_t * old_prev = NULL;
	dllist_t * old_next = NULL;
	dllist_t * new_prev = NULL;
	dllist_t * new_next = NULL;

	if (node == NULL) return(0);
	if (__dllist_is_sentinel(node)) return(0);
	dst = dllist_sentinel(dst);

	old_prev = node->prev;
	old_next = node->next;
	new_prev = dst->prev;
	new_next = dst;

	node->sentinel->size--;
	node->sentinel = dst;
	node->sentinel->size++;

	old_prev->next = old_next;
	old_next->prev = old_prev;

	new_prev->next = node;
	node->prev = new_prev;
	new_next->prev = node;
	node->next = new_next;

	return(1);
}
*/

int dllist_detach_node(dllist_t * list, dllist_t * node)
{
	if (node == NULL) return(0);
	if (__dllist_is_sentinel(node)) return(0);
	list = dllist_sentinel(list);

	list->size--;
	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->sentinel = NULL;
	node->compare = NULL;
	node->free = NULL;

	return(1);
}

int dllist_attach_node(dllist_t * list, dllist_t * node)
{
	if (node == NULL) return(0);
	if (__dllist_is_sentinel(node)) return(0);
	list = dllist_sentinel(list);

	node->prev = list->prev;
	node->next = list;
	list->prev = node;
	node->prev->next = node;
	node->sentinel = list;
	node->compare = list->compare;
	node->free = list->free;
	list->size++;

	return(1);
}

int dllist_sort(dllist_t * list)
{
	dllist_t * n = NULL;
	dllist_t * l1 = NULL;
	dllist_t * l2 = NULL;
	dllist_t * pivot = NULL;
	int r = 0;

	if (list == NULL) return(0);
	if (dllist_size(list) < 2) return(1);
	l1 = dllist_create(list->compare, list->free);
	if (l1 == NULL) return(0);
	l2 = dllist_create(list->compare, list->free);
	if (l1 == NULL) {
		dllist_destroy(l1);
		return(0);
	}

	n = dllist_first(list);
	pivot = n;
	dllist_detach_node(list, n);
	while (dllist_size(list) > 0) {
		n = dllist_first(list);
		r = list->compare(dllist_value(n), dllist_value(pivot));
		if (r < 0) {
			dllist_detach_node(list, n);
			dllist_attach_node(l1, n);
		} else {
			dllist_detach_node(list, n);
			dllist_attach_node(l2, n);
		}
	}

	if (!dllist_sort(l1) || !dllist_sort(l2)) {
		dllist_destroy(l1);
		dllist_destroy(l2);
		dllist_destroy(list);
		return(0);
	}

	n = dllist_first(l1);
	while (n != NULL) {
		dllist_detach_node(l1, n);
		dllist_attach_node(list, n);
		n = dllist_first(l1);
	}
	dllist_attach_node(list, pivot);
	n = dllist_first(l2);
	while (n != NULL) {
		dllist_detach_node(l2, n);
		dllist_attach_node(list, n);
		n = dllist_first(l2);
	}

	dllist_destroy(l1);
	dllist_destroy(l2);

	return(1);
}

