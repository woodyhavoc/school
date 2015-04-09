JFLAGS=

#Recognize files with .class and .java extenstions
.SUFFIXES: .class .java
#
# This is a rule to convert a file with a .java extension
# into a file with a .class extension.  The macro $< just
# supplies the name of the file (without the extension)
# that invoked this rule.

.java.class:
	javac $(JFLAGS) $<

CC=gcc
CFLAGS = -g -Wall
LFLAGS=$(LIBS)
LIBS= -pthread
DEBUG=
PROGS = fileAccess FileAccess.class FileAccessBoss.class

all: $(PROGS)

fileAccess: FileAccessDriver.o FileAccess.o FileAccess.h
	$(CC) $(CFLAGS) $(DEBUG) -o $@ FileAccessDriver.o FileAccess.o  $(LFLAGS)

FileAccess: FileAccess.class FileAccessBoss.class

clean:
	rm -rf *.o $(PROGS)
