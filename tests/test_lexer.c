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
test_spaces_and_comments(void)
{
	char *input = "                     \n"
	              "// A line comment    \n"
	              "/* A block comment */\n";
	Token tokens[1000];

	assert(tokenize(tokens, input) == 0);
}

static void
test_punctuation(void)
{
	char *input = "+-*/%{}[]()=|&^!?~,.:;\\><+=-=*=/=%===!=>=<=||&&|=&=^=++-->><<->>>=<<=&&=||=...";
	Token tokens[1000];
	size_t n = 0;
	size_t token_number;

	token_number = tokenize(tokens, input);
	assert(tokens[n++].type == Plus);
	assert(tokens[n++].type == Minus);
	assert(tokens[n++].type == Star);
	assert(tokens[n++].type == Slash);
	assert(tokens[n++].type == Mod);
	assert(tokens[n++].type == LeftBrace);
	assert(tokens[n++].type == RightBrace);
	assert(tokens[n++].type == LeftBracket);
	assert(tokens[n++].type == RightBracket);
	assert(tokens[n++].type == LeftParen);
	assert(tokens[n++].type == RightParen);
	assert(tokens[n++].type == Equal);
	assert(tokens[n++].type == Or);
	assert(tokens[n++].type == And);
	assert(tokens[n++].type == Xor);
	assert(tokens[n++].type == Not);
	assert(tokens[n++].type == Que);
	assert(tokens[n++].type == Tilde);
	assert(tokens[n++].type == Comma);
	assert(tokens[n++].type == Dot);
	assert(tokens[n++].type == Colon);
	assert(tokens[n++].type == Semicolon);
	assert(tokens[n++].type == Backslash);
	assert(tokens[n++].type == Greater);
	assert(tokens[n++].type == Less);
	assert(tokens[n++].type == PlusEq);
	assert(tokens[n++].type == MinusEq);
	assert(tokens[n++].type == StarEq);
	assert(tokens[n++].type == SlashEq);
	assert(tokens[n++].type == ModEq);
	assert(tokens[n++].type == EqEq);
	assert(tokens[n++].type == NotEq);
	assert(tokens[n++].type == GreaterEq);
	assert(tokens[n++].type == LessEq);
	assert(tokens[n++].type == OrOr);
	assert(tokens[n++].type == AndAnd);
	assert(tokens[n++].type == OrEq);
	assert(tokens[n++].type == AndEq);
	assert(tokens[n++].type == XorEq);
	assert(tokens[n++].type == PlusPlus);
	assert(tokens[n++].type == MinusMinus);
	assert(tokens[n++].type == RightShift);
	assert(tokens[n++].type == LeftShift);
	assert(tokens[n++].type == Arrow);
	assert(tokens[n++].type == RightShiftEq);
	assert(tokens[n++].type == LeftShiftEq);
	assert(tokens[n++].type == AndAndEq);
	assert(tokens[n++].type == OrOrEq);
	assert(tokens[n++].type == Elipses);
	assert(token_number == n);
}

int
main(void)
{
	test_spaces_and_comments();
	test_punctuation();
	puts("All tests passed");
	return 0;
}
