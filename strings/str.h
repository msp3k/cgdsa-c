/* String
 * by Michael Peek
 * Date: Mon, 28 Apr 2014 15:38:25 -0400
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

#ifndef __STR_H__
#define __STR_H__

#include <stdio.h>
#include "dllist.h"

/** Create a new string
 *
 * Arguments:
 * - char * src -- A pointer to a string to use for initialization.  The
 *     contents of src will be copied into the newly-created string.  If src
 *     is a pointer to a location within a string, then only the remaining
 *     characters starting at src will be copied.  If src is NULL, then the
 *     newly-created string is initialized to "\0".
 *
 * Returns:
 * - Success: char * pointer to the new string
 * - Failure: NULL
 *
 * NOTES:
 * - Calls str_reserve() for memory allocation, see notes for str_reserve()
 * - The contents of src are copied into the newly-created string.
 * - If src is a pointer to a location within a string, then only the
 *     remaining characters from src to the end of the string are copied.
 * - If src is NULL, then the newly-created string is initialized to "\0".
 *
 */
char * str_create(char * src);

/** Destroy a string, freeing it's memory
 *
 * Arguments:
 * - char * str -- A pointer to the string, or a location within a string.
 *
 * Returns: char * -- NULL
 *
 * NOTES:
 * - Only use str_destroy() on strings created with str_*() routines.
 * - If str is NULL then there is no effect.
 * - If str is a pointer to a location within the string, then the head of the
 *     string is found first so that the entire string can be destroyed.
 */
char * str_destroy(char * str);

/** Return the length of the string
 *
 * Arguments:
 * - char * str -- A pointer to a location within a string
 *
 * Return value:
 * - Success: size_t number of characters in the string, excluding
 *     the terminating '\0'.
 * - Failure: 0
 *
 * NOTES:
 * - If str is not the head of the string, then only the remaining characters
 *     between str and the end of the string will be counted.
 */
size_t str_len(char * str);

/** Return the number of bytes that the string can hold, including the
 * terminating '\0'
 *
 * Arguments:
 * - char * str -- A pointer to a location within a string
 *
 * Return value:
 * - Success: size_t number of bytes
 * - Failure: 0
 *
 * NOTES: 
 * - Only use str_capacity() on strings created with str_*() routines.
 * - Whether or not str is the head of the string or a location within the
 *     string makes no difference.
 */
size_t str_capacity(char * str);

/** Increase the capacity of a string to at least size bytes
 *
 * Arguments:
 * - char * str -- A pointer to a location within a string
 * - size_t size -- The number of bytes to reserve
 *
 * Return value:
 * - Success: char * -- A pointer to the head of the string (whose memory
 *     location may have changed during memory reallocation)
 * - Failure: NULL
 *
 * NOTES:
 * - Only use str_reserve() for strings created with the str_*() routines.
 * - Whether or not str is the head of the string or a location within the
 *     string makes no difference.
 * - Memory (re)allocation for strings follows an exponential allocation
 *     strategy.  This is a trade-off between memory usage and speed.  Each
 *     time the string asks for an increased reserve, the amount of memory
 *     allocated doubles.
 */
char * str_reserve(char * str, size_t size);

/** Shrink a string's capacity
 *
 * Arguments:
 * - char * str -- A pointer to a location within a string.
 *
 * Return value:
 * - Success: char * pointer to the head of the string (whose memory
 *     location may have changed during memory reallocation)
 * - Failure: NULL
 *
 * NOTES:
 * - Only use str_reserve() for strings created with the str_*() routines.
 * - Whether or not str is the head of the string or a location within the
 *     string makes no difference.
 * - Calls str_create(), see notes for str_create().
 */
char * str_shrink_reserve(char * str);

/** Clear the contents of a string
 *
 * Arguments:
 * - char * str -- A pointer to a location within a string.
 *
 * Return value: none
 *
 * NOTES:
 * - If str is a location within a string, then the string is truncated at
 *     that location.
 */
void str_clear(char * src);

/** Assign a value to a string.
 *
 * Arguments:
 * - char * dst -- A pointer to a location within the destination string
 * - char * src -- A pointer to a location within the source string
 * 
 * Return value:
 * - Success: char * -- A pointer to the head of dst (whose memory
 *     location may have changed due to (re)allocation)
 * - Failure: NULL
 *
 * NOTES:
 * - If src is NULL, the procedure fails and NULL is returned.
 * - If dst is NULL, dst will be allocated.
 * - If dst is a location within a string, then the rest of the string is
 *     truncated at dst, and replaced with the contents of src.
 * - Uses memmove(), so src and dst may overlap.
 */
