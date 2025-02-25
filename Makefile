# User information
USER = aikbarahira

# Compiler and flags
CC = g++
CFLAGS = -g -std=c++11

# Object files needed for the project
OBJS = bibleajax.o Bible.o Ref.o Verse.o

# Directory paths
CGI_DIR = /var/www/html/class/csc3004/$(USER)/cgi-bin
HTML_DIR = /var/www/html/class/csc3004/$(USER)

# Default target
all: bibleajax.cgi PutCGI PutHTML

# Main executable
bibleajax.cgi: $(OBJS)
	$(CC) $(CFLAGS) -o bibleajax.cgi $(OBJS) -lcgicc

# Object file compilation rules
bibleajax.o: bibleajax.cpp Bible.h Ref.h Verse.h
	$(CC) $(CFLAGS) -c bibleajax.cpp

Bible.o: Bible.cpp Bible.h
	$(CC) $(CFLAGS) -c Bible.cpp

Ref.o: Ref.cpp Ref.h
	$(CC) $(CFLAGS) -c Ref.cpp

Verse.o: Verse.cpp Verse.h
	$(CC) $(CFLAGS) -c Verse.cpp

# Installation targets
PutCGI: bibleajax.cgi
	chmod 755 bibleajax.cgi
	cp bibleajax.cgi $(CGI_DIR)
	@echo "Current contents of your cgi-bin directory: "
	@ls -l $(CGI_DIR)

PutHTML:
	cp bibleajax.html $(HTML_DIR)
	@echo "Current contents of your HTML directory: "
	@ls -l $(HTML_DIR)

# Cleanup
clean:
	rm -f *.o core bibleajax.cgi

# Phony targets
.PHONY: all clean PutCGI PutHTML
