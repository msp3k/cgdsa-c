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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "dllist.h"

char * str_reserve(char * str, size_t size)
{
	char * b = (char *)mem_begin(str);
	char * e = (char *)mem_end(str);
	size_t capacity = 0;
	size_t new_capacity = 1;

	if (str != NULL && b != NULL && e != NULL)
		capacity = (size_t)(e-b) + (size_t)(str - b);
	while (new_capacity < capacity || new_capacity < size)
		new_capacity *= 2;
	if (new_capacity <= capacity) return(str);
	str = (char *)mem_realloc(str, new_capacity);
	if(capacity == 0)
		str[0] = '\0';
	return(str);
}

size_t str_len(char * str)
{
	if (str == NULL) return(0);
	return(strlen(str));
}

char * str_create(char * src)
{
	char * new_str = NULL;
	size_t capacity = 1;
	size_t src_len = 0;

	if (src != NULL)
		capacity = str_len(src);
	
	new_str = str_reserve(new_str, capacity);
	if (new_str == NULL) return(NULL);
	new_str[0] = '\0';
	if (src != NULL) {
		src_len = str_len(src);
		memmove(new_str, src, src_len+1);
	}
	return(new_str);
}

char * str_destroy(char * str)
{
	mem_free(str);
	return(NULL);
}

size_t str_capacity(char * str)
{
	return(mem_size(str));
}

char * str_shrink_reserve(char * str)
{
	char * new_str = NULL;
	char * str_begin = mem_begin(str);

	if (str_begin != NULL)
		str = str_begin;
	new_str = str_create(str);
	str_destroy(str);
	return(new_str);
}

void str_clear(char * str)
{
	if (str != NULL)
		str[0] = '\0';
}

char * str_assign(char * dst, char * src)
{
	char * dst_start = NULL;
	size_t src_len = 0;
	size_t dst_offset = 0;

	if (src == NULL) return(NULL);
	src_len = str_len(src);
	dst_start = mem_begin(dst);
	dst_offset = dst - dst_start;
	dst_start = str_reserve(dst_start, dst_offset+src_len+1);
	if (dst_start == NULL) return(NULL);
	dst = dst_start + dst_offset;
	memmove(dst, src, src_len+1);
	return(dst_start);
}

char * str_insert(char * dst, char * src)
{
	char * dst_start = NULL;
	char * src_start = NULL;
	size_t dst_offset = 0;
	size_t src_offset = 0;
	size_t dst_len = 0;
	size_t src_len = 0;
	int dst_and_src_are_same_str = 0;

	if (src == NULL) return(NULL);
	if (dst == NULL) dst = str_create(NULL);
	if (dst == NULL) return(NULL);
	dst_start = mem_begin(dst);
	src_start = mem_begin(src);
	if (dst_start == src_start)
		dst_and_src_are_same_str = 1;
	dst_offset = (size_t)(dst - dst_start);
	src_offset = (size_t)(src - src_start);
	dst_len = str_len(dst);
	src_len = str_len(src);
	dst_start = str_reserve(dst_start, dst_len + (dst - dst_start) + src_len + 1);
	if (dst_start == NULL) return(NULL);
	dst = dst_start + dst_offset;
	if (dst_and_src_are_same_str)
		src = dst_start + src_offset;
	memmove(dst+src_len, dst, dst_len+1);
	memmove(dst, src, src_len);
	return(dst_start);
}

char * str_erase(char * str, size_t len)
{
	size_t len_str = str_len(str);
	char * str_begin = mem_begin(str);

	if (len_str < len)
		len = len_str;
	memmove(str, str+len, len_str+1);
	return(str_begin);
}

char * str_replace(char * dst, size_t len, char * src)
{
	char * dst_start = NULL;
	size_t dst_offset = 0;

	if (src == NULL) return(NULL);
	if (dst == NULL) return(NULL);
	dst_start = mem_begin(dst);
	dst_offset = dst - dst_start;
	dst_start = str_erase(dst, len);
	dst = dst_start + dst_offset;
	dst_start = str_insert(dst, src);
	return(dst_start);
}

