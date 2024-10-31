CFLAGS = -Wall -Wextra -g
PREFIX = /usr/local

liblexer.a: lexer.o
	$(AR) r $@ $<

test_lexer: test_lexer.o liblexer.a

test: test_lexer
	./test_lexer

install:
	cp liblexer.a ${PREFIX}/lib

clean:
	rm -f *.o *.a test_lexer
.PHONY: clean test install
