#ifndef LEXER_H
#define LEXER_H

typedef enum TokenType TokenType;
enum TokenType {
	TokPlus,
	TokMinus,
	TokStar,
	TokSlash,
	TokMod,
	TokLBrace,
	TokRBrace,
	TokLBracket,
	TokRBracket,
	TokLParen,
	TokRParen,
	TokEq,
	TokBitOr,
	TokBitAnd,
	TokBitXor,
	TokEx,
	TokQue,
	TokBitNot,
	TokComma,
	TokDot,
	TokColon,
	TokSemiColon,
	TokBackSlash,
	TokMore,
	TokLess,

	TokPlusEq,
	TokMinusEq,
	TokStarEq,
	TokSlashEq,
	TokModEq,
	TokEqEq,
	TokExEq,
	TokMoreEq,
	TokLessEq,
	TokBoolOr,
	TokBoolAnd,
	TokOrEq,
	TokAndEq,
	TokXorEq,
	TokInc,
	TokDec,
	TokRShift,
	TokLShift,
	TokArrow,

	TokRShiftEq,
	TokLShiftEq,
	TokAndAndEq,
	TokOrOrEq,
	TokElipses,
};

typedef struct Token Token;
struct Token {
	TokenType type;
};

typedef struct Lexer Lexer;
struct Lexer {
	const char *start;
	const char *pos;
};

Lexer	new_lexer(const char *);
int	next_token(Lexer *, Token *);

#endif
