INSTALL=..
INCLUDE=$(INSTALL)/include -I.
CC=gcc
CFLAGS= -g -Wall -I$(INCLUDE)
LFLAGS=-L$(LIBS) -lmylib 
LIBS=$(INSTALL)/lib -lreadline -lncurses
OBJECTS=Command.o Job.o svn_version.o

PROGS=mydash 

all: $(PROGS) dox

mydash: mydash.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@  $^ $(LFLAGS)
	cp mydash ../
	cp valgrind.supp ../

dox:
	cp ../doxygen-config ./

clean:
	rm --force *.o $(PROGS) a.out core  *.log
	rm --force doxygen-config
	rm --force ../mydash
	rm --force ../valgrind.supp

##
## on every build, record the working copy revision string
##
svn_version.c: FORCE
		echo -n 'const char* svn_version(void) { const char* SVN_Version = "' > svn_version.c
		svnversion -n . >> svn_version.c
		echo '"; return SVN_Version; }' >> svn_version.c

FORCE:
##
## Then any executable that links in svn_version.o will be able
## to call the function svn_version() to get a string that
## describes exactly what revision was built.
