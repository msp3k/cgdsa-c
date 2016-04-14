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

all:: test-io-ready
io-ready.o: io-ready.c
test-io-ready.o: test-io-ready.c
test-io-ready: test-io-ready.o io-ready.o
clobber::
	$(RM) test-io-ready
check:: test-io-ready
	./test-io-ready

