#---[ Variable Declarations ]-------------------------------------------------

RBTREE_DIR = ../red-black-tree
DLLIST_DIR = ../double-linked-list
MEM_DIR = ../memory-allocator

CC = gcc
CFLAGS = -I$(RBTREE_DIR) -I$(DLLIST_DIR) -I$(MEM_DIR) -g -Wall
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
	cd $(DLLIST_DIR) && $(MAKE)
	cd $(RBTREE_DIR) && $(MAKE)
	cd $(MEM_DIR) && $(MAKE)
all:: test-str
test-str.o: test-str.c
str.o: str.c
test-str: test-str.o str.o $(MEM_DIR)/mem.o $(RBTREE_DIR)/rbtree.o $(DLLIST_DIR)/dllist.o
clobber::
	$(RM) test-str
check:: test-str
	./test-str

