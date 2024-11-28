CC = cc
CFLAGS = -g -std=c99 -pedantic -Wall -Wextra

all: tests

tests: tests.o lexer.o
	${CC} -o $@ tests.o lexer.o

tests.o: tests.c lexer.h
lexer.o: lexer.c lexer.h

test: tests
	./tests

clean:
	rm -f *.o tests

.PHONY: all clean test
