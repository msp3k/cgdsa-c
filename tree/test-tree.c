#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "tree.h"

int __write_node_key(FILE * fout, void * key)
{
	int i = 0;

	i = *(int *)key;
	fprintf(fout, "%d", i);
	return(1);
}

int __write_node_value_char(FILE * fout, void * value)
{
	char * s = (char *)value;

	fprintf(fout, "%s", s);
	return(1);
}

int __write_node_value_double(FILE * fout, void * value)
{
	double * d = (double *)value;

	fprintf(fout, "%5.2g", *d);
	return(1);
}

int compare(const void * a1, const void * a2)
{
	int i1 = *((const int *)a1);
	int i2 = *((const int *)a2);

	if (i1 < i2) return(-1);
	if (i1 > i2) return(1);
	return(0);
}

void test_tree(tree_t * t, int s, int k[], double d[], unsigned long long b)
{
	int j;

	// Test each set bit to see that it has an associated node in the tree
	for (j = 0; j < s; j++) {
		if (b & (1 << j)) {
			// k[j] should be in the tree
			trnode_t * n = NULL;

			// DEBUG_LINE("j=%d, k[j]=%d\n", j, k[j]);
			// DEBUG_LINE("(void *)&k[j] = 0x%lx\n", (void *)&k[j]);
			n = tree_find(t, (void *)&k[j]);
			assert(n != NULL);
			assert(*(int *)tree_key(n) == k[j]);
			assert(*(double *)tree_value(n) == d[j]);
		}
		else {
			// k[j] should not be in the tree
			trnode_t * n = NULL;

			n = tree_find(t, (void *)&k[j]);
			assert(n == NULL);
		}
	}

	// Test each node in the tree to see that it has an associated set bit
	trnode_t * n;

	n = tree_in_order_first(t);
	while (n != NULL) {
		j = *(int *)tree_key(n);
		assert((b & (1 << j)) != 0);
		n = tree_in_order_next(n);
	}
}

void test_delete_permutation(
	tree_t * t, int s, int *k, double *d, int index, unsigned long long b)
{
	trnode_t * n = NULL;
	tree_t * tclone = NULL;

	if (index == 0) return;

	test_tree(t, s, k, d, b);

	// Can we choose this bit?
	if (b & (1 << (index - 1))) {
		tclone = tree_clone(t);
		assert(tclone != NULL);

		n = tree_find(tclone, (void *)&k[index-1]);
		assert(n != NULL);
		assert(tree_remove(tclone, n) != 0);
		n = NULL;

		test_tree(t, s, k, d, b ^ (1 << (index-1)));

		test_delete_permutation(t, s, k, d, index-1, b ^ (1 << (index-1)));
		
		tclone = tree_destroy(tclone);
	}

	// Choosing not to pick this bit
	test_delete_permutation(t, s, k, d, index-1, b);
}

void test_insert_permutation(
	tree_t *t, int s, int *k, double *d, int index, int bits_left, 
	unsigned long long b)
{
	trnode_t * n;

	if (index == 0) {
		// DEBUG_LINE("bits = ");
		// DEBUG_PRINT_BINARY(b, 10);
		test_delete_permutation(t, s, k, d, index, b);
		return;
	}

	test_tree(t, s, k, d, b);

	// Can we choose not to pick this bit?
	if (index - 1 >= bits_left) {
		test_insert_permutation(t, s, k, d, index - 1, bits_left, b);
	}

	// Picking this bit
	if (bits_left > 0)  {
		assert(tree_insert(t, (void *)&k[index-1], (void *)&d[index-1]) != 0);
		n = NULL;
		n = tree_find(t, (void *)&k[index-1]);
		assert(n != NULL);
		test_tree(t, s, k, d, b | (1 << (index - 1)));
		test_insert_permutation(t, s, k, d, index - 1, bits_left - 1, b | (1 << (index - 1)));
		n = NULL;
		n = tree_find(t, (void *)&k[index-1]);
		assert(n != NULL);
		assert(tree_remove(t, n) != 0);
		n = NULL;
		n = tree_find(t, (void *)&k[index-1]);
		assert(n == NULL);
		test_tree(t, s, k, d, b);
	}
}

