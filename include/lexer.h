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

	TOK_IDENTIFIER,   // any identifier that's not a keyword
	TOK_FUN,          // fun - truly the most fun keyword out there
	TOK_RETURN,       // return
	TOK_STRUCT,       // struct
	TOK_INT,          // int | i32 | int32
	TOK_INT8,         // int8 | i8
	TOK_INT16,        // int16 | i16
	TOK_INT64,        // int64 | i64
	TOK_UINT,         // uint | u32 | uint32
	TOK_UINT8,        // uint8 | u8
	TOK_UINT16,       // uint16 | u16
	TOK_UINT64,       // uint64 | u64
	TOK_STRING,       // string
} TokenType;

typedef struct {
	u32 line;
	u32 character;
	char *lexeme;
} Token;
