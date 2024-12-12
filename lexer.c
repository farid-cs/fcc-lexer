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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

static void
print_pos(Lexer *self, const char *pos)
{
	size_t row = 1, col = 1;

	for (const char *p = self->start; p != pos; p++) {
		if (*p == '\n') {
			row++;
			col = 1;
		} else {
			col++;
		}
	}
	fprintf(stderr, "%zu:%zu: ", row, col);
}

static int
skip_space_or_comment(Lexer *self)
{
	const char *pos = self->pos;

	switch (*pos++) {
	case '/':
		if (*pos == '/') {
			while (*pos && *pos != '\n')
				pos++;
			self->pos = (char *) pos + 1;
			return 0;
		}

		if (*pos != '*')
			break;
		for (;; pos++) {
			if (*pos == '\0') {
				print_pos(self, pos);
				fprintf(stderr, "error: unterminated block comment\n");
				exit(1);
			}
			if (pos[0] == '*' && pos[1] == '/')
				break;
		}
		self->pos = (char *) pos + 2;
		return 0;
	case ' ':
	case '\t':
	case '\v':
	case '\r':
	case '\n':
		self->pos++;
		return 0;
	}
	return -1;
}

static void
skip_spaces_and_comments(Lexer *self)
{
	while (!skip_space_or_comment(self))
		continue;
}

static void
next_punc(Lexer *self, Token *tok, enum TokenType type, size_t len)
{
	tok->type = type;
	tok->pos = self->pos;
	tok->len = len;
	self->pos += len;
}

static int
next_integer(Lexer *self, Token *tok)
{
	const char *pos;
	char suffix[4] = {0};
	size_t suflen = 0;

	pos = self->pos;
	tok->type = Integer;
	while (isdigit(*pos))
		pos++;

	while (isalpha(pos[suflen])) {
		if (suflen >= 3)
			goto FAIL;
		suffix[suflen] = pos[suflen];
		suflen++;
	}

	if (!strcmp(suffix, "")
	    || !strcmp(suffix, "l")   || strcmp(suffix, "L")
	    || !strcmp(suffix, "u")   || strcmp(suffix, "U")
	    || !strcmp(suffix, "ll")  || strcmp(suffix, "LL")
	    || !strcmp(suffix, "lu")  || strcmp(suffix, "LU")
	    || !strcmp(suffix, "ul")  || strcmp(suffix, "UL")
	    || !strcmp(suffix, "Lu")  || strcmp(suffix, "lU")
	    || !strcmp(suffix, "Ul")  || strcmp(suffix, "uL")
	    || !strcmp(suffix, "llu") || strcmp(suffix, "LLU")
	    || !strcmp(suffix, "ull") || strcmp(suffix, "ULL")
	    || !strcmp(suffix, "LLu") || strcmp(suffix, "llU")
	    || !strcmp(suffix, "Ull") || strcmp(suffix, "uLL")) {
		tok->pos = self->pos;
		tok->len = pos + suflen - tok->pos;
		self->pos = (char *) pos + suflen;
		return 0;
	}
FAIL:
	print_pos(self, pos);
	fprintf(stderr, "error: ivalid suffix for integer\n");
	exit(1);
}

void
lexer_init(Lexer *lexer, const char *str)
{
	lexer->start = (char *) str;
	lexer->pos = (char *) str;
}

