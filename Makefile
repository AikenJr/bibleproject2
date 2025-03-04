# User information
USER = aikbarahira

# Compiler and flags
CC = g++
CFLAGS = -g -std=c++11

# Object files needed for the project
OBJS = Bible.o Ref.o Verse.o
CGI_OBJS = bibleajax.o $(OBJS)
TEST_OBJS = testreader.o $(OBJS)

# Directory paths
CGI_DIR = /var/www/html/class/csc3004/$(USER)/cgi-bin
HTML_DIR = /var/www/html/class/csc3004/$(USER)

all: testreader

# Test program for Bible indexing
testreader: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o testreader $(TEST_OBJS)

testreader.o: testreader.cpp Bible.h Ref.h Verse.h
	$(CC) $(CFLAGS) -c testreader.cpp

# Object file compilation rules
Bible.o: Bible.cpp Bible.h
	$(CC) $(CFLAGS) -c Bible.cpp

Ref.o: Ref.cpp Ref.h
	$(CC) $(CFLAGS) -c Ref.cpp

Verse.o: Verse.cpp Verse.h
	$(CC) $(CFLAGS) -c Verse.cpp

# Cleanup
clean:
	rm -f *.o core testreader bibleajax.cgi

# Temporarily commented out targets for Project 3 Part 1
# Uncomment these for Part 2
#
# # Main executable
# bibleajax.cgi: $(CGI_OBJS)
# 	$(CC) $(CFLAGS) -o bibleajax.cgi $(CGI_OBJS) -lcgicc
#
# bibleajax.o: bibleajax.cpp Bible.h Ref.h Verse.h
# 	$(CC) $(CFLAGS) -c bibleajax.cpp
#
# # Installation targets
# PutCGI: bibleajax.cgi
# 	chmod 755 bibleajax.cgi
# 	cp bibleajax.cgi $(CGI_DIR)
# 	@echo "Current contents of your cgi-bin directory: "
# 	@ls -l $(CGI_DIR)
#
# PutHTML:
# 	cp bibleajax.html $(HTML_DIR)
# 	@echo "Current contents of your HTML directory: "
# 	@ls -l $(HTML_DIR)
#
# # Full web deployment target
# web: bibleajax.cgi PutCGI PutHTML

# Phony targets
.PHONY: all clean
