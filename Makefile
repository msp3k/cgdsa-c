all:
	for m in $$(find . -type f -name Makefile | grep -v '^\./Makefile'); do \
		d=$$(dirname "$$m"); \
		(cd $$d && $(MAKE)); \
	done
	$(MAKE) check

docs:
	-$(RM) -f ./mainpage.h
	echo "/** \mainpage [C]ool and [G]roovy [D]ata [S]tructures and [A]lgorithms in [C]" >> mainpage.h
	echo " * GitHub Repository: https://github.com/msp3k/cgdsa-c/tree/master" >> mainpage.h
	for m in $$(find . -type f -name Makefile | grep -v '^\./Makefile'); do \
		d=$$(dirname "$$m"); \
		(cd $$d && grep 'mainpagesection' *.h) \
		| sed 's,mainpagesection,section,g' \
		>> mainpage.h ; \
	done
	echo " */" >> mainpage.h
	-$(RM) -fr ./docs
	mkdir ./docs
	doxygen ./doxygen.conf

clean:
	for m in $$(find . -type f -name Makefile | grep -v '^\./Makefile'); do \
		d=$$(dirname "$$m"); \
		(cd $$d && $(MAKE) clean); \
	done

clobber:
	for m in $$(find . -type f -name Makefile | grep -v '^\./Makefile'); do \
		d=$$(dirname "$$m"); \
		(cd $$d && $(MAKE) clobber); \
	done
	-$(RM) -r docs
	-$(RM) mainpage.h

check:
	for m in $$(find . -type f -name Makefile | grep -v '^\./Makefile'); do \
		d=$$(dirname "$$m"); \
		(cd $$d && $(MAKE) check); \
	done
	@echo
	@echo "OK -- ALL TESTS PASSED"
	@echo

