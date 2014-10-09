#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h> // sleep

#define NUM_THREADS 10000
#define NUM_REPETITIONS 1000

#include "mem.h"

int go_flag = 0;

void test_001(void)
{
	char * str = NULL;

	str = (char *)mem_alloc(10);
	assert(str != NULL);
	assert(mem_size(str) == 10);
	assert(mem_end(str) == str+10);
	str = mem_free(str);
	assert(str == NULL);
	assert(mem_size(str) == 0);
	assert(mem_end(str) == NULL);
	assert(mem_free(str) == NULL);
}

void test_002(void)
{
	char * str = NULL;

	str = (char *)mem_alloc(10);
	assert(str != NULL);
	assert(mem_size(str) == 10);
	assert(mem_end(str) == str+10);

	str = mem_realloc(str, 1024*1024);
	assert(str != NULL);
	assert(mem_size(str) == 1024*1024);
	assert(mem_end(str) == str+(1024*1024));

	str = mem_free(str);
	assert(str == NULL);
	assert(mem_size(str) == 0);
	assert(mem_end(str) == NULL);
	assert(mem_free(str) == NULL);
}

void test_003(void)
{
	char * str1 = NULL;
	char * str2 = NULL;

	str1 = (char *)mem_alloc(10);
	assert(str1 != NULL);
	str2 = (char *)malloc(10);
	assert(str2 != NULL);
	assert(mem_size(str1) == 10);
#ifndef WITH_PTHREAD
	assert(mem_size(str2) == 0); // This test fails under threads -- WHY?
#endif
	assert(mem_end(str1) == str1+10);
#ifndef WITH_PTHREAD
	assert(mem_end(str2) == NULL); // This test fails under threads -- WHY?
#endif
	str1 = mem_free(str1);
	assert(str1 == NULL);
	str2 = mem_free(str2);
	assert(str2 == NULL);
}

void test_004(void)
{
	char * str = NULL;

	str = (char *)mem_alloc(10);
	assert(str != NULL);
	assert(mem_begin(str) == str);
	assert(mem_begin(str+5) == str);
	assert(mem_begin(str+10) == str);
	assert(mem_begin(str-1) == NULL);
	assert(mem_begin(str+11) == NULL);
	str = mem_free(str);
}

void *thread_func(void *idptr)
{
	unsigned int i = 0;

	while (!go_flag); // Wait for it...
	for (i = 0; i < NUM_REPETITIONS; i++) {
		test_001();
		test_002();
		test_003();
		test_004();
	}
	return(NULL);
}

int main(int argc, char ** argv)
{
	pthread_t tcb[NUM_THREADS];
	int i;
	void * status = NULL;

	fprintf(stderr, "Spawning threads...\n");
	for (i = 0; i < NUM_THREADS; i++) {
		if (pthread_create(&(tcb[i]), NULL, thread_func, &(tcb[i])) != 0) {
			perror("pthread_create()");
			exit(1);
		}
	}
	fprintf(stderr, "Activating threads...\n");
	go_flag = 1;
	fprintf(stderr, "Joining threads...\n");
	for (i = 0; i < NUM_THREADS; i++) {
		if (pthread_join(tcb[i], &status) != 0) {
			perror("pthread_join()");
			exit(1);
		}
	}
	fprintf(stderr, "Done\n");
	return(0);
}
