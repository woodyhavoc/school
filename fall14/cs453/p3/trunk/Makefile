SUBMAKE=library pc

all: $(SUBMAKE) dox

library:	
	cd libsrc; make install

pc:
	cd producers-consumers; make

clean:
	cd libsrc; make installclean
	cd producers-consumers; make clean
	rm -rf docs doxygen.log
	rm -rf *.o *.c
	rm -rf pc
	rm -rf pc-mq

dox:
	@echo "Generating documentation using doxygen..."
	doxygen libsrc/doxygen-config >& doxygen.log
	doxygen producers-consumers/doxygen-config >& doxygen.log
	@echo "Use konqueror docs/html/index.html to see docs (or any other browser)"


