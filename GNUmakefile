CFLAGS = -Wall -Wextra -g

liblexer.a: lexer.o
	$(AR) r $@ $<

test_lexer: test_lexer.o liblexer.a

test: test_lexer
	./test_lexer
clean:
	rm -f *.o *.a test_lexer
.PHONY: clean test