void test_001(void)
{
	tree_t * t = NULL;
	// int s = 64; // <-- causes problems
	int s = 16; // <-- A more reasonable waiting time
	int k[64] = {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
	              10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	              20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	              30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	              40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	              50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
	              60, 61, 62, 63 };
	double d[64] = {  0.4,  1.4,  2.4,  3.4,  4.4,  5.4,  6.4,  7.4,  8.4,  9.4,
	                 10.4, 11.4, 12.4, 13.4, 14.4, 15.4, 16.4, 17.4, 18.4, 19.4,
	                 20.4, 21.4, 22.4, 23.4, 24.4, 25.4, 26.4, 27.4, 28.4, 29.4,
	                 30.4, 31.4, 32.4, 33.4, 34.4, 35.4, 36.4, 37.4, 38.4, 39.4,
	                 40.4, 41.4, 42.4, 43.4, 44.4, 45.4, 46.4, 47.4, 48.4, 49.4,
	                 50.4, 51.4, 52.4, 53.4, 54.4, 55.4, 56.4, 57.4, 58.4, 59.4,
	                 60.4, 61.4, 62.4, 63.4 };
	int i;

	t = tree_create(compare, NULL);
	assert(t != NULL);
	for (i = 0; i <= s; i++) {
		// DEBUG_LINE("i = %d\n", i);
		// printf("%d / %d\n", i, s);
		test_insert_permutation(t, s, k, d, s, i, 0);
	}
	t = tree_destroy(t);
}

void test_in_order_traversal(tree_t * t)
{
	int last_i = 0;
	double last_d = 0.0;
	trnode_t * n = NULL;

	n = tree_in_order_first(t);
	last_i = *(int *)tree_key(n);
	last_d = *(double *)tree_value(n);
	while (n != NULL) {
		n = tree_in_order_next(n);
		if (n != NULL) {
			assert(last_i < *(int *)tree_key(n));
			assert(last_d < *(double *)tree_value(n));
			last_i = *(int *)tree_key(n);
			last_d = *(double *)tree_value(n);
		}
	}
	n = tree_in_order_last(t);
	last_i = *(int *)tree_key(n);
	last_d = *(double *)tree_value(n);
	while (n != NULL) {
		n = tree_in_order_prev(n);
		if (n != NULL) {
			assert(last_i > *(int *)tree_key(n));
			assert(last_d > *(double *)tree_value(n));
			last_i = *(int *)tree_key(n);
			last_d = *(double *)tree_value(n);
		}
	}
}

