CC = cc
CFLAGS = -Wall -Wextra -pedantic -g
PREFIX = /usr/local

all: liblexer.a

test: test_lexer
	./test_lexer

install: liblexer.a
	cp lexer.h ${PREFIX}/include
	cp liblexer.a ${PREFIX}/lib

clean:
	rm -f *.o *.a test_lexer

liblexer.a: lexer.o
	$(AR) rcs $@ $?

test_lexer: test_lexer.o liblexer.a
	$(CC) -o $@ $< liblexer.a

lexer.o: lexer.c lexer.h

.o:
	$(CC) -o $@ $<

.PHONY: all clean test install
