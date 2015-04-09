
CC=gcc
CFLAGS=-Wall -g -c -O0 -I. -MMD 
LFLAGS=

PROGS=build
OBJECTS=List.o Node.o

all: dox 
	$(CC) $(CFLAGS) *.c

-include *.d

dox:
	echo "Generating documentation using doxygen..."
	doxygen doxygen-config >& doxygen.log
	echo "Use konqueror docs/html/index.html to see docs (or any other browser)"

clean:
	/bin/rm -f $(PROGS) *.o a.out  *.d
	/bin/rm -fr docs doxygen.log