int
next_token(Lexer *self, Token *tok)
{
	skip_spaces_and_comments(self);

	switch (*self->pos) {
	case '\0':
		return -1;
	case '+':
		if (self->pos[1] == '+')
			next_punc(self, tok, PlusPlus, 2);
		else if (self->pos[1] == '=')
			next_punc(self, tok, PlusEq, 2);
		else
			next_punc(self, tok, Plus, 1);
		return 0;
	case '-':
		if (self->pos[1] == '>')
			next_punc(self, tok, Arrow, 2);
		else if (self->pos[1] == '-')
			next_punc(self, tok, MinusMinus, 2);
		else if (self->pos[1] == '=')
			next_punc(self, tok, MinusEq, 2);
		else
			next_punc(self, tok, Minus, 1);
		return 0;
	case '*':
		if (self->pos[1] == '=')
			next_punc(self, tok, StarEq, 2);
		else
			next_punc(self, tok, Star, 1);
		return 0;
	case '/':
		if (self->pos[1] == '=')
			next_punc(self, tok, SlashEq, 2);
		else
			next_punc(self, tok, Slash, 1);
		return 0;
	case '%':
		if (self->pos[1] == '=')
			next_punc(self, tok, ModEq, 2);
		else
			next_punc(self, tok, Mod, 1);
		return 0;
	case '|':
		if (self->pos[1] == '|')
			if (self->pos[2] == '=')
				next_punc(self, tok, OrOrEq, 3);
			else
				next_punc(self, tok, OrOr, 2);
		else if (self->pos[1] == '=')
			next_punc(self, tok, OrEq, 2);
		else
			next_punc(self, tok, Or, 1);
		return 0;
	case '&':
		if (self->pos[1] == '&')
			if (self->pos[2] == '=')
				next_punc(self, tok, AndAndEq, 3);
			else
				next_punc(self, tok, AndAnd, 2);
		else if (self->pos[1] == '=')
			next_punc(self, tok, AndEq, 2);
		else
			next_punc(self, tok, And, 1);
		return 0;
	case '^':
		if (self->pos[1] == '=')
			next_punc(self, tok, XorEq, 2);
		else
			next_punc(self, tok, Xor, 1);
		return 0;
	case '=':
		if (self->pos[1] == '=')
			next_punc(self, tok, EqEq, 2);
		else
			next_punc(self, tok, Equal, 1);
		return 0;
	case '>':
		if (self->pos[1] == '>')
			if (self->pos[2] == '=')
				next_punc(self, tok, RightShiftEq, 3);
			else
				next_punc(self, tok, RightShift, 2);
		else if (self->pos[1] == '=')
			next_punc(self, tok, GreaterEq, 2);
		else
			next_punc(self, tok, Greater, 1);
		return 0;
	case '<':
		if (self->pos[1] == '<')
			if (self->pos[2] == '=')
				next_punc(self, tok, LeftShiftEq, 3);
			else
				next_punc(self, tok, LeftShift, 2);
		else if (self->pos[1] == '=')
			next_punc(self, tok, LessEq, 2);
		else
			next_punc(self, tok, Less, 1);
		return 0;
	case '!':
		if (self->pos[1] == '=')
			next_punc(self, tok, NotEq, 2);
		else
			next_punc(self, tok, Not, 1);
		return 0;
	case '.':
		if (self->pos[1] == '.' && self->pos[2] == '.')
			next_punc(self, tok, Elipses, 3);
		else
			next_punc(self, tok, Dot, 1);
		return 0;
	case '\\':
		next_punc(self, tok, Backslash, 1);
		return 0;
	case '{':
		next_punc(self, tok, LeftBrace, 1);
		return 0;
	case '}':
		next_punc(self, tok, RightBrace, 1);
		return 0;
	case '[':
		next_punc(self, tok, LeftBracket, 1);
		return 0;
	case ']':
		next_punc(self, tok, RightBracket, 1);
		return 0;
	case '(':
		next_punc(self, tok, LeftParen, 1);
		return 0;
	case ')':
		next_punc(self, tok, RightParen, 1);
		return 0;
	case '?':
		next_punc(self, tok, Que, 1);
		return 0;
	case '~':
		next_punc(self, tok, Tilde, 1);
		return 0;
	case ',':
		next_punc(self, tok, Comma, 1);
		return 0;
	case ':':
		next_punc(self, tok, Colon, 1);
		return 0;
	case ';':
		next_punc(self, tok, Semicolon, 1);
		return 0;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		next_integer(self, tok);
		return 0;
	}

	print_pos(self, self->pos);
	fprintf(stderr, "error: unexpected character\n");
	exit(1);
}
