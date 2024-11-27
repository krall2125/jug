#include <error.h>
#include <lexer.h>
#include <tokenlist.h>
#include <common.h>
#include <memory.h>

#include <ctype.h>

// additional functions that C stdlib doesn't have
static bool is_octal(char c) {
	return (c >= '0' && c <= '7');
}

// the main lexing function
void lexer_lex(Lexer *lexer) {
	for (u32 i = 0; i < strlen(lexer->code); i++) {
		push_tokenlist(&lexer->list, lexer_querytoken(lexer));
	}
}

void lexer_init(Lexer *lexer, char *code) {
	lexer->iter = 0;
	lexer->current_char = 0;
	lexer->current_line = 1;
	lexer->code = code;
	zeroinit_tokenlist(&lexer->list);
}

bool lexer_at_end(Lexer *lexer) {
	return lexer_peek(lexer) == '\0';
}

bool lexer_match(Lexer *lexer, char c) {
	return lexer_peek_next(lexer) == c;
}

char lexer_peek(Lexer *lexer) {
	return lexer->code[lexer->iter];
}

char lexer_peek_next(Lexer *lexer) {
	return lexer->code[lexer->iter + 1];
}

void lexer_advance(Lexer *lexer) {
	if (lexer_at_end(lexer)) {
		return;
	}

	if (lexer_match(lexer, '\n')) {
		lexer->current_char = 0;
		lexer->iter++;
		lexer->current_line++;
		return;
	}

	lexer->current_char++;
	lexer->iter++;
}

void lexer_goback(Lexer *lexer) {
	lexer->current_char--;
	lexer->iter--;

	if (lexer_peek(lexer) == '\n') {
		lexer->current_line--;
	}
}

Token lexer_querytoken(Lexer *lexer) {
}

static Token lexer_lexid(Lexer *lexer) {
	u32 start_char = lexer->current_char;
	u32 start_line = lexer->current_line;

	char *beginning = &lexer->code[lexer->iter];

	while (!lexer_at_end(lexer) && isalnum(lexer_peek(lexer))) {
		lexer_advance(lexer);
	}

	u32 count = (u32) (&lexer->code[lexer->iter] - beginning);

	char *lexeme = ALLOC(char, (count + 1));

	memcpy(lexeme, beginning, count);
	lexeme[count] = '\0';

	switch (lexeme[0]) {
	case 'f':
		switch (strlen(lexeme)) {
		case 3:
			if (memcmp(&lexeme[1], "un", 2) == 0) {
				return (Token) {
					.character = start_char,
					.line = start_line,
					.type = TOK_FUN,
					.lexeme = lexeme
				};
			}
			else if (memcmp(&lexeme[1], "or", 2) == 0) {
				return (Token) {
					.character = start_char,
					.line = start_line,
					.type = TOK_FOR,
					.lexeme = lexeme
				};
			}
			break;
		case 5:
			if (memcmp(&lexeme[1], "alse", 4) == 0) {
				return (Token) {
					.character = start_char,
					.line = start_line,
					.type = TOK_FALSE,
					.lexeme = lexeme
				};
			}
			else if (memcmp(&lexeme[1], "loat", 4) == 0) {
				return (Token) {
					.character = start_char,
					.line = start_line,
					.type = TOK_FLOAT,
					.lexeme = lexeme
				};
			}
			break;
		case 7:
			if (memcmp(&lexeme[1], "oreach", 6) == 0) {
				return (Token) {
					.character = start_char,
					.line = start_line,
					.type = TOK_FOREACH,
					.lexeme = lexeme
				};
			}
			break;
		}
		break;
	case 'r':
		if (strlen(lexeme) != 6) break;

		if (memcmp(&lexeme[1], "eturn", 5) == 0) {
			return (Token) {
				.character = start_char,
				.line = start_line,
				.type = TOK_RETURN,
				.lexeme = lexeme
			};
		}
		break;
	case 's':
		if (strlen(lexeme) != 6) break;

		if (memcmp(&lexeme[1], "truct", 5) == 0) {
			return (Token) {
				.character = start_char,
				.line = start_line,
				.type = TOK_STRUCT,
				.lexeme = lexeme
			};
		}
	case 'l':
		if (strlen(lexeme) != 3) break;

		if (memcmp(&lexeme[1], "et", 2) == 0) {
			return (Token) {
				.character = start_char,
				.line = start_line,
				.type = TOK_LET,
				.lexeme = lexeme
			};
		}
		break;
	case 'w':
		if (strlen(lexeme) != 5) break;

		if (memcmp(&lexeme[1], "hile", 4) == 0) {
			return (Token) {
				.character = start_char,
				.line = start_line,
				.type = TOK_WHILE,
				.lexeme = lexeme
			};
		}
		break;
	case 'd':
		if (strlen(lexeme) != 6) break;

		if (memcmp(&lexeme[1], "ouble", 5) == 0) {
			return (Token) {
				.character = start_char,
				.line = start_line,
				.type = TOK_DOUBLE,
				.lexeme = lexeme
			};
		}
		break;
	case 'm':
		if (strlen(lexeme) != 5) break;

		if (memcmp(&lexeme[1], "atch", 4) == 0) {
			return (Token) {
				.character = start_char,
				.line = start_line,
				.type = TOK_MATCH,
				.lexeme = lexeme
			};
		}
		break;
	case 'i':
		switch (strlen(lexeme)) {
		case 2:
			if (lexeme[1] == 'f') {
				return (Token) {
					.character = start_char,
					.line = start_line,
					.type = TOK_IF,
					.lexeme = lexeme
				};
			}
			break;
		case 9:
			if (memcmp(&lexeme[1], "nterface", 8) == 0) {
				return (Token) {
					.character = start_char,
					.line = start_line,
					.type = TOK_INTERFACE,
					.lexeme = lexeme
				};
			}
			break;
		}
		break;
	}
}

