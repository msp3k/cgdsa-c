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

all:: test-tree
tree.o: tree.c
test-tree.o: test-tree.c
test-tree: test-tree.o tree.o
clobber::
	$(RM) test-tree
check:: test-tree
	./test-tree

