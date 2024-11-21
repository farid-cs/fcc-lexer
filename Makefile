CC = cc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -g
TEST_OBJ = tests.o lexer.o

all: tests

tests: ${TEST_OBJ}
	${CC} -o $@ ${TEST_OBJ}

tests.o: tests.c lexer.h
lexer.o: lexer.c lexer.h

test: tests
	./tests

clean:
	rm -f *.o tests

.PHONY: all clean test
