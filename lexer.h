/* MIT License
 *
 * Copyright (c) Farid Farajli
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
	Question,
	Tilde,
	Comma,
	Period,
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

	Integer,
};

enum ErrorType {
	NOERROR,
	BLOCK_COMMENT,
	INTEGER_SUFFIX,
	UNEXPECTED,
};

typedef struct Token Token;
struct Token {
	enum TokenType type;
	char *pos;
	size_t len;
};

typedef struct Lexer Lexer;
struct Lexer {
	char *start;
	char *pos;
	enum ErrorType err;
};

void	lexer_init(Lexer *, const char *);
int	next_token(Lexer *, Token *);
void	print_error(Lexer *);

#endif
