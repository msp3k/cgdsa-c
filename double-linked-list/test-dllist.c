#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dllist.h"

// int __dllist_is_sentinel(dllist_t * n);

int compare(const void * a1, const void *a2)
{
	int i1 = 0;
	int i2 = 0;

	if (a1 == NULL && a1 == NULL) return(0);
	if (a1 == NULL) return(-1);
	if (a2 == NULL) return(1);

	i1 = *(int *)a1;
	i2 = *(int *)a2;

	if (i1 > i2) return(1);
	if (i1 < i2) return(-1);
	return(0);
}

int * dup_int(int i)
{
	int * iptr = NULL;

	iptr = (int *)malloc(sizeof(int));
	*iptr = i;
	return(iptr);
}

int main(int argc, char ** argv)
{
	{
		// dllist_t * dllist_create(int (*compare)(const void * a1, const void * a2));
		// dllist_t * dllist_destroy(dllist_t * list);

		dllist_t * l = NULL;

		l = dllist_create(NULL, NULL);
		assert(l != NULL);
		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// dllist_t * dllist_insert_before(dllist_t * node, void * value);

		dllist_t * l = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };
		dllist_t * n = NULL;

		l = dllist_create(NULL,NULL);
		assert(l != NULL);

		n = l;
		assert((n = dllist_insert_before(n, (void *)&(a[4]))) != NULL);
		assert((n = dllist_insert_before(n, (void *)&(a[3]))) != NULL);
		assert((n = dllist_insert_before(n, (void *)&(a[2]))) != NULL);
		assert((n = dllist_insert_before(n, (void *)&(a[1]))) != NULL);
		assert((n = dllist_insert_before(n, (void *)&(a[0]))) != NULL);

		n = dllist_first(l);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 1);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 2);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 3);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 4);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 5);
		n = dllist_next(n);

		assert(n == NULL);

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// dllist_t * dllist_insert_after(dllist_t * node, void * value);

		dllist_t * l = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };
		dllist_t * n = NULL;

		l = dllist_create(NULL,NULL);
		assert(l != NULL);

		n = l;
		assert((n = dllist_insert_after(n, (void *)&(a[0]))) != NULL);
		assert((n = dllist_insert_after(n, (void *)&(a[1]))) != NULL);
		assert((n = dllist_insert_after(n, (void *)&(a[2]))) != NULL);
		assert((n = dllist_insert_after(n, (void *)&(a[3]))) != NULL);
		assert((n = dllist_insert_after(n, (void *)&(a[4]))) != NULL);

		n = dllist_first(l);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 1);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 2);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 3);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 4);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 5);
		n = dllist_next(n);

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// dllist_t * dllist_insert(dllist_t * node, void * value);

		dllist_t * l = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };
		dllist_t * n = NULL;

		l = dllist_create(NULL,NULL);
		assert(l != NULL);

		n = l;
		assert((n = dllist_insert(n, (void *)&(a[4]))) != NULL);
		assert((n = dllist_insert(n, (void *)&(a[3]))) != NULL);
		assert((n = dllist_insert(n, (void *)&(a[2]))) != NULL);
		assert((n = dllist_insert(n, (void *)&(a[1]))) != NULL);
		assert((n = dllist_insert(n, (void *)&(a[0]))) != NULL);

		n = dllist_first(l);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 1);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 2);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 3);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 4);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 5);
		n = dllist_next(n);

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// dllist_t * dllist_insert(dllist_t * node, void * value);

		dllist_t * l = NULL;
		int a[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		dllist_t * n = NULL;

		l = dllist_create(compare,NULL);
		assert(l != NULL);

		assert((n = dllist_insert(l, (void *)&(a[5]))) != NULL);
		assert(((dllist_prev(n) == NULL) || *((int *)(dllist_value(dllist_prev(n)))) <= *((int *)(dllist_value(n)))) && ((dllist_next(n) == NULL) || *((int *)(dllist_value(dllist_next(n)))) >= *((int *)(dllist_value(n)))));

		assert((n = dllist_insert(l, (void *)&(a[3]))) != NULL);
		assert(((dllist_prev(n) == NULL) || *((int *)(dllist_value(dllist_prev(n)))) <= *((int *)(dllist_value(n)))) && ((dllist_next(n) == NULL) || *((int *)(dllist_value(dllist_next(n)))) >= *((int *)(dllist_value(n)))));

		assert((n = dllist_insert(l, (void *)&(a[7]))) != NULL);
		assert(((dllist_prev(n) == NULL) || *((int *)(dllist_value(dllist_prev(n)))) <= *((int *)(dllist_value(n)))) && ((dllist_next(n) == NULL) || *((int *)(dllist_value(dllist_next(n)))) >= *((int *)(dllist_value(n)))));

		assert((n = dllist_insert(l, (void *)&(a[0]))) != NULL);
		assert(((dllist_prev(n) == NULL) || *((int *)(dllist_value(dllist_prev(n)))) <= *((int *)(dllist_value(n)))) && ((dllist_next(n) == NULL) || *((int *)(dllist_value(dllist_next(n)))) >= *((int *)(dllist_value(n)))));

		assert((n = dllist_insert(l, (void *)&(a[9]))) != NULL);
		assert(((dllist_prev(n) == NULL) || *((int *)(dllist_value(dllist_prev(n)))) <= *((int *)(dllist_value(n)))) && ((dllist_next(n) == NULL) || *((int *)(dllist_value(dllist_next(n)))) >= *((int *)(dllist_value(n)))));

		assert((n = dllist_insert(l, (void *)&(a[1]))) != NULL);
		assert(((dllist_prev(n) == NULL) || *((int *)(dllist_value(dllist_prev(n)))) <= *((int *)(dllist_value(n)))) && ((dllist_next(n) == NULL) || *((int *)(dllist_value(dllist_next(n)))) >= *((int *)(dllist_value(n)))));

		assert((n = dllist_insert(l, (void *)&(a[8]))) != NULL);
		assert(((dllist_prev(n) == NULL) || *((int *)(dllist_value(dllist_prev(n)))) <= *((int *)(dllist_value(n)))) && ((dllist_next(n) == NULL) || *((int *)(dllist_value(dllist_next(n)))) >= *((int *)(dllist_value(n)))));

		n = dllist_first(l);
		assert(*((int *)(dllist_value(n))) == a[0]);
		n = dllist_next(n);
		assert(*((int *)(dllist_value(n))) == a[1]);
		n = dllist_next(n);
		assert(*((int *)(dllist_value(n))) == a[3]);
		n = dllist_next(n);
		assert(*((int *)(dllist_value(n))) == a[5]);
		n = dllist_next(n);
		assert(*((int *)(dllist_value(n))) == a[7]);
		n = dllist_next(n);
		assert(*((int *)(dllist_value(n))) == a[8]);
		n = dllist_next(n);
		assert(*((int *)(dllist_value(n))) == a[9]);

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// dllist_t * dllist_push_front(dllist_t * list, void * value);

		dllist_t * l = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };
		dllist_t * n = NULL;

		l = dllist_create(NULL,NULL);
		assert(l != NULL);

		assert(dllist_push_front(l, (void *)&(a[4])) != NULL);
		assert(dllist_push_front(l, (void *)&(a[3])) != NULL);
		assert(dllist_push_front(l, (void *)&(a[2])) != NULL);
		assert(dllist_push_front(l, (void *)&(a[1])) != NULL);
		assert(dllist_push_front(l, (void *)&(a[0])) != NULL);

		n = dllist_first(l);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 1);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 2);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 3);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 4);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 5);
		n = dllist_next(n);

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// dllist_t * dllist_push_back(dllist_t * list, void * value);

		dllist_t * l = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };
		dllist_t * n = NULL;

		l = dllist_create(NULL,NULL);
		assert(l != NULL);

		assert(dllist_push_back(l, (void *)&(a[4])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[3])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[2])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[1])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[0])) != NULL);

		n = dllist_first(l);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 5);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 4);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 3);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 2);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 1);
		n = dllist_next(n);

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// dllist_push_front() and dllist_push_back()

		dllist_t * l = NULL;
		int a[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		dllist_t * n = NULL;

		l = dllist_create(NULL,NULL);
		assert(l != NULL);

		assert(dllist_push_back(l, (void *)&(a[0])) != NULL);

		n = dllist_first(l);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_prev(n);
		assert(n == NULL);

		assert(dllist_push_front(l, (void *)&(a[1])) != NULL);

		n = dllist_first(l);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_prev(n);
		assert(n == NULL);

		assert(dllist_push_back(l, (void *)&(a[2])) != NULL);

		n = dllist_first(l);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_prev(n);
		assert(n == NULL);

		assert(dllist_push_front(l, (void *)&(a[3])) != NULL);

		n = dllist_first(l);
		assert(dllist_value(n) == (void *)&(a[3]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[3]));
		n = dllist_prev(n);
		assert(n == NULL);

		assert(dllist_push_back(l, (void *)&(a[4])) != NULL);

		n = dllist_first(l);
		assert(dllist_value(n) == (void *)&(a[3]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[4]));
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l);
		assert(dllist_value(n) == (void *)&(a[4]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[3]));
		n = dllist_prev(n);
		assert(n == NULL);

		assert(dllist_push_front(l, (void *)&(a[5])) != NULL);

		n = dllist_first(l);
		assert(dllist_value(n) == (void *)&(a[5]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[3]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[4]));
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l);
		assert(dllist_value(n) == (void *)&(a[4]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[3]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[5]));
		n = dllist_prev(n);
		assert(n == NULL);

		assert(dllist_push_back(l, (void *)&(a[6])) != NULL);

		n = dllist_first(l);
		assert(dllist_value(n) == (void *)&(a[5]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[3]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[4]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[6]));
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l);
		assert(dllist_value(n) == (void *)&(a[6]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[4]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[3]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[5]));
		n = dllist_prev(n);
		assert(n == NULL);

		assert(dllist_push_front(l, (void *)&(a[7])) != NULL);

		n = dllist_first(l);
		assert(dllist_value(n) == (void *)&(a[7]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[5]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[3]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[4]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[6]));
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l);
		assert(dllist_value(n) == (void *)&(a[6]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[4]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[3]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[5]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[7]));
		n = dllist_prev(n);
		assert(n == NULL);

		assert(dllist_push_back(l, (void *)&(a[8])) != NULL);

		n = dllist_first(l);
		assert(dllist_value(n) == (void *)&(a[7]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[5]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[3]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[4]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[6]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[8]));
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l);
		assert(dllist_value(n) == (void *)&(a[8]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[6]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[4]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[3]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[5]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[7]));
		n = dllist_prev(n);
		assert(n == NULL);

		assert(dllist_push_front(l, (void *)&(a[9])) != NULL);

		n = dllist_first(l);
		assert(dllist_value(n) == (void *)&(a[9]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[7]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[5]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[3]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[4]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[6]));
		n = dllist_next(n);
		assert(dllist_value(n) == (void *)&(a[8]));
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l);
		assert(dllist_value(n) == (void *)&(a[8]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[6]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[4]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[2]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[0]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[1]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[3]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[5]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[7]));
		n = dllist_prev(n);
		assert(dllist_value(n) == (void *)&(a[9]));
		n = dllist_prev(n);
		assert(n == NULL);

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// dllist_t * dllist_clone(dllist_t * list);
		dllist_t * l1 = NULL;
		dllist_t * l2 = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };
		dllist_t * n1 = NULL;
		dllist_t * n2 = NULL;

		l1 = dllist_create(NULL,NULL);
		assert(l1 != NULL);

		assert(dllist_push_front(l1, (void *)&(a[4])) != NULL);
		assert(dllist_push_front(l1, (void *)&(a[3])) != NULL);
		assert(dllist_push_front(l1, (void *)&(a[2])) != NULL);
		assert(dllist_push_front(l1, (void *)&(a[1])) != NULL);
		assert(dllist_push_front(l1, (void *)&(a[0])) != NULL);

		l2 = dllist_clone(l1);
		assert(l2 != NULL);
		assert(l2 != l1);

		n1 = dllist_first(l1);
		n2 = dllist_first(l2);
		while (n1 != NULL && n2 != NULL) {
			assert(dllist_value(n1) == dllist_value(n2));
			n1 = dllist_next(n1);
			n2 = dllist_next(n2);
		}
		assert(n1 == NULL);
		assert(n2 == NULL);

		l1 = dllist_destroy(l1);
		assert(l1 == NULL);
		l2 = dllist_destroy(l2);
		assert(l1 == NULL);
	}

	{
		// dllist_t * dllist_find(dllist_t * list, void * value);

		dllist_t * l = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };
		dllist_t * n = NULL;
		int i;

		l = dllist_create(NULL,NULL);
		assert(l != NULL);

		n = l;
		assert((n = dllist_insert_before(n, (void *)&(a[4]))) != NULL);
		assert((n = dllist_insert_before(n, (void *)&(a[3]))) != NULL);
		assert((n = dllist_insert_before(n, (void *)&(a[2]))) != NULL);
		assert((n = dllist_insert_before(n, (void *)&(a[1]))) != NULL);
		assert((n = dllist_insert_before(n, (void *)&(a[0]))) != NULL);

		i = 3;
		n = dllist_find(l, (void *)&i);
		assert(n == NULL);

		n = dllist_find(l, (void *)&(a[3]));
		assert(n != NULL);
		assert( *((int *)(dllist_value(n))) == a[3]);

		n = dllist_find(l, (void *)&(a[0]));
		assert(n != NULL);
		assert( *((int *)(dllist_value(n))) == a[0]);

		n = dllist_find(l, (void *)&(a[4]));
		assert(n != NULL);
		assert( *((int *)(dllist_value(n))) == a[4]);

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// dllist_t * dllist_find(dllist_t * list, void * value);

		dllist_t * l = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };
		dllist_t * n = NULL;
		int i;

		l = dllist_create(compare,NULL);
		assert(l != NULL);

		n = l;
		assert((n = dllist_insert_before(n, (void *)&(a[4]))) != NULL);
		assert((n = dllist_insert_before(n, (void *)&(a[3]))) != NULL);
		assert((n = dllist_insert_before(n, (void *)&(a[2]))) != NULL);
		assert((n = dllist_insert_before(n, (void *)&(a[1]))) != NULL);
		assert((n = dllist_insert_before(n, (void *)&(a[0]))) != NULL);

		i = 3;
		n = dllist_find(l, (void *)&i);
		assert(n != NULL);
		assert( *((int *)(dllist_value(n))) == i);

		n = dllist_find(l, (void *)&(a[3]));
		assert(n != NULL);
		assert( *((int *)(dllist_value(n))) == a[3]);

		n = dllist_find(l, (void *)&(a[0]));
		assert(n != NULL);
		assert( *((int *)(dllist_value(n))) == a[0]);

		n = dllist_find(l, (void *)&(a[4]));
		assert(n != NULL);
		assert( *((int *)(dllist_value(n))) == a[4]);

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// void * dllist_value(dllist_t * node);

		dllist_t * l = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };
		dllist_t * n = NULL;

		l = dllist_create(NULL,NULL);
		assert(l != NULL);

		assert(dllist_push_back(l, (void *)&(a[4])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[3])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[2])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[1])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[0])) != NULL);

		n = dllist_first(l);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 5);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 4);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 3);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 2);
		n = dllist_next(n);

		assert(n != l);
		assert(*(int *)(dllist_value(n)) == 1);
		n = dllist_next(n);

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// int dllist_remove(dllist_t * node);

		dllist_t * l = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };
		dllist_t * n = NULL;

		l = dllist_create(NULL,NULL);
		assert(l != NULL);

		assert(dllist_push_back(l, (void *)&(a[4])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[3])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[2])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[1])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[0])) != NULL);

		n = dllist_first(l);
		n = dllist_next(n);
		n = dllist_next(n);
		assert(*(int *)(dllist_value(n)) == a[2]);
		assert(dllist_remove(n));

		n = dllist_first(l);
		assert(*(int *)dllist_value(n) == a[4]);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == a[3]);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == a[1]);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == a[0]);
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_first(l);
		assert(*(int *)(dllist_value(n)) == a[4]);
		assert(dllist_remove(n));

		n = dllist_first(l);
		assert(*(int *)dllist_value(n) == a[3]);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == a[1]);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == a[0]);
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l);
		assert(*(int *)(dllist_value(n)) == a[0]);
		assert(dllist_remove(n));

		n = dllist_first(l);
		assert(*(int *)dllist_value(n) == a[3]);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == a[1]);
		n = dllist_next(n);
		assert(n == NULL);

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// dllist_t * dllist_first(dllist_t * list);

		dllist_t * l = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };

		l = dllist_create(NULL,NULL);
		assert(l != NULL);

		assert(dllist_push_back(l, (void *)&(a[4])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[3])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[2])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[1])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[0])) != NULL);

		assert(dllist_value(dllist_first(l)) == (void *)&(a[4]));

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// dllist_t * dllist_last(dllist_t * list);

		dllist_t * l = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };

		l = dllist_create(NULL,NULL);
		assert(l != NULL);

		assert(dllist_push_back(l, (void *)&(a[4])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[3])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[2])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[1])) != NULL);
		assert(dllist_push_back(l, (void *)&(a[0])) != NULL);

		assert(dllist_value(dllist_last(l)) == (void *)&(a[0]));

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// dllist_t * dllist_next(dllist_t * node);

		dllist_t * l = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };
		dllist_t * n = NULL;
		int i = 0;

		l = dllist_create(NULL,NULL);
		assert(l != NULL);

		for (n = dllist_first(l); n != NULL; n = dllist_next(n), i++) {
			assert(*(int *)dllist_value(n) == a[i]);
		}

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// dllist_t * dllist_prev(dllist_t * node);

		dllist_t * l = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };
		dllist_t * n = NULL;
		int i = 0;

		l = dllist_create(NULL,NULL);
		assert(l != NULL);

		for (n = dllist_last(l); n != NULL; n = dllist_prev(n), i++) {
			assert(*(int *)dllist_value(n) == a[4-i]);
		}

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	{
		// int dllists_are_identical(dllist_t * list1, dllist_t * list2);

		dllist_t * l1 = NULL;
		dllist_t * l2 = NULL;
		dllist_t * l3 = NULL;
		dllist_t * l4 = NULL;
		dllist_t * l5 = NULL;
		dllist_t * l6 = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };
		int b[5] = { 1, 2, 3, 4, 5 };

		l1 = dllist_create(NULL,NULL);
		l2 = dllist_create(NULL,NULL);
		l3 = dllist_create(NULL,NULL);
		l4 = dllist_create(compare,NULL);
		l5 = dllist_create(compare,NULL);

		dllist_push_back(l1, (void *)&(a[0]));
		dllist_push_back(l2, (void *)&(a[0]));
		dllist_push_front(l3, (void *)&(a[0]));
		dllist_push_back(l4, (void *)&(b[0]));
		dllist_push_back(l5, (void *)&(b[0]));
		dllist_push_front(l6, (void *)&(a[0]));

		dllist_push_back(l1, (void *)&(a[1]));
		dllist_push_back(l2, (void *)&(a[1]));
		dllist_push_front(l3, (void *)&(a[1]));
		dllist_push_back(l4, (void *)&(b[1]));
		dllist_push_back(l5, (void *)&(b[1]));
		dllist_push_front(l6, (void *)&(a[1]));

		dllist_push_back(l1, (void *)&(a[2]));
		dllist_push_back(l2, (void *)&(a[2]));
		dllist_push_front(l3, (void *)&(a[2]));
		dllist_push_back(l4, (void *)&(b[2]));
		dllist_push_back(l5, (void *)&(b[2]));
		dllist_push_front(l6, (void *)&(a[2]));

		dllist_push_back(l1, (void *)&(a[3]));
		dllist_push_back(l2, (void *)&(a[3]));
		dllist_push_front(l3, (void *)&(a[3]));
		dllist_push_back(l4, (void *)&(b[3]));
		dllist_push_back(l5, (void *)&(b[3]));
		dllist_push_front(l6, (void *)&(a[3]));

		dllist_push_back(l1, (void *)&(a[4]));
		dllist_push_back(l2, (void *)&(a[4]));
		dllist_push_front(l3, (void *)&(a[4]));
		dllist_push_back(l4, (void *)&(b[4]));
		dllist_push_back(l5, (void *)&(b[4]));
		dllist_push_front(l6, (void *)&(a[4]));

		assert(dllists_are_identical(l1, l2));
		assert(!dllists_are_identical(l1, l3));
		assert(dllists_are_identical(l4, l5));
		assert(!dllists_are_identical(l4, l6));

		l1 = dllist_destroy(l1);
		l2 = dllist_destroy(l2);
		l3 = dllist_destroy(l3);
		l4 = dllist_destroy(l4);
		l5 = dllist_destroy(l5);
		l6 = dllist_destroy(l6);
	}

	{
		// size_t dllist_size(dllist_t * list);

		dllist_t * l = NULL;
		int a[5] = { 1, 2, 3, 4, 5 };
		dllist_t * n = NULL;

		l = dllist_create(NULL,NULL);
		assert(l != NULL);

		assert(dllist_size(l) == 0);
		assert(dllist_push_back(l, (void *)&(a[4])) != NULL);
		assert(dllist_size(l) == 1);
		assert(dllist_push_back(l, (void *)&(a[3])) != NULL);
		assert(dllist_size(l) == 2);
		assert(dllist_push_back(l, (void *)&(a[2])) != NULL);
		assert(dllist_size(l) == 3);
		assert(dllist_push_back(l, (void *)&(a[1])) != NULL);
		assert(dllist_size(l) == 4);
		assert(dllist_push_back(l, (void *)&(a[0])) != NULL);
		assert(dllist_size(l) == 5);

		n = dllist_first(l);
		n = dllist_next(n);
		n = dllist_next(n);
		assert(*(int *)(dllist_value(n)) == a[2]);
		assert(dllist_remove(n));
		assert(dllist_size(l) == 4);

		n = dllist_first(l);
		assert(*(int *)dllist_value(n) == a[4]);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == a[3]);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == a[1]);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == a[0]);
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_first(l);
		assert(*(int *)(dllist_value(n)) == a[4]);
		assert(dllist_remove(n));
		assert(dllist_size(l) == 3);

		n = dllist_first(l);
		assert(*(int *)dllist_value(n) == a[3]);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == a[1]);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == a[0]);
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l);
		assert(*(int *)(dllist_value(n)) == a[0]);
		assert(dllist_remove(n));
		assert(dllist_size(l) == 2);

		n = dllist_first(l);
		assert(*(int *)dllist_value(n) == a[3]);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == a[1]);
		n = dllist_next(n);
		assert(n == NULL);

		assert(dllist_remove(dllist_first(l)));
		assert(dllist_size(l) == 1);
		assert(dllist_remove(dllist_first(l)));
		assert(dllist_size(l) == 0);

		l = dllist_destroy(l);
		assert(l == NULL);
	}

	// TODO: Test dllist_free
	// TODO: Test dllist_clear

	{
		dllist_t * l1 = NULL;
		dllist_t * l2 = NULL;
		dllist_t * n = NULL;
		int a[10] = { 5, 2, 9, 3, 7, 1, 8, 4, 6, 0 };
		
		l1 = dllist_create(compare, dllist_free_node);
		assert(l1 != NULL);
		l2 = dllist_create(compare, dllist_free_node);
		assert(l2 != NULL);

		dllist_push_back(l1, (void *)dup_int(a[0]));
		dllist_push_back(l1, (void *)dup_int(a[1]));
		dllist_push_back(l1, (void *)dup_int(a[2]));
		dllist_push_back(l1, (void *)dup_int(a[3]));
		dllist_push_back(l1, (void *)dup_int(a[4]));
		dllist_push_back(l1, (void *)dup_int(a[5]));
		dllist_push_back(l1, (void *)dup_int(a[6]));
		dllist_push_back(l1, (void *)dup_int(a[7]));
		dllist_push_back(l1, (void *)dup_int(a[8]));
		dllist_push_back(l1, (void *)dup_int(a[9]));

		assert(dllist_size(l1) == 10);
		assert(dllist_size(l2) == 0);

		n = dllist_first(l1);
		assert(*(int *)dllist_value(n) == 5);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 2);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 9);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 3);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 7);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 1);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 8);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 4);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 6);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 0);
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l1);
		assert(*(int *)dllist_value(n) == 0);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 6);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 4);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 8);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 1);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 7);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 3);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 9);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 2);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 5);
		n = dllist_prev(n);
		assert(n == NULL);

		n = dllist_first(l2);
		assert(n == NULL);

		n = dllist_first(l1);
		dllist_detach_node(l1, n);
		dllist_attach_node(l2, n);

		assert(dllist_size(l1) == 9);
		assert(dllist_size(l2) == 1);

		n = dllist_first(l1);
		assert(*(int *)dllist_value(n) == 2);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 9);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 3);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 7);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 1);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 8);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 4);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 6);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 0);
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l1);
		assert(*(int *)dllist_value(n) == 0);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 6);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 4);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 8);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 1);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 7);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 3);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 9);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 2);
		n = dllist_prev(n);
		assert(n == NULL);

		n = dllist_first(l2);
		assert(*(int *)dllist_value(n) == 5);
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l2);
		assert(*(int *)dllist_value(n) == 5);
		assert(dllist_sentinel(n) == l2);
		n = dllist_prev(n);
		assert(n == NULL);

		n = dllist_last(l1);
		dllist_detach_node(l1, n);
		dllist_attach_node(l2, n);

		assert(dllist_size(l1) == 8);
		assert(dllist_size(l2) == 2);

		n = dllist_first(l1);
		assert(*(int *)dllist_value(n) == 2);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 9);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 3);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 7);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 1);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 8);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 4);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 6);
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l1);
		assert(*(int *)dllist_value(n) == 6);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 4);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 8);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 1);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 7);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 3);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 9);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 2);
		n = dllist_prev(n);
		assert(n == NULL);

		n = dllist_first(l2);
		assert(*(int *)dllist_value(n) == 5);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 0);
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l2);
		assert(*(int *)dllist_value(n) == 0);
		assert(dllist_sentinel(n) == l2);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 5);
		assert(dllist_sentinel(n) == l2);
		n = dllist_prev(n);
		assert(n == NULL);

		n = dllist_first(l1);
		n = dllist_next(n);
		n = dllist_next(n);
		dllist_detach_node(l1, n);
		dllist_attach_node(l2, n);

		assert(dllist_size(l1) == 7);
		assert(dllist_size(l2) == 3);

		n = dllist_first(l1);
		assert(*(int *)dllist_value(n) == 2);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 9);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 7);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 1);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 8);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 4);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 6);
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l1);
		assert(*(int *)dllist_value(n) == 6);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 4);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 8);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 1);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 7);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 9);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 2);
		n = dllist_prev(n);
		assert(n == NULL);

		n = dllist_first(l2);
		assert(*(int *)dllist_value(n) == 5);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 0);
		n = dllist_next(n);
		assert(*(int *)dllist_value(n) == 3);
		n = dllist_next(n);
		assert(n == NULL);

		n = dllist_last(l2);
		assert(*(int *)dllist_value(n) == 3);
		assert(dllist_sentinel(n) == l2);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 0);
		assert(dllist_sentinel(n) == l2);
		n = dllist_prev(n);
		assert(*(int *)dllist_value(n) == 5);
		assert(dllist_sentinel(n) == l2);
		n = dllist_prev(n);
		assert(n == NULL);

		dllist_destroy(l1);
		dllist_destroy(l2);

	}

	{
		dllist_t * l = NULL;
		int c = 0;
		dllist_t * n = NULL;
		dllist_t * last_n = NULL;
		int i1 = 0;
		int i2 = 0;

		l = dllist_create(compare, dllist_free_node);
		assert(l != NULL);

		srand48(0);
		for (c = 0; c < 12; c++) {
			int * i = NULL;

			i = (int *)malloc(sizeof(int));
			*i = lrand48() % 256;

			dllist_push_back(l, (void *)i);
		}

		assert(dllist_sort(l));

		n = dllist_first(l);
		last_n = n;
		n = dllist_next(n);
		while (n != NULL) {
			i1 = *(int *)dllist_value(last_n);
			i2 = *(int *)dllist_value(n);
			assert(i1 <= i2);
			last_n = n;
			n = dllist_next(n);
		}

		dllist_destroy(l);
	}

	{
		dllist_t * l = NULL;
		int c = 0;
		dllist_t * n = NULL;
		dllist_t * last_n = NULL;
		int i1 = 0;
		int i2 = 0;

		l = dllist_create(compare, dllist_free_node);
		assert(l != NULL);

		srand48(0);
		for (c = 0; c < 10000; c++) {
			int * i = NULL;

			i = (int *)malloc(sizeof(int));
			*i = lrand48();

			dllist_push_back(l, (void *)i);
		}

		assert(dllist_sort(l));
		assert(dllist_size(l) == 10000);

		n = dllist_first(l);
		last_n = n;
		n = dllist_next(n);
		while (n != NULL) {
			i1 = *(int *)dllist_value(last_n);
			i2 = *(int *)dllist_value(n);
			assert(i1 <= i2);
			last_n = n;
			n = dllist_next(n);
		}

		dllist_destroy(l);
	}

	return(0);
}