static Token lexer_lexstr(Lexer *lexer) {
	u32 start_char = lexer->current_char;
	u32 start_line = lexer->current_line;
	lexer_advance(lexer);
	char *beginning = &lexer->code[lexer->iter];

	while (!lexer_at_end(lexer) && lexer_peek(lexer) != '\"' && lexer->code[lexer->iter - 1] != '\\') {
		lexer_advance(lexer);
	}

	u32 count = (u32) (&lexer->code[lexer->iter] - beginning);

	char *lexeme = ALLOC(char, (count + 1));

	for (int i = 0; i < count; i++) {
		if (beginning[i] != '\\') {
			lexeme[i] = beginning[i];
			continue;
		}

		switch (beginning[i + 1]) {
		case 'a': {
			lexeme[i] = '\a';
			break;
		}
		case 'b': {
			lexeme[i] = '\b';
			break;
		}
		case 'e': {
			lexeme[i] = '\e';
			break;
		}
		case 'f': {
			lexeme[i] = '\f';
			break;
		}
		case 'n': {
			lexeme[i] = '\n';
			break;
		}
		case 'r': {
			lexeme[i] = '\r';
			break;
		}
		case 't': {
			lexeme[i] = '\t';
			break;
		}
		case 'v': {
			lexeme[i] = '\v';
			break;
		}
		case '\\': {
			lexeme[i] = '\\';
			break;
		}
		case '\'': {
			lexeme[i] = '\'';
			break;
		}
		case '"': {
			lexeme[i] = '"';
			break;
		}
		case '?': {
			lexeme[i] = '\?';
			break;
		}
		default: {
			complainf("[%d, %d] Invalid escape character '%c'\n",
				lexer->current_line,
				lexer->current_char - (count - i),
				beginning[i + 1]);
			continue;
		}
		}

		i++;
	}

	lexeme[count] = '\0';

	return (Token) {
		.character = start_char,
		.line = start_line,
		.type = TOK_STR,
		.lexeme = lexeme
	};
}

