CFLAGS = -Wall -Wextra -O2

facc: facc.o lexer.o
clean:
	rm -f facc *.o *.out

.PHONY: clean
