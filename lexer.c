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
#include <errno.h>
#include <inttypes.h>
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

	if (!strcmp(suffix, "") || !strcmp(suffix, "L")
	    || !strcmp(suffix, "LL") || !strcmp(suffix, "LU")
	    || !strcmp(suffix, "LLU") || !strcmp(suffix, "U")
	    || !strcmp(suffix, "UL") || !strcmp(suffix, "ULL")) {
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
		if (self->pos[1] == '+') {
			tok->type = PlusPlus;
			self->pos += 2;
		} else if (self->pos[1] == '=') {
			tok->type = PlusEq;
			self->pos += 2;
		} else {
			tok->type = Plus;
			self->pos++;
		}
		return 0;
	case '-':
		if (self->pos[1] == '>') {
			tok->type = Arrow;
			self->pos += 2;
		} else if (self->pos[1] == '-') {
			tok->type = MinusMinus;
			self->pos += 2;
		} else if (self->pos[1] == '=') {
			tok->type = MinusEq;
			self->pos += 2;
		} else {
			tok->type = Minus;
			self->pos++;
		}
		return 0;
	case '*':
		if (self->pos[1] == '=') {
			tok->type = StarEq;
			self->pos += 2;
		} else {
			tok->type = Star;
			self->pos++;
		}
		return 0;
	case '/':
		if (self->pos[1] == '=') {
			tok->type = SlashEq;
			self->pos += 2;
		} else {
			tok->type = Slash;
			self->pos++;
		}
		return 0;
	case '%':
		if (self->pos[1] == '=') {
			tok->type = ModEq;
			self->pos += 2;
		} else {
			tok->type = Mod;
			self->pos++;
		}
		return 0;
	case '|':
		if (self->pos[1] == '|') {
			if (self->pos[2] == '=') {
				tok->type = OrOrEq;
				self->pos += 3;
			} else {
				tok->type = OrOr;
				self->pos += 2;
			}
		} else if (self->pos[1] == '=') {
			tok->type = OrEq;
			self->pos += 2;
		} else {
			tok->type = Or;
			self->pos++;
		}
		return 0;
	case '&':
		if (self->pos[1] == '&') {
			if (self->pos[2] == '=') {
				tok->type = AndAndEq;
				self->pos += 3;
			} else {
				tok->type = AndAnd;
				self->pos += 2;
			}
		} else if (self->pos[1] == '=') {
			tok->type = AndEq;
			self->pos += 2;
		} else {
			tok->type = And;
			self->pos++;
		}
		return 0;
	case '^':
		if (self->pos[1] == '=') {
			tok->type = XorEq;
			self->pos += 2;
		} else {
			tok->type = Xor;
			self->pos++;
		}
		return 0;
	case '=':
		if (self->pos[1] == '=') {
			tok->type = EqEq;
			self->pos += 2;
		} else {
			tok->type = Equal;
			self->pos++;
		}
		return 0;
	case '>':
		if (self->pos[1] == '>') {
			if (self->pos[2] == '=') {
				tok->type = RightShiftEq;
				self->pos += 3;
			} else {
				tok->type = RightShift;
				self->pos += 2;
			}
		} else if (self->pos[1] == '=') {
			tok->type = GreaterEq;
			self->pos += 2;
		} else {
			tok->type = Greater;
			self->pos++;
		}
		return 0;
	case '<':
		if (self->pos[1] == '<') {
			if (self->pos[2] == '=') {
				tok->type = LeftShiftEq;
				self->pos += 3;
			} else {
				tok->type = LeftShift;
				self->pos += 2;
			}
		} else if (self->pos[1] == '=') {
			tok->type = LessEq;
			self->pos += 2;
		} else {
			tok->type = Less;
			self->pos++;
		}
		return 0;
	case '!':
		if (self->pos[1] == '=') {
			tok->type = NotEq;
			self->pos += 2;
		} else {
			tok->type = Not;
			self->pos++;
		}
		return 0;
	case '.':
		if (self->pos[1] == '.' && self->pos[2] == '.') {
			tok->type = Elipses;
			self->pos += 3;
		} else {
			tok->type = Dot;
			self->pos++;
		}
		return 0;
	case '\\':
		tok->type = Backslash;
		self->pos++;
		return 0;
	case '{':
		tok->type = LeftBrace;
		self->pos++;
		return 0;
	case '}':
		tok->type = RightBrace;
		self->pos++;
		return 0;
	case '[':
		tok->type = LeftBracket;
		self->pos++;
		return 0;
	case ']':
		tok->type = RightBracket;
		self->pos++;
		return 0;
	case '(':
		tok->type = LeftParen;
		self->pos++;
		return 0;
	case ')':
		tok->type = RightParen;
		self->pos++;
		return 0;
	case '?':
		tok->type = Que;
		self->pos++;
		return 0;
	case '~':
		tok->type = Tilde;
		self->pos++;
		return 0;
	case ',':
		tok->type = Comma;
		self->pos++;
		return 0;
	case ':':
		tok->type = Colon;
		self->pos++;
		return 0;
	case ';':
		tok->type = Semicolon;
		self->pos++;
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
