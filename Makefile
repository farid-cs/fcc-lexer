CC = cc
CFLAGS = -std=c99 -pedantic -Wall -Wextra

all: lexer

lexer: lexer.o
	${CC} -o $@ lexer.o

clean:
	rm -f lexer *.o

.PHONY: all clean test
