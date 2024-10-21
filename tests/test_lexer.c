#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../src/lexer.h"

static size_t
tokenize(Token *tokens, char *input)
{
	Lexer lexer = new_lexer(input);
	size_t n = 0;

	while (!next_token(&lexer, tokens++))
		n++;
	return n;
}

static void
test_spaces_and_comments()
{
	char *input = "                     "
	              "// A line comment    "
	              "/* A block comment */";
	Token tokens[1000];

	assert(tokenize(tokens, input) == 0);
}

static void
test_punctuation()
{
	char *input = "+-*/%{}[]()=|&^!?~,.:;\\><+=-=*=/=%===!=>=<=||&&|=&=^=++-->><<->>>=<<=&&=||=...";
	Token tokens[1000];
	size_t n = 0;

	assert(tokenize(tokens, input) == 49);
	assert(tokens[n++].type == TokPlus);
	assert(tokens[n++].type == TokMinus);
	assert(tokens[n++].type == TokStar);
	assert(tokens[n++].type == TokSlash);
	assert(tokens[n++].type == TokMod);
	assert(tokens[n++].type == TokLBrace);
	assert(tokens[n++].type == TokRBrace);
	assert(tokens[n++].type == TokLBracket);
	assert(tokens[n++].type == TokRBracket);
	assert(tokens[n++].type == TokLParen);
	assert(tokens[n++].type == TokRParen);
	assert(tokens[n++].type == TokEq);
	assert(tokens[n++].type == TokBitOr);
	assert(tokens[n++].type == TokBitAnd);
	assert(tokens[n++].type == TokBitXor);
	assert(tokens[n++].type == TokEx);
	assert(tokens[n++].type == TokQue);
	assert(tokens[n++].type == TokBitNot);
	assert(tokens[n++].type == TokComma);
	assert(tokens[n++].type == TokDot);
	assert(tokens[n++].type == TokColon);
	assert(tokens[n++].type == TokSemiColon);
	assert(tokens[n++].type == TokBackSlash);
	assert(tokens[n++].type == TokMore);
	assert(tokens[n++].type == TokLess);
	assert(tokens[n++].type == TokPlusEq);
	assert(tokens[n++].type == TokMinusEq);
	assert(tokens[n++].type == TokStarEq);
	assert(tokens[n++].type == TokSlashEq);
	assert(tokens[n++].type == TokModEq);
	assert(tokens[n++].type == TokEqEq);
	assert(tokens[n++].type == TokExEq);
	assert(tokens[n++].type == TokMoreEq);
	assert(tokens[n++].type == TokLessEq);
	assert(tokens[n++].type == TokBoolOr);
	assert(tokens[n++].type == TokBoolAnd);
	assert(tokens[n++].type == TokOrEq);
	assert(tokens[n++].type == TokAndEq);
	assert(tokens[n++].type == TokXorEq);
	assert(tokens[n++].type == TokInc);
	assert(tokens[n++].type == TokDec);
	assert(tokens[n++].type == TokRShift);
	assert(tokens[n++].type == TokLShift);
	assert(tokens[n++].type == TokArrow);
	assert(tokens[n++].type == TokRShiftEq);
	assert(tokens[n++].type == TokLShiftEq);
	assert(tokens[n++].type == TokAndAndEq);
	assert(tokens[n++].type == TokOrOrEq);
	assert(tokens[n++].type == TokElipses);
}

int
main(void)
{
	test_spaces_and_comments();
	test_punctuation();
	puts("All tests passed");
	return 0;
}
