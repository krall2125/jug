#include <lexer.h>
#include <tokenlist.h>

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

static Token lexer_lexstr(Lexer *lexer) {
	lexer_advance(lexer);
	char *beginning = &lexer->code[lexer->iter];

	// the amount of bytes to allocate for the string lexeme
	u32 count = 0;
	do {
		lexer_advance(lexer);
		if (lexer->code[lexer->iter] == '\\') {
			continue;
		}
		count++;
	} while (!lexer_match(lexer, '\"') && lexer_peek(lexer) != '\\');

	char *lexeme = malloc(sizeof(char) * (count + 1));
}

Token lexer_querytoken(Lexer *lexer) {
}

void lexer_lex(Lexer *lexer) {
	for (u32 i = 0; i < strlen(lexer->code); i++) {
		push_tokenlist(&lexer->list, lexer_querytoken(lexer));
	}
}
