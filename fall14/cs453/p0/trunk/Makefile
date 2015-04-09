

all: library testcode dox

library:	
	cd libsrc; make install

testcode:
	cd testsuite; make

clean:
	cd libsrc; make installclean
	cd testsuite; make clean
	rm -rf docs doxygen.log

dox:
	@echo "Generating documentation using doxygen..."
	cp doxygen-config libsrc/
	cp doxygen-config testsuite/
	doxygen libsrc/doxygen-config >& doxygen.log
	doxygen testsuite/doxygen-config >& doxygen.log
	@echo "Use konqueror docs/html/index.html to see docs (or any other browser)"
