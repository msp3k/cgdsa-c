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

all:: test-dllist
dllist.o: dllist.c
test-dllist.o: test-dllist.c
test-dllist: test-dllist.o dllist.o
clobber::
	$(RM) test-dllist
check:: test-dllist
	./test-dllist