// this will lex both integers and floats
static Token lexer_lexnum(Lexer *lexer) {
	u32 start_char = lexer->current_char;
	u32 start_line = lexer->current_line;

	char *beginning = &lexer->code[lexer->iter];

	while (!lexer_at_end(lexer) && isdigit(lexer_peek(lexer))) {
		lexer_advance(lexer);
	}

	if (lexer_peek(lexer) != '.' || !isdigit(lexer_peek_next(lexer))) {
		u32 count = (u32) (&lexer->code[lexer->iter] - beginning);

		char *lexeme = ALLOC(char, (count + 1));

		memcpy(lexeme, beginning, count);
		lexeme[count] = '\0';

		return (Token) {
			.character = start_char,
			.line = start_line,
			.type = TOK_INTNUM,
			.lexeme = lexeme
		};
	}

	lexer_advance(lexer);

	while (!lexer_at_end(lexer) && isdigit(lexer_peek(lexer))) {
		lexer_advance(lexer);
	}

	u32 count = (u32) (&lexer->code[lexer->iter] - beginning);

	char *lexeme = ALLOC(char, (count + 1));

	memcpy(lexeme, beginning, count);
	lexeme[count] = '\0';

	return (Token) {
		.character = start_char,
		.line = start_line,
		.type = TOK_FLOATNUM,
		.lexeme = lexeme
	};
}

static Token lexer_lexoct(Lexer *lexer) {
	u32 start_char = lexer->current_char;
	u32 start_line = lexer->current_line;

	char *beginning = &lexer->code[lexer->iter];

	while (!lexer_at_end(lexer) && is_octal(lexer_peek(lexer))) {
		lexer_advance(lexer);
	}

	u32 count = (u32) (&lexer->code[lexer->iter] - beginning);

	char *lexeme = ALLOC(char, (count + 2));

	lexeme[0] = '0';

	// copy the necessary bytes into the lexeme
	memcpy(&lexeme[1], beginning, count);
	lexeme[count + 1] = '\0';

	return (Token) {
		.character = start_char,
		.line = start_line,
		.type = TOK_OCTNUM,
		.lexeme = lexeme
	};
}

static Token lexer_lexbin(Lexer *lexer) {
	u32 start_char = lexer->current_char;
	u32 start_line = lexer->current_line;

	char *beginning = &lexer->code[lexer->iter];

	while (!lexer_at_end(lexer) && (lexer_peek(lexer) == '0' || lexer_peek(lexer) == '1')) {
		lexer_advance(lexer);
	}

	u32 count = (u32) (&lexer->code[lexer->iter] - beginning);

	char *lexeme = ALLOC(char, (count + 3));

	lexeme[0] = '0';
	lexeme[1] = 'b';
	memcpy(&lexeme[2], beginning, count);
	lexeme[count + 2] = '\0';

	return (Token) {
		.character = start_char,
		.line = start_line,
		.type = TOK_BINNUM,
		.lexeme = lexeme
	};
}

static Token lexer_lexhex(Lexer *lexer) {
	u32 start_char = lexer->current_char;
	u32 start_line = lexer->current_line;

	char *beginning = &lexer->code[lexer->iter];

	while (!lexer_at_end(lexer) && isxdigit(lexer_peek(lexer))) {
		lexer_advance(lexer);
	}

	u32 count = (u32) (&lexer->code[lexer->iter] - beginning);

	char *lexeme = ALLOC(char, (count + 3));

	lexeme[0] = '0';
	lexeme[1] = 'x';
	memcpy(&lexeme[2], beginning, count);
	lexeme[count + 2] = '\0';

	return (Token) {
		.character = start_char,
		.line = start_line,
		.type = TOK_HEXNUM,
		.lexeme = lexeme
	};
}
