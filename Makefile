#---[ Variable Declarations ]-------------------------------------------------

#---[ Directive Stubs ]-------------------------------------------------------

all::
clean::
	$(RM) *.o
clobber:: clean
check::

#---[ Directives ]------------------------------------------------------------

all::
	for m in $$(find . -type f -name Makefile | grep -v '^\./Makefile'); do \
		d=$$(dirname "$$m"); \
		(cd $$d && $(MAKE)); \
	done
	$(MAKE) check
	$(MAKE) docs

.PHONY: docs
docs:
	-$(RM) -fr ./docs
	mkdir ./docs
	doxygen ./doxygen.conf

clean::
	for m in $$(find . -type f -name Makefile | grep -v '^\./Makefile'); do \
		d=$$(dirname "$$m"); \
		(cd $$d && $(MAKE) clean); \
	done

clobber::
	for m in $$(find . -type f -name Makefile | grep -v '^\./Makefile'); do \
		d=$$(dirname "$$m"); \
		(cd $$d && $(MAKE) clobber); \
	done
	-$(RM) -r docs

check::
	for m in $$(find . -type f -name Makefile | grep -v '^\./Makefile'); do \
		d=$$(dirname "$$m"); \
		(cd $$d && $(MAKE) check); \
	done
	@echo
	@echo "OK -- ALL TESTS PASSED"
	@echo

view-docs: docs
	xdg-open docs/index.html
