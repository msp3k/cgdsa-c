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

#ifndef __MEM_H__
#define __MEM_H__

/** Mem
 *
 * A very simple memory map that tracks memory allocation size.
 *
 */

/** Allocate size bytes of memory 
 *
 * Arguments:
 * - size_t size -- Number of bytes to allocate
 *
 * Return value:
 * - Success: void * -- pointer to memory
 * - Failure: NULL
 */
void * mem_alloc(size_t size);

/** (Re)allocate size bytes of memory
 *
 * Arguments:
 * - void * ptr -- A pointer to a location in memory
 * - size_t size -- Total number of bytes requested
 *
 * Return value:
 * - Succcess: void * -- pointer to the head of the memory segment 
 *     (which may have changed due to (re)allocation)
 * - Failure: NULL
 *
 * NOTES:
 * - If ptr is NULL, then mem_realloc() acts like mem_alloc()
 * - Ptr need not be the head of the memory segment, but may be a pointer to a
 *     location within a memory segment
 */
void * mem_realloc(void * ptr, size_t size);

/** Return the number of bytes allocated for ptr 
 *
 * Arguments:
 * - void * ptr -- A pointer to a location in memory
 *
 * Return value:
 * - Success: size_t value > 0
 * - Failure: 0
 *
 * NOTES:
 * - Ptr need not be the head of the memory segment, but may be a pointer to a
 *     location within a memory segment
 */
size_t mem_size(void * ptr);

/** Return the beginning of the ptr's chunk of memory, if found 
 *
 * Arguments:
 * - void * ptr -- A pointer to a location in memory
 *
 * Return value:
 * - Success: void * -- pointer to the begining of the memory segment
 * - Failure: NULL
 */
void * mem_begin(void * ptr);

/** Return an pointer to the end of ptr's chunk of memory 
 * 
 * Arguments:
 * - void * ptr -- A pointer to a location in memory
 *
 * Return value:
 * - Success: void * -- pointer to the end of the memory segment
 * - Failure: NULL
 */
void * mem_end(void * ptr);

/** Release memory allocated to ptr
 *
 * Arguments:
 * - void * ptr -- A pointer to a location in memory
 *
 * Return value: NULL
 *
 * NOTES:
 * - Ptr need not be the head of the memory segment, but may be a pointer to a
 *     location within a memory segment
 */
void * mem_free(void * ptr);

/** Return the total number of bytes allocated my mem_alloc()/mem_realloc()
 *
 * Arguments: none
 *
 * Return value:
 * - unsigned long long -- The number of bytes currently allocated
 */
unsigned long long mem_total(void);

#endif