void test_002(void)
{
	int i;
	tree_t * t = NULL;
	int k[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	double d[10] = { 0.00, 1.11, 2.22, 3.33, 4.44, 5.55, 6.66, 7.77, 8.88, 9.99 };
	trnode_t * n = NULL;

	t = tree_create(compare, NULL);
	assert(t != NULL);
	assert(tree_insert(t, (void*)&k[5], (void*)&d[5]));
	assert(tree_insert(t, (void*)&k[3], (void*)&d[3]));
	assert(tree_insert(t, (void*)&k[2], (void*)&d[2]));
	assert(tree_insert(t, (void*)&k[4], (void*)&d[4]));
	assert(tree_insert(t, (void*)&k[7], (void*)&d[7]));
	assert(tree_insert(t, (void*)&k[6], (void*)&d[6]));
	assert(tree_insert(t, (void*)&k[8], (void*)&d[8]));
	// debug_print_node(t, 0, 0);

	test_in_order_traversal(t);
	for (i = 2; i < 9; i++) {
		tree_t * t2 = NULL;

		// printf("\n");
		// printf("Cloning tree\n");

		t2 = tree_clone(t);
		assert(t2 != NULL);
		assert(trees_are_identical(t, t2));

		n = tree_find(t2, (void*)&k[i]);
		assert(n != NULL);
		assert(tree_remove(t2, n));

		// printf("Deleting node w/ key %d\n", k[i]);
		test_in_order_traversal(t);
		/*
		for (n = tree_in_order_first(t2); n != NULL; n = tree_in_order_next(n)) {
			printf("%d\n", *(int *)tree_key(n);
		}
		for (n = tree_in_order_last(t2); n != NULL; n = tree_in_order_prev(n)) {
			printf("%d\n", *(int *)tree_key(n);
		}
		*/

		t2 = tree_destroy(t2);
	}

	t = tree_destroy(t);
}

void test_003(void)
{
	tree_t * t = NULL;
	int k[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	double d[10] = { 0.00, 1.11, 2.22, 3.33, 4.44, 5.55, 6.66, 7.77, 8.88, 9.99 };
	trnode_t * n = NULL;
	tree_t * t2 = NULL;

	t = tree_create(compare, NULL);
	assert(t != NULL);
	assert(tree_insert(t, (void*)&k[5], (void*)&d[5]));
	assert(tree_insert(t, (void*)&k[3], (void*)&d[3]));
	assert(tree_insert(t, (void*)&k[2], (void*)&d[2]));
	assert(tree_insert(t, (void*)&k[4], (void*)&d[4]));
	assert(tree_insert(t, (void*)&k[7], (void*)&d[7]));
	assert(tree_insert(t, (void*)&k[6], (void*)&d[6]));
	assert(tree_insert(t, (void*)&k[8], (void*)&d[8]));
	// debug_print_node(t, 0, 0);

	test_in_order_traversal(t);

	// tree_write_dot(t, "t1.gv", __write_node_key, NULL);

	t2 = tree_clone(t);
	n = tree_top(t2);
	n = tree_left(n);
	n = tree_right(n);
	assert(tree_rotate_left(n));
	// tree_write_dot(t2, "t2.gv", __write_node_key, NULL);
	test_in_order_traversal(t2);
	t2 = tree_destroy(t2);

	t2 = tree_clone(t);
	n = tree_top(t2);
	n = tree_right(n);
	n = tree_right(n);
	assert(tree_rotate_left(n));
	// tree_write_dot(t2, "t3.gv", __write_node_key, NULL);
	test_in_order_traversal(t2);
	t2 = tree_destroy(t2);

	t2 = tree_clone(t);
	n = tree_top(t2);
	n = tree_right(n);
	n = tree_left(n);
	assert(tree_rotate_right(n));
	// tree_write_dot(t2, "t4.gv", __write_node_key, NULL);
	test_in_order_traversal(t2);
	t2 = tree_destroy(t2);

	t2 = tree_clone(t);
	n = tree_top(t2);
	n = tree_left(n);
	n = tree_left(n);
	assert(tree_rotate_right(n));
	// tree_write_dot(t2, "t5.gv", __write_node_key, NULL);
	test_in_order_traversal(t2);
	t2 = tree_destroy(t2);

	t = tree_destroy(t);
}

void test_004(void)
{
	tree_t * t = NULL;
	int a[17] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	char *b[17] = {
		"5",   // 0
		"*",   // 1
		"a",   // 2
		"+",   // 3
		"10",  // 4
		"*",   // 5
		"b",   // 6
		"/",   // 7
		"14",  // 8
		"*",   // 9
		"a",   // 10
		"exp", // 11
		"4",   // 12
		"-",   // 13
		"28",  // 14
		"*",   // 15
		"b"    // 16
		};
	trnode_t * n = NULL;

	t = tree_create(compare, NULL);
	assert(t != NULL);

	assert(tree_insert(t, (void *)&(a[7]), (void *)b[7]));
	assert(tree_insert(t, (void *)&(a[3]), (void *)b[3]));
	assert(tree_insert(t, (void *)&(a[13]), (void *)b[13]));
	assert(tree_insert(t, (void *)&(a[1]), (void *)b[1]));
	assert(tree_insert(t, (void *)&(a[0]), (void *)b[0]));
	assert(tree_insert(t, (void *)&(a[2]), (void *)b[2]));
	assert(tree_insert(t, (void *)&(a[5]), (void *)b[5]));
	assert(tree_insert(t, (void *)&(a[4]), (void *)b[4]));
	assert(tree_insert(t, (void *)&(a[6]), (void *)b[6]));
	assert(tree_insert(t, (void *)&(a[9]), (void *)b[9]));
	assert(tree_insert(t, (void *)&(a[8]), (void *)b[8]));
	assert(tree_insert(t, (void *)&(a[11]), (void *)b[11]));
	assert(tree_insert(t, (void *)&(a[10]), (void *)b[10]));
	assert(tree_insert(t, (void *)&(a[12]), (void *)b[12]));
	assert(tree_insert(t, (void *)&(a[15]), (void *)b[15]));
	assert(tree_insert(t, (void *)&(a[14]), (void *)b[14]));
	assert(tree_insert(t, (void *)&(a[16]), (void *)b[16]));

	// tree_write_dot(t, "t1.gv", __write_node_key, __write_node_value_char);

	n = tree_pre_order_first(t);
	assert(strcmp((char *)tree_value(n), "/") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "+") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "5") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "10") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "-") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "14") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "exp") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "4") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "28") == 0);
	n = tree_pre_order_next(n);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_pre_order_next(n);
	assert(n == NULL);

	n = tree_pre_order_last(t);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "28") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "4") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "exp") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "14") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "-") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "10") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "5") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "+") == 0);
	n = tree_pre_order_prev(n);
	assert(strcmp((char *)tree_value(n), "/") == 0);
	n = tree_pre_order_prev(n);
	assert(n == NULL);

	n = tree_post_order_first(t);
	assert(strcmp((char *)tree_value(n), "5") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "10") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "+") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "14") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "4") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "exp") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "28") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "-") == 0);
	n = tree_post_order_next(n);
	assert(strcmp((char *)tree_value(n), "/") == 0);
	n = tree_post_order_next(n);
	assert(n == NULL);

	n = tree_post_order_last(t);
	assert(strcmp((char *)tree_value(n), "/") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "-") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "28") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "exp") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "4") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "14") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "+") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "10") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_post_order_prev(n);
	assert(strcmp((char *)tree_value(n), "5") == 0);
	n = tree_post_order_prev(n);
	assert(n == NULL);

	n = tree_in_order_first(t);
	assert(strcmp((char *)tree_value(n), "5") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "+") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "10") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "/") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "14") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "exp") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "4") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "-") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "28") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_in_order_next(n);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_in_order_next(n);
	assert(n == NULL);

	n = tree_in_order_last(t);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "28") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "-") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "4") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "exp") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "14") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "/") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "10") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "+") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_in_order_prev(n);
	assert(strcmp((char *)tree_value(n), "5") == 0);
	n = tree_in_order_prev(n);
	assert(n == NULL);

	// tree_write_gv(t, "t1.gv", __write_node_key, __write_node_value_char);

	n = tree_level_order_first(t);
	assert(strcmp((char *)tree_value(n), "/") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "+") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "-") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "5") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "10") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "14") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "exp") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "28") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_level_order_next(n);
	assert(strcmp((char *)tree_value(n), "4") == 0);
	n = tree_level_order_next(n);
	assert(n == NULL);

	n = tree_level_order_last(t);
	assert(strcmp((char *)tree_value(n), "4") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "28") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "exp") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "14") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "b") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "10") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "a") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "5") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "*") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "-") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "+") == 0);
	n = tree_level_order_prev(n);
	assert(strcmp((char *)tree_value(n), "/") == 0);
	n = tree_level_order_prev(n);
	assert(n == NULL);

	tree_destroy(t);
}

