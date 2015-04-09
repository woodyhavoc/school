
CC=gcc
CFLAGS=-Wall -g

all: search

search: search.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	/bin/rm -f *.o search
