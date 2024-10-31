#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

enum TokenType {
	Plus,
	Minus,
	Star,
	Slash,
	Mod,
	LeftBrace,
	RightBrace,
	LeftBracket,
	RightBracket,
	LeftParen,
	RightParen,
	Equal,
	Or,
	And,
	Xor,
	Not,
	Que,
	Tilde,
	Comma,
	Dot,
	Colon,
	Semicolon,
	Backslash,
	Greater,
	Less,

	PlusEq,
	MinusEq,
	StarEq,
	SlashEq,
	ModEq,
	EqEq,
	NotEq,
	GreaterEq,
	LessEq,
	OrOr,
	AndAnd,
	OrEq,
	AndEq,
	XorEq,
	PlusPlus,
	MinusMinus,
	RightShift,
	LeftShift,
	Arrow,

	RightShiftEq,
	LeftShiftEq,
	AndAndEq,
	OrOrEq,
	Elipses,
};

typedef struct Token Token;
struct Token {
	enum TokenType type;
};

typedef struct Lexer Lexer;
struct Lexer {
	const char *start;
	const char *pos;
};

Lexer	new_lexer(const char *);
int	next_token(Lexer *, Token *);

#endif
