INSTALL=..
INCLUDE=$(INSTALL)/include -I.
CC=gcc
CFLAGS = -g -Wall -I$(INCLUDE)
LFLAGS=-L$(LIBS) -lmylib
LIBS=$(INSTALL)/lib -lpthread #-Llib/ -lmylib
DEBUG=

all: pc pc-mq

pc: pc.o  Item.o Item.h
	$(CC) $(CFLAGS) $(DEBUG) -o $@ pc.o Item.o  $(LFLAGS)
	cp ../doxygen-config ./
	cp pc ../

pc-mq: pc-mq.o Item.o Item.h
	$(CC) $(CFLAGS) $(DEBUG) -o $@ pc-mq.o Item.o  $(LFLAGS)
	cp pc-mq ../


clean:
	/bin/rm -f *.o pc
	/bin/rm -f *.o pc-mq
	rm -rf doxygen-config
	rm -rf doxygen.log
