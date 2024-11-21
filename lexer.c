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

	if (isspace(*pos)) {
		self->pos++;
		return 0;
	}
	if (*pos++ != '/')
		return -1;
	switch (*pos++) {
	case '/':
		while (*pos && *pos != '\n')
			pos++;
		self->pos = pos;
		return 0;
	case '*':
		for (;; pos++) {
			if (*pos == '\0') {
				print_pos(self, pos);
				fprintf(stderr, "error: unterminated block comment\n");
				exit(1);
			}
			if (pos[0] == '*' && pos[1] == '/')
				break;
		}
		self->pos = pos + 2;
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
next_punctuation(Lexer *self, Token *tok)
{
	switch (*self->pos) {
	case '+':
		if (self->pos[1] == '+') {
			tok->type = PlusPlus;
			self->pos++;
			break;
		}
		if (self->pos[1] == '=') {
			tok->type = PlusEq;
			self->pos++;
			break;
		}
		tok->type = Plus;
		break;
	case '-':
		if (self->pos[1] == '>') {
			tok->type = Arrow;
			self->pos++;
			break;
		}
		if (self->pos[1] == '-') {
			tok->type = MinusMinus;
			self->pos++;
			break;
		}
		if (self->pos[1] == '=') {
			tok->type = MinusEq;
			self->pos++;
			break;
		}
		tok->type = Minus;
		break;
	case '*':
		if (self->pos[1] == '=') {
			tok->type = StarEq;
			self->pos++;
			break;
		}
		tok->type = Star;
		break;
	case '/':
		if (self->pos[1] == '=') {
			tok->type = SlashEq;
			self->pos++;
			break;
		}
		tok->type = Slash;
		break;
	case '%':
		if (self->pos[1] == '=') {
			tok->type = ModEq;
			self->pos++;
			break;
		}
		tok->type = Mod;
		break;
	case '|':
		if (self->pos[1] == '|') {
			if (self->pos[2] == '=') {
				tok->type = OrOrEq;
				self->pos += 2;
				break;
			}
			tok->type = OrOr;
			self->pos++;
			break;
		}
		if (self->pos[1] == '=') {
			tok->type = OrEq;
			self->pos++;
			break;
		}
		tok->type = Or;
		break;
	case '&':
		if (self->pos[1] == '&') {
			if (self->pos[2] == '=') {
				tok->type = AndAndEq;
				self->pos += 2;
				break;
			}
			tok->type = AndAnd;
			self->pos++;
			break;
		}
		if (self->pos[1] == '=') {
			tok->type = AndEq;
			self->pos++;
			break;
		}
		tok->type = And;
		break;
	case '^':
		if (self->pos[1] == '=') {
			tok->type = XorEq;
			self->pos++;
			break;
		}
		tok->type = Xor;
		break;
	case '=':
		if (self->pos[1] == '=') {
			tok->type = EqEq;
			self->pos++;
			break;
		}
		tok->type = Equal;
		break;
	case '>':
		if (self->pos[1] == '>') {
			if (self->pos[2] == '=') {
				tok->type = RightShiftEq;
				self->pos += 2;
				break;
			}
			tok->type = RightShift;
			self->pos++;
			break;
		}
		if (self->pos[1] == '=') {
			tok->type = GreaterEq;
			self->pos++;
			break;
		}
		tok->type = Greater;
		break;
	case '<':
		if (self->pos[1] == '<') {
			if (self->pos[2] == '=') {
				tok->type = LeftShiftEq;
				self->pos += 2;
				break;
			}
			tok->type = LeftShift;
			self->pos++;
			break;
		}
		if (self->pos[1] == '=') {
			tok->type = LessEq;
			self->pos++;
			break;
		}
		tok->type = Less;
		break;
	case '!':
		if (self->pos[1] == '=') {
			tok->type = NotEq;
			self->pos++;
			break;
		}
		tok->type = Not;
		break;
	case '.':
		if (self->pos[1] == '.' && self->pos[2] == '.') {
			tok->type = Elipses;
			self->pos += 2;
			break;
		}
		tok->type = Dot;
		break;
	case '\\': tok->type = Backslash;    break;
	case '{':  tok->type = LeftBrace;    break;
	case '}':  tok->type = RightBrace;   break;
	case '[':  tok->type = LeftBracket;  break;
	case ']':  tok->type = RightBracket; break;
	case '(':  tok->type = LeftParen;    break;
	case ')':  tok->type = RightParen;   break;
	case '?':  tok->type = Que;          break;
	case '~':  tok->type = Tilde;        break;
	case ',':  tok->type = Comma;        break;
	case ':':  tok->type = Colon;        break;
	case ';':  tok->type = Semicolon;    break;
	default:
		return -1;
	}
	self->pos++;
	return 0;
}

Lexer
new_lexer(const char *str)
{
	return (Lexer) {
		.start = str,
		.pos = str,
	};
}

int
next_token(Lexer *self, Token *tok)
{
	skip_spaces_and_comments(self);
	if (!next_punctuation(self, tok))
		return 0;
	if (*self->pos == '\0')
		return EOF;

	print_pos(self, self->pos);
	fprintf(stderr, "error: unexpected character\n");
	exit(1);
}
