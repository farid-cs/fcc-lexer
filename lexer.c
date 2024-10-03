#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

static void print_pos(Lexer *self, const char *pos)
{
	size_t row = 1, col = 1;
	const char *start = self->start;

	while (start != pos) {
		switch (*start) {
		case '\n':
			row++;
			col = 1;
			break;
		default:
			col++;
		}
		start++;
	}
	fprintf(stderr, "%zu:%zu: ", row, col);
}

static int skip_space(Lexer *self)
{
	if (!isspace(*self->pos)) {
		return -1;
	}
	self->pos++;
	return 0;
}

static int skip_line_comment(Lexer *self)
{
	const char *pos = self->pos;

	if (*pos++ != '/' || *pos++ != '/') {
		return -1;
	}
	for (;; pos++) {
		switch (*pos) {
		case '\n':
			pos++;
		case '\0':
			break;
		default:
			continue;
		}
		break;
	}
	self->pos = pos;
	return 0;
}

static int skip_block_comment(Lexer *self)
{
	const char *pos = self->pos;

	if (*pos++ != '/' || *pos++ != '*') {
		return -1;
	}
	for (;; pos++) {
		if (*pos == '\0') {
			print_pos(self, pos);
			fprintf(stderr, "error: unterminated block comment\n");
			exit(1);
			return 1;
		}
		if (pos[0] == '*' && pos[1] == '/') {
			break;
		}
	}
	self->pos = pos + 2;
	return 0;
}

static void skip_spaces_and_comments(Lexer *self)
{
	while (
	      !skip_space(self) ||
	      !skip_line_comment(self) ||
	      !skip_block_comment(self)
	);
}

Lexer new_lexer(const char *str)
{
	return (Lexer) {
		.start = str,
		.pos = str,
	};
}

int next_token(Lexer *self)
{
	skip_spaces_and_comments(self);
	if (*self->pos == '\0') {
		return EOF;
	}

	print_pos(self, self->pos);
	fprintf(stderr, "error: unexpected character\n");
	exit(1);
}
