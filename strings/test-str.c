#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "str.h"

int main(int argc, char ** argv)
{
	char * str1 = NULL;
	char * str2 = NULL;
	char * str = NULL;
	char * str_old = NULL;

	// TEST: str_create()
	assert((str1 = str_create(NULL)) != NULL);
	assert(str_len(str1) == 0);
	assert(str1[0] == '\0');
	assert(str_capacity(str1) == 1);
	assert((str1 = str_destroy(str1)) == NULL);

	assert((str1 = str_create("Hello World")) != NULL);
	assert(strcmp(str1, "Hello World") == 0);
	assert(str_len(str1) == 11);
	assert(str_capacity(str1) == 16);
	assert((str1 = str_destroy(str1)) == NULL);

	assert((str1 = str_create("Hello World")) != NULL);
	assert(strcmp(str1, "Hello World") == 0);
	assert((str2 = str_create(str1+6)) != NULL);
	assert(strcmp(str2, "World") == 0);
	assert(str_len(str1) == 11);
	assert(str_len(str2) == 5);

	assert(strcmp((str1 = str_reserve(str1, 512)), "Hello World") == 0);
	assert(str_len(str1) == 11);
	assert(str_capacity(str1) == 512);
	assert(strcmp((str2 = str_reserve(str2, 1024)), "World") == 0);
	assert(str_len(str2) == 5);
	assert(str_capacity(str2) == 1024);

	assert((str1 = str_destroy(str1)) == NULL);
	assert((str2 = str_destroy(str2)) == NULL);

	// TEST: str_reserve()
	// TEST: str_capacity()
	assert((str = str_reserve(str, 512)) != NULL);
	assert(str_capacity(str) == 512);
	assert((str = str_reserve(str+32, 511)) != NULL);
	assert(str_capacity(str) == 1024);
	assert((str = str_reserve(str, 16)) != NULL);
	assert(str_capacity(str) == 1024);
	// This attempts to pass a pointer to a location outside the string
	// (str-32).  If this works correctly, then the pointer returned will be for
	// an entirely new string object allocated by str_reserve.  This is checked
	// by testing to see if the capacity is 16 or 1024.  If it's 16, then indeed
	// we are looking at a newly-created string, and the old string is lost as a
	// memory leak.
	str_old = str;
	assert((str = str_reserve(str-32, 16)) != NULL);
	assert(str_capacity(str) == 16);
	str_destroy(str);
	str = str_old; // Plug the memory leak
	assert(str_capacity(str) == 1024);
	// TEST: str_shrink_reserve()
	assert((str = str_shrink_reserve(str)) != NULL);
	assert(str_capacity(str) == 1);
	assert((str = str_reserve(str, 1024)) != NULL);
	strcpy(str, "Hello World");
	assert((str = str_shrink_reserve(str)) != NULL);
	assert(str_capacity(str) == 16);
	assert(strcmp(str, "Hello World") == 0);
	str_destroy(str);

	// TEST: str_clear()
	assert((str = str_create(NULL)) != NULL);
	assert((str = str_assign(str, "Hello World")) != NULL);
	assert(strcmp(str, "Hello World") == 0);
	assert(str_len(str) == 11);
	str_clear(str);
	assert(strcmp(str, "") == 0);
	assert(str_len(str) == 0);
	str_destroy(str);

	// TEST: str_destroy()
	str = NULL;
	assert(str_destroy(str) == NULL);
	str = (char *)malloc(sizeof(char) * 10);
	assert(str_destroy(str) == NULL);

	// TEST: str_assign()
	assert((str = str_assign(str, "")) != NULL);
	assert(str_len(str) == 0);
	assert(strcmp(str, "") == 0);
	assert((str = str_destroy(str)) == NULL);

	assert((str = str_assign(str, "Hello World")) != NULL);
	assert(str_len(str) == 11);
	assert(strcmp(str, "Hello World") == 0);

	assert((str = str_assign(str+6, "Again World")) != NULL);
	assert(str_len(str) == 17);
	assert(strcmp(str, "Hello Again World") == 0);

	assert((str = str_assign(str+6, str)) != NULL);
	assert(str_len(str) == 23);
	assert(strcmp(str, "Hello Hello Again World") == 0);

	assert((str = str_assign(str, "Hello")) != NULL);
	assert(str_len(str) == 5);
	assert(strcmp(str, "Hello") == 0);
	assert((str = str_reserve(str, 1024)) != NULL);
	str_old = str;
	assert((str = str_assign(str+6, " World")) != NULL);
	assert(str == str_old);
	assert(strcmp(str, "Hello") == 0);
	assert(strcmp(str+6, " World") == 0);
	assert(str_destroy(str) == NULL);

	str = str_create("Hello World");
	str_old = str;
	assert(str_capacity(str) == 16);
	str = str_assign(str+15, "Hi");
	assert(str_capacity(str) == 32);
	assert(str_destroy(str) == NULL);

	// TEST: str_insert()
	assert((str1 = str_create(NULL)) != NULL);
	assert((str2 = str_create(NULL)) != NULL);

	assert((str1 = str_assign(str1, "Hello World")) != NULL);
	assert((str2 = str_assign(str2, "Again ")) != NULL);
	assert((str1 = str_insert(str1+6, str2)) != NULL);
	assert(strcmp(str1, "Hello Again World") == 0);

	assert((str1 = str_assign(str1, "Hello World")) != NULL);
	assert((str2 = str_assign(str2, "Again ")) != NULL);
	assert((str1 = str_insert(str1, str2)) != NULL);
	assert(strcmp(str1, "Again Hello World") == 0);

	assert((str1 = str_assign(str1, "Hello World")) != NULL);
	assert((str2 = str_assign(str2, "Again ")) != NULL);
	assert((str1 = str_insert(str1+str_len(str1), str2)) != NULL);
	assert(strcmp(str1, "Hello WorldAgain ") == 0);

	assert((str1 = str_assign(str1, "Hello World")) != NULL);
	assert((str1 = str_insert(str1, str1)) != NULL);
	assert(strcmp(str1, "Hello WorldHello World") == 0);

	assert((str1 = str_assign(str1, "Hello World")) != NULL);
	assert((str1 = str_insert(str1+5, str1)) != NULL);
	assert(strcmp(str1, "HelloHello World World") == 0);

	str1 = str_destroy(str1);
	assert((str1 = str_insert(str1, "Hello World")) != NULL);
	assert(strcmp(str1, "Hello World") == 0);

	assert((str1 = str_destroy(str1)) == NULL);
	assert((str2 = str_destroy(str2)) == NULL);

	// TEST: str_erase()
	assert((str = str_create("Hello World")) != NULL);
	assert((str = str_erase(str, 1)) != NULL);
	assert(strcmp(str, "ello World") == 0);
	assert((str = str_erase(str+3, 3)) != NULL);
	assert(strcmp(str, "ellorld") == 0);
	assert((str = str_erase(str+6, 3)) != NULL);
	assert(strcmp(str, "ellorl") == 0);
	assert((str = str_erase(str+6, 1)) != NULL);
	assert(strcmp(str, "ellorl") == 0);
	assert((str = str_destroy(str)) == NULL);

	// TEST: str_replace()
	assert((str = str_create("Hello World")) != NULL);
	assert((str = str_replace(str, 5, "Goodbye")) != NULL);
	assert(strcmp(str, "Goodbye World") == 0);
	assert((str = str_assign(str, "Hello World")) != NULL);
	assert((str = str_replace(str+6, 5, "Moon")) != NULL);
	assert(strcmp(str, "Hello Moon") == 0);
	assert((str = str_assign(str, "Hello World")) != NULL);
	assert((str = str_replace(str+5, 1, " Again ")) != NULL);
	assert(strcmp(str, "Hello Again World") == 0);
	assert((str = str_assign(str, "Hello World")) != NULL);
	assert((str = str_replace(str+6, 17, "Moon")) != NULL);
	assert(strcmp(str, "Hello Moon") == 0);
	assert((str = str_destroy(str)) == NULL);

	// TEST: str_find(), str_caseless_find()
	assert((str1 = str_create("Hello World")) != NULL);
	assert(str_find(str1, "qwerty") == NULL);
	assert(strcmp(str_find(str1, "Hello"), "Hello World") == 0);
	assert(strcmp(str_find(str1, "World"), "World") == 0);
	assert(strcmp((str2 = str_find(str1, "l")), "llo World") == 0);
	assert(strcmp((str2 = str_find(str2+1, "l")), "lo World") == 0);
	assert(strcmp((str2 = str_find(str2+1, "l")), "ld") == 0);
	assert((str2 = str_find(str2+1, "l")) == NULL);

	assert(str_caseless_find(str1, "qwerty") == NULL);
	assert(strcmp(str_caseless_find(str1, "hello"), "Hello World") == 0);
	assert(strcmp(str_caseless_find(str1, "world"), "World") == 0);
	assert(strcmp((str2 = str_caseless_find(str1, "L")), "llo World") == 0);
	assert(strcmp((str2 = str_caseless_find(str2+1, "L")), "lo World") == 0);
	assert(strcmp((str2 = str_caseless_find(str2+1, "L")), "ld") == 0);
	assert((str2 = str_caseless_find(str2+1, "L")) == NULL);

	assert((str1 = str_destroy(str1)) == NULL);

	// TEST: str_rfind(), str_caseless_rfind()
	assert((str1 = str_create("Hello World")) != NULL);
	assert(str_rfind(str1+str_len(str1), "qwerty") == NULL);
	assert(strcmp(str_rfind(str1+str_len(str1), "Hello"), "Hello World") == 0);
	assert(strcmp(str_rfind(str1+str_len(str1), "World"), "World") == 0);
	assert(strcmp((str2 = str_rfind(str1+str_len(str1), "l")), "ld") == 0);
	assert(strcmp((str2 = str_rfind(str2-1, "l")), "lo World") == 0);
	assert(strcmp((str2 = str_rfind(str2-1, "l")), "llo World") == 0);
	assert((str2 = str_rfind(str2-1, "l")) == NULL);

	assert(str_caseless_rfind(str1+str_len(str1), "qwerty") == NULL);
	assert(strcmp(str_caseless_rfind(str1+str_len(str1), "hello"), "Hello World") == 0);
	assert(strcmp(str_caseless_rfind(str1+str_len(str1), "world"), "World") == 0);
	assert(strcmp((str2 = str_caseless_rfind(str1+str_len(str1), "L")), "ld") == 0);
	assert(strcmp((str2 = str_caseless_rfind(str2-1, "L")), "lo World") == 0);
	assert(strcmp((str2 = str_caseless_rfind(str2-1, "L")), "llo World") == 0);
	assert((str2 = str_caseless_rfind(str2-1, "L")) == NULL);

	assert((str1 = str_destroy(str1)) == NULL);

	// TEST: str_find_char()
	assert((str1 = str_create("Hello World")) != NULL);
	assert((str2 = str_find_char(str1, "H")) != NULL);
	assert(str2 == str1);
	assert((str2 = str_find_char(str2+1, "e")) != NULL);
	assert(str2 == str1+1);
	assert((str2 = str_find_char(str2+1, "W")) != NULL);
	assert(str2 == str1+6);
	assert((str2 = str_find_char(str2+1, "d")) != NULL);
	assert(str2 == str1+10);
	assert((str1 = str_destroy(str1)) == NULL);

	// TEST: str_rfind_char()
	assert((str1 = str_create("Hello World")) != NULL);
	assert((str2 = str_rfind_char(str1+10, "d")) != NULL);
	assert(str2 == str1+10);
	assert((str2 = str_rfind_char(str2-1, "W")) != NULL);
	assert(str2 == str1+6);
	assert((str2 = str_rfind_char(str2-1, "e")) != NULL);
	assert(str2 == str1+1);
	assert((str2 = str_rfind_char(str2-1, "H")) != NULL);
	assert(str2 == str1);
	assert((str1 = str_destroy(str1)) == NULL);

	// TEST: str_find_not_char()
	assert((str1 = str_create("Hello World")) != NULL);
	assert((str2 = str_find_not_char(str1, "z")) == str1);
	assert((str2 = str_find_not_char(str1, "H")) == str1+1);
	assert((str2 = str_find_not_char(str1, "Hel")) == str1+4);
	assert((str2 = str_find_not_char(str1, "Helo Wor")) == str1+10);
	assert((str1 = str_destroy(str1)) == NULL);

	// TEST: str_rfind_not_char()
	assert((str1 = str_create("Hello World")) != NULL);
	assert((str2 = str_rfind_not_char(str1+10, "z")) == str1+10);
	assert((str2 = str_rfind_not_char(str1+10, "rld")) == str1+7);
	assert((str2 = str_rfind_not_char(str1+10, " World")) == str1+1);
	assert((str2 = str_rfind_not_char(str1+10, "e World")) == str1);
	assert((str1 = str_destroy(str1)) == NULL);

	// TEST: str_substr()
	assert((str1 = str_create("Hello World")) != NULL);
	assert(strcmp((str2 = str_substr(str1, 5)), "Hello") == 0);
	str2 = str_destroy(str2);
	assert(strcmp((str2 = str_substr(str1+4, 3)), "o W") == 0);
	str2 = str_destroy(str2);
	assert(strcmp((str2 = str_substr(str1+6, 5)), "World") == 0);
	str2 = str_destroy(str2);
	assert(strcmp((str2 = str_substr(str1+10, 5)), "d") == 0);
	str2 = str_destroy(str2);
	assert(strcmp((str2 = str_substr(str1+11, 5)), "") == 0);
	str2 = str_destroy(str2);
	assert((str1 = str_destroy(str1)) == NULL);

	// TEST: str_compare()
	assert((str1 = str_create("Hello World")) != NULL);
	assert(str_compare(str1, "Hello World") == 0);
	assert(str_compare(str1, "H") > 0);
	assert(str_compare(str1, "Hello Worldo") < 0);
	assert((str1 = str_destroy(str1)) == NULL);

	// TEST: str_caseless_compare()
	assert((str1 = str_create("Hello World")) != NULL);
	assert(str_caseless_compare(str1, "hello world") == 0);
	assert(str_caseless_compare(str1, "h") > 0);
	assert(str_caseless_compare(str1, "hello worldo") < 0);
	assert((str1 = str_destroy(str1)) == NULL);

	// TEST: str_swap()
	assert((str1 = str_create("Hello")) != NULL);
	assert((str2 = str_create("Goodbye")) != NULL);
	assert(str_swap(&str1, &str2) == 1);
	assert(str_compare(str1, "Goodbye") == 0);
	assert(str_compare(str2, "Hello") == 0);
	str1 = str_destroy(str1);
	str2 = str_destroy(str2);

	// TEST: str_append()
	// TEST: str_prepend()
	assert((str1 = str_create("Hello World")) != NULL);
	assert(str_compare((str1 = str_prepend(str1, "1")), "1Hello World") == 0);
	assert(str_compare((str1 = str_prepend(str1+1, "2")), "21Hello World") == 0);
	assert(str_compare((str1 = str_prepend(str1+10, "3")), "321Hello World") == 0);
	assert(str_compare((str1 = str_append(str1, "4")), "321Hello World4") == 0);
	assert(str_compare((str1 = str_append(str1+1, "5")), "321Hello World45") == 0);
	assert(str_compare((str1 = str_append(str1+15, "6")), "321Hello World456") == 0);
	assert((str1 = str_destroy(str1)) == NULL);

	// TEST: str_geline()
	int p[2] = { 0, 0 };
	FILE * fin = NULL;
	FILE * fout = NULL;
	assert(pipe(p) == 0);
	assert((fin = fdopen(p[0], "r")) != NULL);
	assert((fout = fdopen(p[1], "w")) != NULL);
	fprintf(fout, "Hello World\nHello Again\n");
	assert(fflush(fout) == 0);
	assert(str_getline(&str, fin, " \n") == 5);
	assert(str_compare(str, "Hello") == 0);
	fgetc(fin);
	str_clear(str);
	assert(str_getline(&str, fin, " \n") == 5);
	assert(str_compare(str, "World") == 0);
	fgetc(fin);
	str_clear(str);
	assert(str_getline(&str, fin, "\n") == 11);
	assert(str_compare(str, "Hello Again") == 0);
	fgetc(fin);
	assert(feof(fin) == 0);
	fclose(fin);
	fclose(fout);
	close(p[0]);
	close(p[1]);
	str = str_destroy(str);

	// TEST: str_split()
	dllist_t * l = NULL;
	dllist_t * ln = NULL;
	str1 = str_create("The quick brown fox jumpped over the lazy dog.");
	str2 = str_create(" \t.");
	l = str_split(str1, str2, 500);
	assert(dllist_size(l) == 9);
	ln = dllist_first(l);
	assert(str_compare((char *)dllist_value(ln), "The") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "quick") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "brown") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "fox") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "jumpped") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "over") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "the") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "lazy") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "dog") == 0);
	ln = dllist_next(ln);
	assert(ln == NULL);
	str1 = str_destroy(str1);
	str2 = str_destroy(str2);
	l = dllist_destroy(l);

	str1 = str_create("The quick brown fox jumpped over the lazy dog.");
	str2 = str_create(" ");
	l = str_split(str1, str2, 500);
	assert(dllist_size(l) == 9);
	ln = dllist_first(l);
	assert(str_compare((char *)dllist_value(ln), "The") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "quick") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "brown") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "fox") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "jumpped") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "over") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "the") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "lazy") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "dog.") == 0);
	ln = dllist_next(ln);
	assert(ln == NULL);
	str1 = str_destroy(str1);
	str2 = str_destroy(str2);
	l = dllist_destroy(l);

	str1 = str_create("The quick brown fox jumpped over the lazy dog");
	str2 = str_create(" ");
	l = str_split(str1, str2, 500);
	assert(dllist_size(l) == 9);
	ln = dllist_first(l);
	assert(str_compare((char *)dllist_value(ln), "The") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "quick") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "brown") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "fox") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "jumpped") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "over") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "the") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "lazy") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "dog") == 0);
	ln = dllist_next(ln);
	assert(ln == NULL);
	str1 = str_destroy(str1);
	str2 = str_destroy(str2);
	l = dllist_destroy(l);

	str1 = str_create("The quick brown fox jumpped over the lazy dog");
	str2 = str_create(" ");
	l = str_split(str1, str2, 5);
	assert(dllist_size(l) == 5);
	ln = dllist_first(l);
	assert(str_compare((char *)dllist_value(ln), "The") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "quick") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "brown") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "fox") == 0);
	ln = dllist_next(ln);
	assert(str_compare((char *)dllist_value(ln), "jumpped over the lazy dog") == 0);
	ln = dllist_next(ln);
	assert(ln == NULL);
	str1 = str_destroy(str1);
	str2 = str_destroy(str2);
	l = dllist_destroy(l);

	str1 = str_create(NULL);
	str1 = str_create(NULL);
	l = str_split(str1, str2, 500);
	assert(dllist_size(l) == 0);
	str1 = str_destroy(str1);
	str2 = str_destroy(str2);
	l = dllist_destroy(l);

	// TEST: str_join()
	str1 = str_create("The quick brown fox jumpped over the lazy dog.");
	str2 = str_create(" \t.");
	l = str_split(str1, str2, 500);
	assert(dllist_size(l) == 9);
	str = str_join(l, " @@@ ");
	assert(str_compare(str, "The @@@ quick @@@ brown @@@ fox @@@ jumpped @@@ over @@@ the @@@ lazy @@@ dog") == 0);
	str1 = str_destroy(str1);
	str2 = str_destroy(str2);
	str = str_destroy(str);
	l = dllist_destroy(l);

	str1 = str_create("The quick brown fox jumpped over the lazy dog.");
	str2 = str_create(" \t.");
	l = str_split(str1, str2, 500);
	assert(dllist_size(l) == 9);
	str = str_join(l, "");
	assert(str_compare(str, "Thequickbrownfoxjumppedoverthelazydog") == 0);
	str1 = str_destroy(str1);
	str2 = str_destroy(str2);
	str = str_destroy(str);
	l = dllist_destroy(l);

	return(0);
}
