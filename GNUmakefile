CFLAGS = -Wall -Wextra -g

lib/liblexer.a: lexer.o lib
	$(AR) r $@ $<

lib:
	mkdir -p lib

test_lexer: test_lexer.o lib/liblexer.a

test: test_lexer
	./test_lexer
clean:
	rm -rf lib *.o
.PHONY: clean test
