/* A Memory Allocator Wrapper
 * by: Michael Peek
 * Date: Thu, 09 Sep 2014 20:23:11 -0400
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

// CAUTION: MAY CONTAIN BUGS WHEN USED WITH THREADING
// SEE test-mem-pthread.c FOR DETAILS

#ifdef WITH_PTHREAD
#include <pthread.h>
#endif

#include "mem.h"
#include "rbtree.h"

rbtree_t * __mem_map = NULL;
unsigned long long __mem_total = 0;

#ifdef WITH_PTHREAD
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

#define LOCK pthread_mutex_lock(&lock)
#define UNLOCK pthread_mutex_unlock(&lock)
#else
#define LOCK
#define UNLOCK
#endif

int mf__mem_compare(const void * a1, const void * a2)
{
	if (a1 < a2) return(-1);
	if (a1 > a2) return(1);
	return(0);
}

int mf__mem_init(void)
{
	if (__mem_map == NULL) {
		__mem_map = rbtree_create(mf__mem_compare, NULL);
		if (__mem_map == NULL) {
			return(0);
		}
	}
	return(1);
}

void * mf__mem_alloc(size_t size)
{
	void * start = NULL;

	if (!mf__mem_init()) return(NULL);

	start = malloc(size);
	if (start == NULL) {
		perror("malloc()");
		return(NULL);
	}
	__mem_total += size;

	if (!rbtree_insert(__mem_map, start, start + size)) {
		free(start);
		return(NULL);
	}

	return(start);
}

void * mf__mem_begin(void * ptr)
{
	rbnode_t * n = NULL;
	void * b = NULL;
	void * e = NULL;

	if (!mf__mem_init()) return(NULL);

	n = rbtree_top(__mem_map);
	while (n != NULL) {
		b = rbtree_key(n);
		e = rbtree_value(n);
		if (ptr >= b && ptr <= e) return(b);
		if (ptr > e)
			n = rbtree_right(n);
		else
			n = rbtree_left(n);
	}
	return(NULL);
}

void * mf__mem_end(void * ptr)
{
	rbnode_t * n = NULL;

	if (!mf__mem_init()) return(NULL);

	ptr = mf__mem_begin(ptr);
	n = rbtree_find(__mem_map, ptr);
	if (n == NULL) return(NULL);
	return(rbtree_value(n));
}

size_t mf__mem_size(void * ptr)
{
	rbnode_t * n = NULL;
	size_t size = 0;

	if (!mf__mem_init()) return(0);

	ptr = mf__mem_begin(ptr);
	n = rbtree_find(__mem_map, ptr);
	if (n == NULL) return(0);
	size = rbtree_value(n) - ptr;
	return(size);
}

void * mf__mem_free(void * ptr)
{
	rbnode_t * n = NULL;
	void * ptr_begin = mf__mem_begin(ptr);

	if (!mf__mem_init()) return(NULL);

	if (ptr_begin != NULL)
		ptr = ptr_begin;
	__mem_total -= mf__mem_size(ptr);
	n = rbtree_find(__mem_map, ptr);
	if (n != NULL) {
		assert(rbtree_remove(__mem_map, n));
	}
	free(ptr);
	return(NULL);
}

void * mf__mem_realloc(void * ptr, size_t size)
{
	void * start = NULL;

	if (!mf__mem_init()) return(NULL);

	if (size == 0)
		return(mf__mem_free(ptr));

	__mem_total -= mf__mem_size(ptr);
	ptr = mf__mem_begin(ptr);
	start = realloc(ptr, size);
	if (start == NULL) {
		perror("realloc()");
		return(NULL);
	}
	__mem_total += size;

	if (!rbtree_insert(__mem_map, start, start+size)) {
		free(start);
		return(NULL);
	}

	return(start);
}

unsigned long long mf__mem_total(void)
{
	return(__mem_total);
}

void * mem_alloc(size_t size)
{
	void * r_ptr = NULL;

	LOCK;
	r_ptr = mf__mem_alloc(size);
	UNLOCK;
	return(r_ptr);
}

void * mem_begin(void * ptr)
{
	void * r_ptr = NULL;

	LOCK;
	r_ptr = mf__mem_begin(ptr);
	UNLOCK;
	return(r_ptr);
}

void * mem_end(void * ptr)
{
	void * r_ptr = NULL;

	LOCK;
	r_ptr = mf__mem_end(ptr);
	UNLOCK;
	return(r_ptr);
}

void * mem_realloc(void * ptr, size_t size)
{
	void * r_ptr = NULL;

	LOCK;
	r_ptr = mf__mem_realloc(ptr, size);
	UNLOCK;
	return(r_ptr);
}

size_t mem_size(void * ptr)
{
	size_t r_size_t = 0;

	LOCK;
	r_size_t = mf__mem_size(ptr);
	UNLOCK;
	return(r_size_t);
}

void * mem_free(void * ptr)
{
	void * r_ptr = NULL;

	LOCK;
	r_ptr = mf__mem_free(ptr);
	UNLOCK;
	return(r_ptr);
}

unsigned long long mem_total(void)
{
	unsigned long long r_ull = 0;

	LOCK;
	r_ull = mf__mem_total();
	UNLOCK;
	return(r_ull);
}

