#---[ Variable Declarations ]-------------------------------------------------

CC = gcc
CFLAGS = -g -Wall
LDFLAGS = 

#---[ Directive Stubs ]-------------------------------------------------------

all::
clean::
	$(RM) *.o
clobber:: clean
check::

#---[ Directives ]------------------------------------------------------------

all:: test-rbtree
rbtree.o: rbtree.c
test-rbtree.o: test-rbtree.c
test-rbtree: test-rbtree.o rbtree.o
clobber::
	$(RM) test-rbtree
check:: test-rbtree
	./test-rbtree

