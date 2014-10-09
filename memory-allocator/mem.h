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
