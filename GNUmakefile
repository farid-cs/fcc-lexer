CFLAGS = -Wall -Wextra -g

lib/liblexer.a: src/lexer.o lib
	$(AR) r $@ $<

lib:
	mkdir -p lib

tests/test_lexer: tests/test_lexer.o lib/liblexer.a

test: tests/test_lexer
	./tests/test_lexer
clean:
	rm -rf lib src/*.o
.PHONY: clean test