void test_005(void)
{
	tree_t * t1 = NULL;
	int a[17] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	char *b[17] = {
		"5",   // 0
		"*",   // 1
		"a",   // 2
		"+",   // 3
		"10",  // 4
		"*",   // 5
		"b",   // 6
		"/",   // 7
		"14",  // 8
		"*",   // 9
		"a",   // 10
		"exp", // 11
		"4",   // 12
		"-",   // 13
		"28",  // 14
		"*",   // 15
		"b"    // 16
		};
	// tree_t * n = NULL;

	t1 = tree_create(compare, NULL);
	assert(t1 != NULL);

	assert(tree_insert(t1, (void *)&(a[7]), (void *)b[7]));
	assert(tree_insert(t1, (void *)&(a[3]), (void *)b[3]));
	assert(tree_insert(t1, (void *)&(a[13]), (void *)b[13]));
	assert(tree_insert(t1, (void *)&(a[1]), (void *)b[1]));
	assert(tree_insert(t1, (void *)&(a[0]), (void *)b[0]));
	assert(tree_insert(t1, (void *)&(a[2]), (void *)b[2]));
	assert(tree_insert(t1, (void *)&(a[5]), (void *)b[5]));
	assert(tree_insert(t1, (void *)&(a[4]), (void *)b[4]));
	assert(tree_insert(t1, (void *)&(a[6]), (void *)b[6]));
	assert(tree_insert(t1, (void *)&(a[9]), (void *)b[9]));
	assert(tree_insert(t1, (void *)&(a[8]), (void *)b[8]));
	assert(tree_insert(t1, (void *)&(a[11]), (void *)b[11]));
	assert(tree_insert(t1, (void *)&(a[10]), (void *)b[10]));
	assert(tree_insert(t1, (void *)&(a[12]), (void *)b[12]));
	assert(tree_insert(t1, (void *)&(a[15]), (void *)b[15]));
	assert(tree_insert(t1, (void *)&(a[14]), (void *)b[14]));
	assert(tree_insert(t1, (void *)&(a[16]), (void *)b[16]));

	// tree_write_gv(t1, "t1.gv", __write_node_key, __write_node_value_char);

	// printf("\n");
	// printf("t1 :: Size: %ld\n", tree_size(t1));
	// printf("t1 :: Max Depth: %d\n", tree_max_depth(t1));
	// printf("t1 :: Min Depth: %d\n", tree_min_depth(t1));
	// printf("t1 :: Is full: %s\n", tree_is_full(t1) ? "yes" : "no");
	// printf("t1 :: Is perfect: %s\n", tree_is_perfect(t1) ? "yes" : "no");
	// printf("t1 :: Is degenerate: %s\n", tree_is_degenerate(t1) ? "yes" : "no");
	// printf("t1 :: Is balanced: %s\n", tree_is_balanced(t1) ? "yes" : "no");
	// printf("t1 :: Is complete: %s\n", tree_is_complete(t1) ? "yes" : "no");
	assert(tree_size(t1) == 17);
	assert(tree_max_depth(tree_top(t1)) == 5);
	assert(tree_min_depth(tree_top(t1)) == 4);
	assert(tree_is_full(t1) == 1);
	assert(tree_is_degenerate(t1) == 0);
	assert(tree_is_balanced(t1) == 1);
	assert(tree_is_complete(t1) == 0);

	assert(tree_max_depth(tree_top(t1)) == 5);
	assert(tree_min_depth(tree_top(t1)) == 4);
	assert(tree_is_full(t1) == 1);

	tree_t * t2 = NULL;
	int a2[5] = { 0, 1, 2, 3, 4 };
	t2 = tree_create(compare, NULL);
	assert(t2 != NULL);

	assert(tree_insert(t2, (void *)&(a2[5]), NULL) != 0);
	assert(tree_insert(t2, (void *)&(a2[4]), NULL) != 0);
	assert(tree_insert(t2, (void *)&(a2[3]), NULL) != 0);
	assert(tree_insert(t2, (void *)&(a2[2]), NULL) != 0);
	assert(tree_insert(t2, (void *)&(a2[1]), NULL) != 0);
	assert(tree_insert(t2, (void *)&(a2[0]), NULL) != 0);

	// tree_write_dot(t2, "t2.gv", __write_node_key, NULL);

	// printf("\n");
	// printf("t2 :: Size: %ld\n", tree_size(t2));
	// printf("t2 :: Max Depth: %d\n", tree_max_depth(t2));
	// printf("t2 :: Min Depth: %d\n", tree_min_depth(t2));
	// printf("t2 :: Is full: %s\n", tree_is_full(t2) ? "yes" : "no");
	// printf("t2 :: Is perfect: %s\n", tree_is_perfect(t2) ? "yes" : "no");
	// printf("t2 :: Is degenerate: %s\n", tree_is_degenerate(t2) ? "yes" : "no");
	// printf("t2 :: Is balanced: %s\n", tree_is_balanced(t2) ? "yes" : "no");
	// printf("t2 :: Is complete: %s\n", tree_is_complete(t2) ? "yes" : "no");
	assert(tree_size(t2) == 6);
	assert(tree_max_depth(tree_top(t2)) == 6);
	assert(tree_min_depth(tree_top(t2)) == 1);
	assert(tree_is_full(t2) == 0);
	assert(tree_is_degenerate(t2) == 1);
	assert(tree_is_balanced(t2) == 0);
	assert(tree_is_complete(t2) == 0);

	tree_t * t3= NULL;
	int k[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	double d[10] = { 0.00, 1.11, 2.22, 3.33, 4.44, 5.55, 6.66, 7.77, 8.88, 9.99 };

	t3 = tree_create(compare, NULL);
	assert(t3 != NULL);
	assert(tree_insert(t3, (void*)&k[5], (void*)&d[5]));
	assert(tree_insert(t3, (void*)&k[3], (void*)&d[3]));
	assert(tree_insert(t3, (void*)&k[2], (void*)&d[2]));
	assert(tree_insert(t3, (void*)&k[4], (void*)&d[4]));
	assert(tree_insert(t3, (void*)&k[7], (void*)&d[7]));
	assert(tree_insert(t3, (void*)&k[6], (void*)&d[6]));
	assert(tree_insert(t3, (void*)&k[8], (void*)&d[8]));
	// debug_print_node(t3, 0, 0);

	// tree_write_gv(t3, "t3.gv", __write_node_key, __write_node_value_double);

	// printf("\n");
	// printf("t3 :: Size: %ld\n", tree_size(t3));
	// printf("t3 :: Max Depth: %d\n", tree_max_depth(t3));
	// printf("t3 :: Min Depth: %d\n", tree_min_depth(t3));
	// printf("t3 :: Is full: %s\n", tree_is_full(t3) ? "yes" : "no");
	// printf("t3 :: Is perfect: %s\n", tree_is_perfect(t3) ? "yes" : "no");
	// printf("t3 :: Is degenerate: %s\n", tree_is_degenerate(t3) ? "yes" : "no");
	// printf("t3 :: Is balanced: %s\n", tree_is_balanced(t3) ? "yes" : "no");
	// printf("t3 :: Is complete: %s\n", tree_is_complete(t3) ? "yes" : "no");
	assert(tree_size(t3) == 7);
	assert(tree_max_depth(tree_top(t3)) == 3);
	assert(tree_min_depth(tree_top(t3)) == 3);
	assert(tree_is_full(t3) == 1);
	assert(tree_is_degenerate(t3) == 0);
	assert(tree_is_balanced(t3) == 1);
	assert(tree_is_complete(t3) == 1);

	tree_t * t4 = NULL;
	int a3[5] = { 0, 1, 2, 3, 4 };
	t4 = tree_create(compare, NULL);
	assert(t4 != NULL);

	assert(tree_insert(t4, (void *)&(a3[3]), NULL) != 0);
	assert(tree_insert(t4, (void *)&(a3[4]), NULL) != 0);
	assert(tree_insert(t4, (void *)&(a3[1]), NULL) != 0);
	assert(tree_insert(t4, (void *)&(a3[2]), NULL) != 0);
	assert(tree_insert(t4, (void *)&(a3[0]), NULL) != 0);

	// tree_write_dot(t4, "t4.gv", __write_node_key, NULL);

	// printf("\n");
	// printf("t4 :: Size: %ld\n", tree_size(t4));
	// printf("t4 :: Max Depth: %d\n", tree_max_depth(t4));
	// printf("t4 :: Min Depth: %d\n", tree_min_depth(t4));
	// printf("t4 :: Is full: %s\n", tree_is_full(t4) ? "yes" : "no");
	// printf("t4 :: Is perfect: %s\n", tree_is_perfect(t4) ? "yes" : "no");
	// printf("t4 :: Is degenerate: %s\n", tree_is_degenerate(t4) ? "yes" : "no");
	// printf("t4 :: Is balanced: %s\n", tree_is_balanced(t4) ? "yes" : "no");
	// printf("t4 :: Is complete: %s\n", tree_is_complete(t4) ? "yes" : "no");
	assert(tree_size(t4) == 5);
	assert(tree_max_depth(tree_top(t4)) == 3);
	assert(tree_min_depth(tree_top(t4)) == 2);
	assert(tree_is_full(t4) == 1);
	assert(tree_is_degenerate(t4) == 0);
	assert(tree_is_balanced(t4) == 1);
	assert(tree_is_complete(t4) == 1);

	tree_destroy(t1);
	tree_destroy(t2);
	tree_destroy(t3);
	tree_destroy(t4);
}

int main(int argc, char ** argv)
{
	test_001();
	test_002();
	test_003();
	test_004();
	test_005();
	return(0);
}
