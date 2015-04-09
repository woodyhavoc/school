
CC=gcc
CFLAGS=-Wall -g -lm -O0 -I. -MMD
LFLAGS=

SUBDIRS=doublyLinkedLists

PROGS=subdirs wf
OBJECTS=HashTable.o WordObj.o doublyLinkedLists/List.o doublyLinkedLists/Node.o

all: $(PROGS) dox 

subdirs:
	cd doublyLinkedLists; make

wf: wf.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS) 

dox:
	echo "Generating documentation using doxygen..."
	doxygen doxygen-config >& doxygen.log
	echo "Use konqueror docs/html/index.html to see docs (or any other browser)"

clean:
	cd doublyLinkedLists; make clean
	/bin/rm -f $(PROGS) *.o a.out  *.d
	/bin/rm -fr docs doxygen.log

