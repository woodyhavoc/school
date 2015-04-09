
CC=mpicc
CFLAGS=-Wall -O
#CFLAGS=-g -gstabs+ -Wall
OBJS=pmcpi.o timing.o

pmcpi.o: pmcpi.o timing.o
	$(CC) $(CFLAGS) -o $@ $(OBJS)


clean:
	/bin/rm -f *.o pmcpi

