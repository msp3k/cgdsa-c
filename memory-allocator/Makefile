#---[ Variable Declarations ]-------------------------------------------------

RBTREE_DIR = ../red-black-tree

CC = gcc
CFLAGS = -I$(RBTREE_DIR) -g -Wall
LDFLAGS = 

#---[ Directive Stubs ]-------------------------------------------------------

all::
clean::
	$(RM) *.o
clobber:: clean
check::

#---[ Directives ]------------------------------------------------------------

all:: BUILD_DEPENDENCIES
.PHONY: BUILD_DEPENDENCIES
BUILD_DEPENDENCIES:
	cd $(RBTREE_DIR) && $(MAKE)
mem.o: mem.c
mem-pthread.o: mem.c
	$(CC) $(CFLAGS) -DWITH_PTHREAD -c -o mem-pthread.o mem.c
test-mem.o: test-mem.c
test-mem-pthread.o: test-mem-pthread.c
	$(CC) $(CFLAGS) -DWITH_PTHREAD -c test-mem-pthread.c
test-mem: test-mem.o mem.o $(RBTREE_DIR)/rbtree.o
test-mem-pthread: test-mem-pthread.o mem-pthread.o $(RBTREE_DIR)/rbtree.o -lpthread
all:: test-mem
all:: test-mem-pthread
clobber::
	$(RM) test-mem
	$(RM) test-mem-pthread
check:: test-mem test-mem-pthread
	./test-mem
	./test-mem-pthread

