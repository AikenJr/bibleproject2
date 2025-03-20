# User information
USER = aikbarahira

# Compiler and flags
CC = g++
CFLAGS = -g -std=c++11

# Object files
OBJS = Bible.o Ref.o Verse.o
CGI_OBJS = bibleclient.o $(OBJS) fifo.o
SERVER_OBJS = bibleserver.o $(OBJS) fifo.o
TEST_OBJS = testreader.o $(OBJS)

CGI_DIR = /var/www/html/class/csc3004/$(USER)/cgi-bin
HTML_DIR = /var/www/html/class/csc3004/$(USER)

# Default target
all: bibleserver bibleclient.cgi testreader

# Bible server program
bibleserver: $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o bibleserver $(SERVER_OBJS)

bibleserver.o: bibleserver.cpp Bible.h Ref.h Verse.h fifo.h
	$(CC) $(CFLAGS) -c bibleserver.cpp

# The bible client CGI program
bibleclient.cgi: $(CGI_OBJS)
	$(CC) $(CFLAGS) -o bibleclient.cgi $(CGI_OBJS) -lcgicc

bibleclient.o: bibleclient.cpp Bible.h Ref.h Verse.h fifo.h
	$(CC) $(CFLAGS) -c bibleclient.cpp

# Test program for Bible indexing
testreader: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o testreader $(TEST_OBJS)

testreader.o: testreader.cpp Bible.h Ref.h Verse.h
	$(CC) $(CFLAGS) -c testreader.cpp

# Fifo object file
fifo.o: fifo.cpp fifo.h
	$(CC) $(CFLAGS) -c fifo.cpp

# Object file compilation rules
Bible.o: Bible.cpp Bible.h
	$(CC) $(CFLAGS) -c Bible.cpp

Ref.o: Ref.cpp Ref.h
	$(CC) $(CFLAGS) -c Ref.cpp

Verse.o: Verse.cpp Verse.h
	$(CC) $(CFLAGS) -c Verse.cpp

# Installation targets
PutCGI: bibleclient.cgi
	chmod 755 bibleclient.cgi
	cp bibleclient.cgi $(CGI_DIR)
	@echo "Current contents of your cgi-bin directory: "
	@ls -l $(CGI_DIR)

PutHTML:
	cp bibleindex.html $(HTML_DIR)
	@echo "Current contents of your HTML directory: "
	@ls -l $(HTML_DIR)

# Full web deployment target
web: bibleclient.cgi PutCGI PutHTML

# Cleanup
clean:
	rm -f *.o core testreader bibleserver bibleclient.cgi

# Phony targets
.PHONY: all clean PutCGI PutHTML web