char * str_assign(char * dst, char * src);

/** Insert one string into another
 *
 * Arguments:
 * - char * dst -- A pointer to a location within the destination string
 * - char * src -- A pointer to a location within the source string
 *
 * Return value:
 * - Success: char * -- A pointer to the head of dst (whose memory
 *     location may have changed due to (re)allocation)
 * - Failure: NULL
 *
 * NOTES:
 * - If src is NULL, the procedure fails and NULL is returned.
 * - If dst is NULL, dst will be allocated.
 * - If dst is a location within a string, then insertion will begin at dst
 * - If src is a location within a string, then only the characters from src
 *     to the end of src will be inserted
 * - Uses memmove(), so src and dst may overlap.
 */
char * str_insert(char * dst, char * src);

/** Erase characters from a string
 *
 * Arguments:
 * - char * str -- A pointer to a location within a string
 * - size_t len -- The number of characters to remove
 *
 * Return value:
 * - Success: char * -- A pointer to the head of str
 * - Failure: NULL
 */
char * str_erase(char * str, size_t len);

/** Replace characters in one string with the contents of another
 *
 * Arguments:
 * - char * dst -- A pointer to a location within the destination string
 * - size_t len -- The number of characters in dst to replace
 * - char * src -- A pointer to a location within the source string
 *
 * Return value:
 * - char * -- A pointer to the head of dst
 */
char * str_replace(char * dst, size_t len, char * src);

/** Perform a forward search for string needle in string haystack
 *
 * Arguments:
 * - char * haystack -- A pointer to a location within a string
 * - char * needle -- A pointer to a search string
 *
 * Return value:
 * - Success: char * -- A pointer to the first occurence of needle in haystack
 * - Failure: NULL
 */
char * str_find(char * haystack, char * needle);

/** Perform a forward search for string needle in string haystack, ignore case
 *
 * Arguments:
 * - char * haystack -- A pointer to a location within a string
 * - char * needle -- A pointer to a search string
 *
 * Return value:
 * - Success: char * -- A pointer to the first occurence of needle in haystack
 * - Failure: NULL
 */
char * str_caseless_find(char * haystack, char * needle);

/** Perform a reverse search for string needle in string haystack
 *
 * Arguments:
 * - char * haystack -- A pointer to a location within a string
 * - char * needle -- A pointer to a search string
 *
 * Return value:
 * - Success: char * -- A pointer to the last occurence of needle in haystack
 * - Failure: NULL
 */
char * str_rfind(char * haystack, char * needle);

/** Perform a reverse search for string needle in string haystack, ignore case
 *
 * Arguments:
 * - char * haystack -- A pointer to a location within a string
 * - char * needle -- A pointer to a search string
 *
 * Return value:
 * - Success: char * -- A pointer to the last occurence of needle in haystack
 * - Failure: NULL
 */
char * str_caseless_rfind(char * haystack, char * needle);

/** Perform a forward search of haystack for any character in needles
 *
 * Arguments:
 * - char * haystack -- A pointer to a location within a string
 * - char * needles -- A pointer to a string of characters
 *
 * Return value:
 * - Success: char * -- A pointer to the first character in haystack that is
 *     also found in needles found.  Search begins at haystack, and the return
 *     value is a pointer location greater than or equal to haystack.
 * - Failure: NULL
 */
char * str_find_char(char * haystack, char * needles);

/** Perform a reverse search of haystack for any chacter in needles
 *
 * Arguments:
 * - char * haystack -- A pointer to a location within a string
 * - char * needles -- A pointer to a sting of characters
 *
 * Return value:
 * - Success: char * -- A pointer to the first character found in haystack
 *     that is also in needles.  Search begins at haystack, and the return
 *     value is a pointer location less than or equal to haystack.
 * - Failure: NULL
 */
char * str_rfind_char(char * haystack, char * needles);

/** Perform a forward search of haystack for any character not in needles
 *
 * Arguments:
 * - char * haystack -- A pointer to a location within a string
 * - char * needles -- A pointer to a sting of characters
 *
 * Return value:
 * - Success: char * -- A pointer to the first character in haystack that is
 *     not found in needles.  Search begins at haystack, and the return value
 *     is a pointer location greater than or equal to haystack.
 * - Failure: NULL
 */
char * str_find_not_char(char * haystack, char * needles);

