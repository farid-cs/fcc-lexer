#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"

static size_t
tokenize(Token *tokens, char *input)
{
	Lexer lexer;
	size_t n = 0;

	lexer_init(&lexer, input);
	while (!next_token(&lexer, tokens++))
		n++;
	assert(lexer.err == NOERROR);
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
	assert(tokens[n++].kind == Plus);
	assert(tokens[n++].kind == Minus);
	assert(tokens[n++].kind == Star);
	assert(tokens[n++].kind == Slash);
	assert(tokens[n++].kind == Mod);
	assert(tokens[n++].kind == LeftBrace);
	assert(tokens[n++].kind == RightBrace);
	assert(tokens[n++].kind == LeftBracket);
	assert(tokens[n++].kind == RightBracket);
	assert(tokens[n++].kind == LeftParen);
	assert(tokens[n++].kind == RightParen);
	assert(tokens[n++].kind == Equal);
	assert(tokens[n++].kind == Or);
	assert(tokens[n++].kind == And);
	assert(tokens[n++].kind == Xor);
	assert(tokens[n++].kind == Not);
	assert(tokens[n++].kind == Question);
	assert(tokens[n++].kind == Tilde);
	assert(tokens[n++].kind == Comma);
	assert(tokens[n++].kind == Period);
	assert(tokens[n++].kind == Colon);
	assert(tokens[n++].kind == Semicolon);
	assert(tokens[n++].kind == Backslash);
	assert(tokens[n++].kind == Greater);
	assert(tokens[n++].kind == Less);
	assert(tokens[n++].kind == PlusEq);
	assert(tokens[n++].kind == MinusEq);
	assert(tokens[n++].kind == StarEq);
	assert(tokens[n++].kind == SlashEq);
	assert(tokens[n++].kind == ModEq);
	assert(tokens[n++].kind == EqEq);
	assert(tokens[n++].kind == NotEq);
	assert(tokens[n++].kind == GreaterEq);
	assert(tokens[n++].kind == LessEq);
	assert(tokens[n++].kind == OrOr);
	assert(tokens[n++].kind == AndAnd);
	assert(tokens[n++].kind == OrEq);
	assert(tokens[n++].kind == AndEq);
	assert(tokens[n++].kind == XorEq);
	assert(tokens[n++].kind == PlusPlus);
	assert(tokens[n++].kind == MinusMinus);
	assert(tokens[n++].kind == RightShift);
	assert(tokens[n++].kind == LeftShift);
	assert(tokens[n++].kind == Arrow);
	assert(tokens[n++].kind == RightShiftEq);
	assert(tokens[n++].kind == LeftShiftEq);
	assert(tokens[n++].kind == AndAndEq);
	assert(tokens[n++].kind == OrOrEq);
	assert(tokens[n++].kind == Elipses);
	assert(token_number == n);
}

static void
test_number(void)
{
	char *input = "10 200L 400ULL";
	Token tokens[1000];
	Token *token;
	size_t token_number;

	token = tokens;
	token_number = tokenize(tokens, input);
	assert(token->kind == Integer);
	assert(!memcmp(token->pos, "10", token->len));
	token++;
	assert(token->kind == Integer);
	assert(!memcmp(token->pos, "200L", token->len));
	token++;
	assert(token->kind == Integer);
	assert(!memcmp(token->pos, "400ULL", token->len));
	token++;
	assert(token_number == (size_t)(token - tokens));
}

int
main(void)
{
	test_spaces_and_comments();
	test_punctuation();
	test_number();
	puts("All tests passed");
	return 0;
}
