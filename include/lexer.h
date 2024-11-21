#pragma once

#include <common.h>

typedef enum {
	TOK_DOT,          // .
	TOK_COMMA,        // ,
	TOK_SEMICOLON,    // ;
	
	TOK_COLON,        // :
	TOK_DOUBLE_COLON, // ::

	TOK_ROUND_OPEN,   // (
	TOK_ROUND_CLOSE,  // )
	TOK_SQUARE_OPEN,  // [
	TOK_SQUARE_CLOSE, // ]
	TOK_BRACE_OPEN,   // {
	TOK_BRACE_CLOSE,  // }

	TOK_PLUS,         // +
	TOK_INCREMENT,    // ++
	TOK_PLUS_EQUAL,   // +=

	TOK_MINUS,        // -
	TOK_ARROW,        // ->
	TOK_DECREMENT,    // --
	TOK_MINUS_EQUAL,  // -=

	TOK_STAR,         // *
	TOK_POWER,        // **
	TOK_STAR_EQUAL,   // *=

	TOK_SLASH,        // /
	TOK_SLASH_EQUAL,  // /=
	
	TOK_MOD,          // %
	TOK_MOD_EQUAL,    // %=
	
	TOK_LESS,         // <
	TOK_LESS_EQUAL,   // <=

	TOK_GREATER,      // >
	TOK_GREATER_EQUAL, // >= (fuck you i ain't increasing shit in the other ones by a space)
	
	TOK_EQUAL,        // =
	TOK_CMP_EQUAL,    // ==

	TOK_BITWISE_AND,  // &
	TOK_BIT_AND_EQUAL, // &= (again)
	TOK_LOGICAL_AND,  // &&

	TOK_BITWISE_OR,   // |
	TOK_BIT_OR_EQUAL, // |=
	TOK_LOGICAL_OR,   // ||

	TOK_BITWISE_XOR,  // ^
	TOK_BIT_XOR_EQUAL, // ^= (again)

	TOK_BITWISE_NOT,  // ~
	TOK_BIT_NOT_EQUAL, // ~= (again)

	TOK_LOGICAL_NOT,  // !
	TOK_LOG_NOT_EQUAL, // != (again)
} TokenType;

typedef struct {
	u32 line;
	u32 character;
	char *lexeme;
} Token;
