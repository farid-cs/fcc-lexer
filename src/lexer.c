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
skip_space(Lexer *self)
{
	if (!isspace(*self->pos))
		return -1;
	self->pos++;
	return 0;
}

static int
skip_line_comment(Lexer *self)
{
	const char *pos = self->pos;

	if (*pos++ != '/' || *pos++ != '/')
		return -1;
	while (*pos && *pos != '\n')
		pos++;
	self->pos = pos;
	return 0;
}

static int
skip_block_comment(Lexer *self)
{
	const char *pos = self->pos;

	if (*pos++ != '/' || *pos++ != '*')
		return -1;
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

static void
skip_spaces_and_comments(Lexer *self)
{
	while (!skip_space(self) || !skip_line_comment(self) || !skip_block_comment(self))
		continue;
}

static int
next_triple_char_punctuation(Lexer *self, Token *tok)
{
	const char *const p = self->pos;

	if (!p[0] || !p[1] || !p[2] || p[0] != p[1])
		return -1;
	switch (*p) {
	case '<':
		if (p[2] != '=')
			return -1;
		tok->type = TokLShiftEq;
		break;
	case '>':
		if (p[2] != '=')
			return -1;
		tok->type = TokRShiftEq;
		break;
	case '&':
		if (p[2] != '=')
			return -1;
		tok->type = TokAndAndEq;
		break;
	case '|':
		if (p[2] != '=')
			return -1;
		tok->type = TokOrOrEq;
		break;
	case '.':
		if (p[2] != '.')
			return -1;
		tok->type = TokElipses;
		break;
	default:
		return -1;
	}
	self->pos += 3;
	return 0;
}

static int
next_double_char_punctuation(Lexer *self, Token *tok)
{
	const char *const p = self->pos;

	switch (*p) {
	case '+':
		tok->type = TokPlusEq;
		break;
	case '-':
		tok->type = TokMinusEq;
		break;
	case '*':
		tok->type = TokStarEq;
		break;
	case '/':
		tok->type = TokSlashEq;
		break;
	case '%':
		tok->type = TokModEq;
		break;
	case '!':
		tok->type = TokExEq;
		break;
	case '|':
		tok->type = TokOrEq;
		break;
	case '&':
		tok->type = TokAndEq;
		break;
	case '^':
		tok->type = TokXorEq;
		break;
	case '>':
		tok->type = TokMoreEq;
		break;
	case '<':
		tok->type = TokLessEq;
		break;
	case '=':
		tok->type = TokEqEq;
		break;
	default:
		return -1;
	}
	if (p[1] == '=') {
		self->pos += 2;
		return 0;
	}
	switch (*p) {
	case '+':
		tok->type = TokInc;
		break;
	case '-':
		tok->type = TokDec;
		break;
	case '>':
		tok->type = TokRShift;
		break;
	case '<':
		tok->type = TokLShift;
		break;
	case '&':
		tok->type = TokBoolAnd;
		break;
	case '|':
		tok->type = TokBoolOr;
		break;
	default:
		return -1;
	}
	if (p[0] == p[1]) {
		self->pos += 2;
		return 0;
	}
	if (p[0] == '-' && p[1] == '>') {
		tok->type = TokArrow;
		self->pos += 2;
		return 0;
	}
	return -1;
}

static int
next_single_char_punctuation(Lexer *self, Token *tok)
{
	switch (*self->pos) {
	case '+':
		tok->type = TokPlus;
		break;
	case '-':
		tok->type = TokMinus;
		break;
	case '*':
		tok->type = TokStar;
		break;
	case '/':
		tok->type = TokSlash;
		break;
	case '\\':
		tok->type = TokBackSlash;
		break;
	case '%':
		tok->type = TokMod;
		break;
	case '{':
		tok->type = TokLBrace;
		break;
	case '}':
		tok->type = TokRBrace;
		break;
	case '[':
		tok->type = TokLBracket;
		break;
	case ']':
		tok->type = TokRBracket;
		break;
	case '(':
		tok->type = TokLParen;
		break;
	case ')':
		tok->type = TokRParen;
		break;
	case '|':
		tok->type = TokBitOr;
		break;
	case '&':
		tok->type = TokBitAnd;
		break;
	case '^':
		tok->type = TokBitXor;
		break;
	case '=':
		tok->type = TokEq;
		break;
	case '>':
		tok->type = TokMore;
		break;
	case '<':
		tok->type = TokLess;
		break;
	case '!':
		tok->type = TokEx;
		break;
	case '?':
		tok->type = TokQue;
		break;
	case '~':
		tok->type = TokBitNot;
		break;
	case ',':
		tok->type = TokComma;
		break;
	case '.':
		tok->type = TokDot;
		break;
	case ':':
		tok->type = TokColon;
		break;
	case ';':
		tok->type = TokSemiColon;
		break;
	default:
		return -1;
	}
	self->pos++;
	return 0;
}

static int
next_punctuation(Lexer *self, Token *tok)
{
	if (!next_triple_char_punctuation(self, tok))
		return 0;
	if (!next_double_char_punctuation(self, tok))
		return 0;
	if (!next_single_char_punctuation(self, tok))
		return 0;
	return -1;
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
