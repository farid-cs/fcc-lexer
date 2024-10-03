#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

typedef struct Lexer Lexer;
struct Lexer {
	const char *start;
	const char *pos;
};

extern Lexer new_lexer(const char*);
extern int next_token(Lexer*);

#endif
