all:
	for m in $$(find . -type f -name Makefile | grep -v '^\./Makefile'); do \
		d=$$(dirname "$$m"); \
		(cd $$d && $(MAKE)); \
	done
	-$(RM) -f ./mainpage.h
	echo "/** \mainpage [C]ool and [G]roovy [D]ata [S]tructures and [A]lgorithms in [C]" >> mainpage.h
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
	$(MAKE) check

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

check:
	for m in $$(find . -type f -name Makefile | grep -v '^\./Makefile'); do \
		d=$$(dirname "$$m"); \
		(cd $$d && $(MAKE) check); \
	done
	@echo
	@echo "OK"