char * str_find(char * haystack, char * needle)
{
	char * search_start = NULL;
	char * search_end = NULL;
	char * str = NULL;
	size_t len_needle = 0;

	if (haystack == NULL) return(NULL);
	if (needle == NULL) return(NULL);
	len_needle = str_len(needle);
	search_start = haystack;
	search_end = haystack + str_len(haystack) - len_needle + 1;
	for (str = search_start; str != search_end; str++) {
		if (strncmp(str, needle, len_needle) == 0)
			return(str);
	}
	return(NULL);
}

char * str_caseless_find(char * haystack, char * needle)
{
	char * search_start = NULL;
	char * search_end = NULL;
	char * str = NULL;
	size_t len_needle = 0;

	if (haystack == NULL) return(NULL);
	if (needle == NULL) return(NULL);
	len_needle = str_len(needle);
	search_start = haystack;
	search_end = haystack + str_len(haystack) - len_needle + 1;
	for (str = search_start; str != search_end; str++) {
		if (strncasecmp(str, needle, len_needle) == 0)
			return(str);
	}
	return(NULL);
}

char * str_rfind(char * haystack, char * needle)
{
	char * haystack_begin = mem_begin(haystack);
	char * str = NULL;
	size_t len_needle = 0;
	
	if (haystack == NULL) return(NULL);
	if (needle == NULL) return(NULL);
	len_needle = str_len(needle);
	for (
		str = haystack;
		str >= haystack_begin;
		str--
		)
	{
		if (strncmp(str, needle, len_needle) == 0) return(str);
	}
	return(NULL);

}

char * str_caseless_rfind(char * haystack, char * needle)
{
	char * haystack_begin = mem_begin(haystack);
	char * str = NULL;
	size_t needle_len = 0;
	
	needle_len = str_len(needle);
	for (
		str = haystack;
		str >= haystack_begin;
		str--
		)
	{
		if (strncasecmp(str, needle, needle_len) == 0) return(str);
	}
	return(NULL);
}

char * str_find_char(char * haystack, char * needles)
{
	char * str = NULL;
	size_t len_haystack;

	if (haystack == NULL) return(NULL);
	if (needles == NULL) return(NULL);
	len_haystack = str_len(haystack);
	for (
		str = haystack;
		str < haystack + len_haystack;
		str++
		)
	{
		if (strchr(needles, *str) != NULL) return(str);
	}
	return(NULL);
}

char * str_rfind_char(char * haystack, char * needles)
{
	char * haystack_begin = mem_begin(haystack);
	char * str = NULL;

	if (haystack == NULL) return(NULL);
	if (needles == NULL) return(NULL);
	for (
		str = haystack ;
		str >= haystack_begin;
		str--
		)
	{
		if (strchr(needles, *str) != NULL) return(str);
	}
	return(NULL);
}

char * str_find_not_char(char * haystack, char * needles)
{
	char * str = NULL;
	size_t len_haystack;

	if (haystack == NULL) return(NULL);
	if (needles == NULL) return(NULL);
	len_haystack = str_len(haystack);
	for (
		str = haystack;
		str < haystack + len_haystack;
		str++
		)
	{
		if (strchr(needles, *str) == NULL) return(str);
	}
	return(NULL);
}

char * str_rfind_not_char(char * haystack, char * needles)
{
	char * haystack_begin = mem_begin(haystack);
	char * str = NULL;

	if (haystack == NULL) return(NULL);
	if (needles == NULL) return(NULL);
	for (
		str = haystack ;
		str >= haystack_begin;
		str--
		)
	{
		if (strchr(needles, *str) == NULL) return(str);
	}
	return(NULL);
}

