const std = @import("std");

pub const Lexer = struct {
	allocator: std.mem.Allocator,
	tokens: std.ArrayList(Token),
	buf: []u8,
	i: usize,

	pub fn lex(allocator: std.mem.Allocator, filename: []u8) !Lexer {
		var file = try std.fs.cwd().openFile(filename, .{});
		defer file.close();

		var buf_reader = std.io.bufferedReader(file.reader());
		var in_stream = buf_reader.reader();

		const file_stat = try file.stat();

		const buffer = try allocator.alloc(u8, file_stat.size);
		defer allocator.free(buffer);

		var lexer = &Lexer {
			.allocator = allocator,
			.tokens = std.ArrayList(Token).init(allocator),
			.buf = buffer,
			.i = 0
		};

		try in_stream.readNoEof(buffer);

		while (lexer.i < buffer.len) : (lexer.i += 1) {
			const token = try lexer.read_token();

			if (token.t == .TOK_NONE) {
				continue;
			}

			try lexer.tokens.append(token);
		}

		return lexer;
	}

	fn read_token(self: *Lexer) !Token {
		switch (self.buf[self.i]) {
		}
	}
};

pub const Token = struct {
	lexeme: []u8,
	line: usize,
	char: usize,
	t: TokenType,

	pub fn new(t: TokenType, lexeme: []u8, line: usize, char: usize) Token {
		return Token {
			.lexeme = lexeme,
			.line = line,
			.char = char,
			.t = t
		};
	}

	pub fn eof(line: usize, char: usize) Token {
		return Token {
			.lexeme = "",
			.line = line,
			.char = char,
			.t = .TOK_EOF
		};
	}

	pub fn none(line: usize, char: usize) Token {
		return Token {
			.lexeme = "",
			.line = line,
			.char = char,
			.t = .TOK_NONE
		};
	}
};

pub const TokenType = enum {
	TOK_DOT,
	TOK_COMMA,
	TOK_SEMICOLON,
	TOK_COLON,

	TOK_ROUND_OPEN,
	TOK_ROUND_CLOSE,
	TOK_SQUARE_OPEN,
	TOK_SQUARE_CLOSE,
	TOK_BRACE_OPEN,
	TOK_BRACE_CLOSE,

	TOK_PLUS,
	TOK_INCREMENT,
	TOK_PLUS_EQUAL,

	TOK_MINUS,
	TOK_ARROW,
	TOK_DECREMENT,
	TOK_MINUS_EQUAL,

	TOK_STAR,
	TOK_POWER,
	TOK_STAR_EQUAL,

	TOK_SLASH,
	TOK_SLASH_EQUAL,

	TOK_MOD,
	TOK_MOD_EQUAL,

	TOK_LESS,
	TOK_LESS_EQUAL,
	TOK_LEFT_SHIFT,

	TOK_GREATER,
	TOK_GREATER_EQUAL,
	TOK_RIGHT_SHIFT,

	TOK_EQUAL,
	TOK_CMP_EQUAL,

	TOK_BITWISE_AND,
	TOK_BIT_AND_EQUAL,
	TOK_LOGICAL_AND,

	TOK_BITWISE_OR,
	TOK_BIT_OR_EQUAL,
	TOK_LOGICAL_OR,

	TOK_BITWISE_XOR,
	TOK_BIT_XOR_EQUAL,

	TOK_BITWISE_NOT,
	TOK_BIT_NOT_EQUAL,

	TOK_LOGICAL_NOT,
	TOK_LOG_NOT_EQUAL,

	TOK_IDENTIFIER,
	TOK_FUN,
	TOK_RETURN,
	TOK_STRUCT,
	TOK_FOR,
	TOK_FOREACH,
	TOK_LET,
	TOK_WHILE,
	TOK_DOUBLE,
	TOK_FLOAT,
	TOK_MATCH,
	TOK_IF,
	TOK_ELSE,
	TOK_BREAK,
	TOK_CONTINUE,
	TOK_CONST,
	TOK_ENUM,
	TOK_TRUE,
	TOK_FALSE,
	TOK_NULL,
	TOK_TYPEOF,
	TOK_DATA,
	TOK_INTERFACE,
	
	TOK_INT,
	TOK_INT8,
	TOK_INT16,
	TOK_INT64,
	TOK_UINT,
	TOK_UINT8,
	TOK_UINT16,
	TOK_UINT64,
	TOK_STRING,
	TOK_VOID,

	TOK_INTNUM,
	TOK_OCTNUM,
	TOK_BINNUM,
	TOK_HEXNUM,
	TOK_FLOATNUM,
	TOK_STR,

	TOK_EOF,
	TOK_NONE,
};
