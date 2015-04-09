SUBMAKE=library mydash

all: $(SUBMAKE) dox

library:	
	cd libsrc; make install

mydash: 
	cd dash; make

clean:
	cd libsrc; make installclean
	cd dash; make clean
	rm -rf docs doxygen.log
	rm -rf *.o *.c

dox:
	@echo "Generating documentation using doxygen..."
	doxygen libsrc/doxygen-config >& doxygen.log
	doxygen dash/doxygen-config >& doxygen.log
	@echo "Use konqueror docs/html/index.html to see docs (or any other browser)"