char * str_substr(char * src, size_t len)
{
	char * str = NULL;

	str = str_create(NULL);
	if (str == NULL) return(NULL);
	str = str_reserve(str, len+1);
	if (str == NULL) return(NULL);
	memmove(str, src, len);
	str[len] = '\0';
	return(str);
}

int str_compare(char * str1, char * str2)
{
	int c = strcmp(str1, str2);
	return(c);
}

int str_caseless_compare(char * str1, char * str2)
{
	int c = strcasecmp(str1, str2);
	return(c);
}

int str_swap(char ** str1, char ** str2)
{
	char * tmpstr = NULL;

	tmpstr = str_create(*str1);
	if (*str1 != NULL && tmpstr == NULL) return(0);
	*str1 = str_assign(*str1, *str2);
	*str2 = str_assign(*str2, tmpstr);
	return(1);
}

char * str_append(char * dst, char * src)
{
	char * str = NULL;
	char * dst_begin = mem_begin(dst);
	size_t len_dst = str_len(dst_begin);
	char * dst_end = dst_begin + len_dst;

	str = str_insert(dst_end, src);
	return(str);
}

char * str_prepend(char * dst, char * src)
{
	char * str = NULL;
	char * dst_begin = mem_begin(dst);

	str = str_insert(dst_begin, src);
	return(str);
}

size_t str_getline(char ** str, FILE * fin, char * delimiters)
{
	int ch = 0;
	char s[2] = { '\0', '\0' };
	size_t num = 0;

	while ((ch = fgetc(fin)) != EOF) {
		s[0] = (char)ch;
		if (strchr(delimiters, (char)ch) != NULL) {
			ungetc(ch, fin);
			return(num);
		}
		*str = str_append(*str, s);
		num++;
	}
	return(num);
}

dllist_t * str_split(char * src, char * delimiters, size_t max_split)
{
	dllist_t * l = NULL;
	char * str_b = NULL;
	char * str_e = NULL;
	char * str = NULL;
	size_t c_split = 0;

	if (src == NULL) return(NULL);
	if (delimiters == NULL) return(NULL);
	if (max_split < 1) return(NULL);
	l = dllist_create(NULL, dllist_free_node);
	if (l == NULL) return(NULL);
	str_b = src;
	str_e = src;
	while (str_b != NULL && *str_b != '\0' && c_split < max_split-1) {
		str_e = str_find_char(str_b, delimiters);
		if (str_e == NULL) {
			str = str_create(str_b);
			if (str == NULL) {
				dllist_destroy(l);
				return(NULL);
			}
			if (dllist_push_back(l, (void *)str) == NULL) {
				dllist_destroy(l);
				return(NULL);
			}
		}
		else if (str_e == str_b) {
			return(l);
		}
		else {
			str = str_substr(str_b, (size_t)(str_e - str_b));
			if (str == NULL) {
				dllist_destroy(l);
				return(NULL);
			}
			if (dllist_push_back(l, (void *)str) == NULL) {
				dllist_destroy(l);
				return(NULL);
			}
		}
		c_split++;
		str_e = str_find_not_char(str_e, delimiters);
		str_b = str_e;
	}
	if (str_b != NULL && *str_b != '\0') {
		str = str_create(str_b);
		if (str == NULL) {
			dllist_destroy(l);
			return(NULL);
		}
		if (dllist_push_back(l, (void *)str) == NULL) {
			dllist_destroy(l);
			return(NULL);
		}
	}
	return(l);
}

char * str_join(dllist_t * list, char * separator)
{
	char * str = NULL;
	dllist_t * n = NULL;

	if (list == NULL) return(NULL);
	if (separator == NULL) return(NULL);
	str = str_create(NULL);
	n = dllist_first(list);
	if (n == NULL) return(str);
	str = str_append(str, (char *)dllist_value(n));
	n = dllist_next(n);
	while (n != NULL) {
		if (separator != NULL)
			str = str_append(str, separator);
		str = str_append(str, (char *)dllist_value(n));
		n = dllist_next(n);
	}
	return(str);
}

