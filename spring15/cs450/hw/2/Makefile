CC=gcc
CXX=g++
CFLAGS= -g -Wno-switch
CPPFLAGS= -g -Wall
SCANNER=scanner/scanner.o
OBJECTS=$(SCANNER) tree.o parser.o
LFLAGS=
PROGS=scanner compiler

all: $(PROGS)

scanner:
	cd scanner; make

compiler: parser.o generator.o
	gcc -o j2a main.c

parser.o: tree.o
	gcc -c parser.c

generator.o:
	gcc -c generator.c

clean:
	cd scanner; make clean
	rm --force *.o *.d a.out core *.log
