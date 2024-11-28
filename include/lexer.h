#pragma once

#include <common.h>

typedef enum {
	TOK_DOT,          // .
	TOK_COMMA,        // ,
	TOK_SEMICOLON,    // ;

	TOK_COLON,        // :

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
	TOK_LEFT_SHIFT,   // <<

	TOK_GREATER,      // >
	TOK_GREATER_EQUAL, // >= (fuck you i ain't increasing shit in the other ones by a space)
	TOK_RIGHT_SHIFT,  // >>

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

	// keywords and identifiers
	TOK_IDENTIFIER,   // any identifier that's not a keyword
	TOK_FUN,          // fun - truly the most fun keyword out there
	TOK_RETURN,       // return
	TOK_STRUCT,       // struct
	TOK_FOR,          // for
	TOK_FOREACH,      // foreach
	TOK_LET,          // let
	TOK_WHILE,        // while
	TOK_DOUBLE,       // double
	TOK_FLOAT,        // float
	TOK_MATCH,        // match
	TOK_IF,           // if
	TOK_ELSE,         // else
	TOK_BREAK,        // break
	TOK_CONTINUE,     // continue
	TOK_CONST,        // const
	TOK_ENUM,         // enum
	TOK_TRUE,         // True...
	TOK_FALSE,        // fALSE,,,
	TOK_NULL,         // null
	TOK_TYPEOF,       // typeof
	TOK_DATA,         // data
	TOK_INTERFACE,    // interface
	
	// datatypes
	TOK_INT,          // int | i32 | int32
	TOK_INT8,         // int8 | i8
	TOK_INT16,        // int16 | i16
	TOK_INT64,        // int64 | i64
	TOK_UINT,         // uint | u32 | uint32
	TOK_UINT8,        // uint8 | u8
	TOK_UINT16,       // uint16 | u16
	TOK_UINT64,       // uint64 | u64
	TOK_STRING,       // string
	TOK_VOID,         // void

	TOK_INTNUM,       // any integer constant
	TOK_OCTNUM,       // any octal constant
	TOK_BINNUM,       // any binary constant
	TOK_HEXNUM,       // any hexadecimal constant
	TOK_FLOATNUM,     // any floating point constant
	TOK_STR,          // any string constante

	TOK_EOF,
	TOK_NONE,
} TokenType;

typedef struct {
	u32 line;
	u32 character;
	char *lexeme;
	TokenType type;
} Token;

// i have to include it in this weird place excuse me
#include <tokenlist.h>

typedef struct {
	u32 current_line;
	u32 current_char;
	u32 iter;
	char *code;
	TokenList list;
} Lexer;

void lexer_init(Lexer *lexer, char *code);

bool lexer_at_end(Lexer *lexer);
bool lexer_match(Lexer *lexer, char c);
char lexer_peek(Lexer *lexer);
char lexer_peek_next(Lexer *lexer);

void lexer_advance(Lexer *lexer);
void lexer_goback(Lexer *lexer);

Token lexer_querytoken(Lexer *lexer);
void lexer_lex(Lexer *lexer);

void lexer_free(Lexer *lexer);
