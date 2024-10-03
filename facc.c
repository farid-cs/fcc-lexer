#include <stdio.h>
#include "lexer.h"

int main(int argc, char** argv)
{
	if (argc < 2) {
		fprintf(stderr, "usage: %s <source_code>\n", argv[0]);
		return 1;
	}

	Lexer lex = new_lexer(argv[1]);
	while (!next_token(&lex)) {
	}

	return 0;
}
