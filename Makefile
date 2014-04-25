all::
	for m in $$(find . -type f -name Makefile | grep -v '^\./Makefile'); do \
		d=$$(dirname "$$m"); \
		(cd $$d && $(MAKE)); \
	done
	$(MAKE) check

all:: mainpage.h
.PHONY: mainpage.h
mainpage.h:
	-$(RM) -f ./mainpage.h
	echo "/** \mainpage [C]ool and [G]roovy [D]ata [S]tructures and [A]lgorithms in [C]" >> mainpage.h
	echo " * GitHub Repository: https://github.com/msp3k/cgdsa-c/tree/master" >> mainpage.h
	for m in $$(find . -type f -name Makefile | grep -v '^\./Makefile'); do \
		d=$$(dirname "$$m"); \
		s=$$(cd $$d && grep 'SECTION: ' *.h | sed 's,^.*SECTION: ,,g'); \
		echo " * \section $${s}" >> mainpage.h; \
		echo " * Files: https://github.com/msp3k/cgdsa-c/tree/master/$$d" >> mainpage.h; \
	done
	echo " */" >> mainpage.h

docs: mainpage.h
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

