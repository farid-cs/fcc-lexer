CC = cc
CFLAGS = -Wall -Wextra -Wpedantic -g
LDFLAGS = -L. -llexer
PREFIX = /usr/local

all: liblexer.a

.o:
	${CC} -o $@ $< ${LDFLAGS}

liblexer.a: lexer.o
	${AR} rcs $@ $?

tests: tests.o liblexer.a
tests.o: tests.c lexer.h
lexer.o: lexer.c lexer.h

test: tests
	./tests

clean:
	rm -f *.o *.a tests

install: all
	mkdir -p ${PREFIX}/include ${PREFIX}/lib
	cp lexer.h ${PREFIX}/include
	cp liblexer.a ${PREFIX}/lib

.PHONY: all clean test install
