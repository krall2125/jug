#include <error.h>
#include <lexer.h>
#include <tokenlist.h>
#include <common.h>
#include <memory.h>

#include <ctype.h>

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
	return lexer_peek(lexer) == c;
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

static Token lexer_lexstr(Lexer *lexer) {
	u32 start_char = lexer->current_char;
	u32 start_line = lexer->current_line;
	lexer_advance(lexer);
	char *beginning = &lexer->code[lexer->iter];

	// the amount of bytes to allocate for the string lexeme
	while (!lexer_at_end(lexer) && !lexer_match(lexer, '\"') && lexer->code[lexer->iter - 1] != '\\') {
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

static bool is_octal(char c) {
	return (c >= '0' && c <= '7');
}

// this will lex both integers and floats
static Token lexer_lexnum(Lexer *lexer) {
	u32 start_char = lexer->current_char;
	u32 start_line = lexer->current_line;

	char *beginning = &lexer->code[lexer->iter];

	while (isdigit(lexer_peek(lexer))) {
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

	while (isdigit(lexer_peek(lexer))) {
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

	char *lexeme = ALLOC(char, (count + 1));

	// copy the necessary bytes into the lexeme
	memcpy(lexeme, beginning, count);
	lexeme[count] = '\0';

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

	char *lexeme = ALLOC(char, (count + 1));

	memcpy(lexeme, beginning, count);
	lexeme[count] = '\0';

	return (Token) {
		.character = start_char,
		.line = start_line,
		.type = TOK_BINNUM,
		.lexeme = lexeme
	};
}

Token lexer_querytoken(Lexer *lexer) {
}

void lexer_lex(Lexer *lexer) {
	for (u32 i = 0; i < strlen(lexer->code); i++) {
		push_tokenlist(&lexer->list, lexer_querytoken(lexer));
	}
}
