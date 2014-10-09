#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "mem.h"

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
	assert(mem_size(str2) == 0);
	assert(mem_end(str1) == str1+10);
	assert(mem_end(str2) == NULL);
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

int main(int argc, char ** argv)
{
	test_001();
	test_002();
	test_003();
	test_004();
	return(0);
}
