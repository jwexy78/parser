CC=g++
CFLAGS=-std=c++1y -lcurses

SHAREDLIBSROOT=../sharedlibs

include $(SHAREDLIBSROOT)/catch/rules.mk

all: tokenizer test

tokenizer: tokenizer.cc Tokenizer.h StringEscaper.h
	$(CC) $(CFLAGS) $(IFLAGS) tokenizer.cc -o $@

test: test.cc Tokenizer.h StringEscaper.h
	$(CC) $(CFLAGS) $(IFLAGS) test.cc -o $@