/** Perform a forward search of haystack for any character not in needles
 *
 * Arguments:
 * - char * haystack -- A pointer to a location within a string
 * - char * needles -- A pointer to a sting of characters
 *
 * Return value:
 * - Success: char * -- A pointer to the first character in haystack that is
 *     not found in needles.  Search begins at haystack, and the return value
 *     is a pointer location greater than or equal to haystack.
 * - Failure: NULL
 */
char * str_rfind_not_char(char * heystack, char * needles);

/** Create and return a new string containing a substring of src
 *
 * Arguments:
 * - char * src -- A pointer to a location within a string
 * - size_t len -- The number of characters in substr.
 *
 * Return value:
 * - Success: char * -- A pointer to a newly-created string.
 * - Failure: NULL
 */
char * str_substr(char * src, size_t len);

/** Compare two strings
 *
 * Arguments:
 * - char * str1 -- A pointer to a location within a string
 * - char * str2 -- A pointer to a location within a string
 *
 * Return value:
 * - int < 0 -- str1 < str2
 * - int == 0 -- str1 == str2
 * - int > 0 -- str1 > str2
 *
 * NOTES:
 * - Mixing str_*() functions with string functions from the standard C
 *   library may cause memory leaks and seg-faults.  This is why it is
 *   encouraged that the user not intermix the use of str_*() functions with
 *   string functions from the standard C library.  This function is supplied
 *   as a convenience wrapper for strcasecmp.
 *
 */
int str_compare(char * str1, char * str2);

/** Compare two strings, ignore case
 *
 * Arguments:
 * - char * str1 -- A pointer to a location within a string
 * - char * str2 -- A pointer to a location within a string
 *
 * Return value:
 * - int < 0 -- str1 < str2
 * - int == 0 -- str1 == str2
 * - int > 0 -- str1 > str2
 *
 * NOTES:
 * - Mixing str_*() functions with string functions from the standard C
 *   library may cause memory leaks and seg-faults.  This is why it is
 *   encouraged that the user not intermix the use of str_*() functions with
 *   string functions from the standard C library.  This function is supplied
 *   as a convenience wrapper for strcasecmp.
 *
 */
int str_caseless_compare(char * str1, char * str2);

/** Swap the contents of two strings
 *
 * Arguments:
 * - char ** str1 -- A pointer to a char *
 * - char ** str2 -- A pointer to a char *
 *
 * Return value:
 * - Success: 1
 * - Failure: 0
 */
int str_swap(char ** str1, char ** str2);

/** Append one string to the end of another
 *
 * Arguments:
 * - char * dst -- A pointer to a location within a string
 * - char * src -- A pointer to a location within a string
 *
 * Return value:
 * - Success: char * -- A pointer to the head of dst
 * - Failure: NULL
 */
char * str_append(char * dst, char * src);

/** Prepend one string to the beginning of another
 *
 * Arguments:
 * - char * dst -- A pointer to a location within a string
 * - char * src -- A pointer to a location within a string
 *
 * Return value:
 * - Success: char * -- A pointer to the head of dst
 * - Failure: NULL
 */
char * str_prepend(char * dst, char * src);

/** Read input from a stream into a string
 *
 * Arguments:
 * - char ** str -- A pointer to a char *
 * - FILE * fin -- A pointer to a file stream
 * - char * delimiters -- A pointer to a string of delimiter characters
 *
 * Return value:
 * - Success: size_t -- The number of characters read in from fin
 * - Failure: 0
 */
size_t str_getline(char ** str, FILE * fin, char * delimiters);

/** Split a string into a list
 *
 * Arguments:
 * - char * src -- A pointer to a location within a source string
 * - char * delimiters -- A list of delimiter characters
 * - size_t max_split -- The maximum number of splits to make
 *
 * Return value:
 * - Success: dllist_t * -- A pointer to a list containing new strings
 * - Failure: NULL
 *
 * NOTES:
 * - Substrings split from src are copied.  This means that it is safe to
 *   alter or destroy src without changing the contents of the list.
 * - If the number of splits possible are greater than max_split, then the
 *   last string in the list will contain the remainder of the line.
 */
dllist_t * str_split(char * src, char * delimiters, size_t max_split);

/** Join a list of strings together
 *
 * Arguments:
 * - dllist_t * list -- A pointer to a list of strings
 * - char * separator -- A string of characters to use as separators between
 *     each string in the list
 *
 * Return value:
 * - Success: char * -- A pointer to a new string
 * - Failure: NULL
 *
 * NOTES:
 * - The newly-constructed string is a copy.  This means that it is safe to
 *   alter or destroy the list and the separator without harming the returned
 *   sring.
 */
char * str_join(dllist_t * list, char * separator);

#endif
