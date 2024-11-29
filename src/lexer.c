#include <error.h>
#include <lexer.h>
#include <string.h>
#include <common.h>
#include <memory.h>

#include <ctype.h>
#include <stdarg.h>

// additional functions that C stdlib doesn't have
static bool is_octal(char c) {
	return (c >= '0' && c <= '7');
}

// the main lexing function
void lexer_lex(Lexer *lexer) {
	while (!lexer_at_end(lexer)) {
		push_tokenlist(&lexer->list, lexer_querytoken(lexer));
		printf("[Type: %d, Line: %d, Character: %d, Lexeme: '%s']\n",
				lexer->list.items[lexer->list.size - 1].type,
				lexer->list.items[lexer->list.size - 1].line,
				lexer->list.items[lexer->list.size - 1].character,
				lexer->list.items[lexer->list.size - 1].lexeme == NULL ? ""
				: lexer->list.items[lexer->list.size - 1].lexeme);
		lexer_advance(lexer);
	}
}

void lexer_init(Lexer *lexer, char *code) {
	lexer->iter = 0;
	lexer->current_char = 0;
	lexer->current_line = 1;
	lexer->code = code;
	zeroinit_tokenlist(&lexer->list);
}

void lexer_free(Lexer *lexer) {
	free_tokenlist(&lexer->list);
	lexer_init(lexer, NULL);
}

bool lexer_at_end(Lexer *lexer) {
	return lexer_peek(lexer) == '\0' || lexer->iter >= strlen(lexer->code);
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

	printf("advanci\n");

	if (lexer_peek(lexer) == '\n') {
		lexer->current_char = 0;
		lexer->iter++;
		lexer->current_line++;
		printf("advance next lin\n");
		return;
	}

	lexer->current_char++;
	lexer->iter++;
	printf("finish advanci\n");
}

void lexer_goback(Lexer *lexer) {
	lexer->current_char--;
	lexer->iter--;

	if (lexer_peek(lexer) == '\n') {
		lexer->current_line--;
	}
}

static Token maketoken(TokenType type, u32 character, u32 line, char *lexeme) {
	return (Token) {
		.character = character,
			.line = line,
			.type = type,
			.lexeme = lexeme
	};
}

static Token match_keywords(
	u32 line, u32 character,
	char *lexeme, ...
) {
	va_list ptr;

	va_start(ptr, lexeme);

	char *current;

	while ((current = va_arg(ptr, char *)) != NULL) {
		printf("comparing %s against %s\n", lexeme, current);
		if (strlen(lexeme) == strlen(current) && memcmp(lexeme, current, strlen(lexeme) - 1) != 0) continue;
		printf("finished compar\n");

		TokenType type = va_arg(ptr, TokenType);
		printf("arged\n");

		va_end(ptr);
		printf("endd\n");

		printf("cumplete\n");
		// sorry for this, this function is adapted for partial trie usage
		return maketoken(type, character, line, lexeme - 1);
	}

	va_end(ptr);

	printf("cumplete identifier\n");
	return maketoken(TOK_IDENTIFIER, character, line, lexeme - 1);
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
		return match_keywords(start_line, start_char, &lexeme[1],
			"un", TOK_FUN,
			"or", TOK_FOR,
			"alse", TOK_FALSE,
			"loat", TOK_FLOAT,
			"oreach", TOK_FOREACH, NULL);
	case 'r':
		return match_keywords(start_line, start_char, &lexeme[1],
			"eturn", TOK_RETURN, NULL);
	case 's':
		return match_keywords(start_line, start_char, &lexeme[1],
			"truct", TOK_STRUCT, NULL);
	case 'l':
		return match_keywords(start_line, start_char, &lexeme[1],
			"et", TOK_LET, NULL);
	case 'w':
		return match_keywords(start_line, start_char, &lexeme[1],
			"hile", TOK_WHILE, NULL);
	case 'd':
		return match_keywords(start_line, start_char, &lexeme[1],
			"ouble", TOK_DOUBLE,
			"ata", TOK_DATA, NULL);
	case 'm':
		return match_keywords(start_line, start_char, &lexeme[1],
			"atch", TOK_MATCH, NULL);
	case 'i':
		return match_keywords(start_line, start_char, &lexeme[1],
			"f", TOK_IF,
			"nterface", TOK_INTERFACE, NULL);
	case 'e':
		return match_keywords(start_line, start_char, &lexeme[1],
			"lse", TOK_ELSE,
			"num", TOK_ENUM, NULL);
	case 'b':
		return match_keywords(start_line, start_char, &lexeme[1],
			"reak", TOK_BREAK, NULL);
	case 'c':
		return match_keywords(start_line, start_char, &lexeme[1],
			"ontinue", TOK_CONTINUE,
			"onst", TOK_CONST, NULL);
	case 't':
		return match_keywords(start_line, start_char, &lexeme[1],
			"rue", TOK_TRUE,
			"ypeof", TOK_TYPEOF, NULL);
	case 'n':
		return match_keywords(start_line, start_char, &lexeme[1],
			"ull", TOK_NULL, NULL);
	}

	return maketoken(TOK_IDENTIFIER, start_char, start_line, lexeme);
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

	return maketoken(TOK_STR, start_char, start_line, lexeme);
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

		return maketoken(TOK_INTNUM, start_char, start_line, lexeme);
	}

	lexer_advance(lexer);

	while (!lexer_at_end(lexer) && isdigit(lexer_peek(lexer))) {
		lexer_advance(lexer);
	}

	u32 count = (u32) (&lexer->code[lexer->iter] - beginning);

	char *lexeme = ALLOC(char, (count + 1));

	memcpy(lexeme, beginning, count);
	lexeme[count] = '\0';

	return maketoken(TOK_FLOATNUM, start_char, start_line, lexeme);
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

	return maketoken(TOK_OCTNUM, start_char, start_line, lexeme);
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

	return maketoken(TOK_BINNUM, start_char, start_line, lexeme);
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

	return maketoken(TOK_HEXNUM, start_char, start_line, lexeme);
}

Token lexer_querytoken(Lexer *lexer) {
	switch (lexer_peek(lexer)) {
	case '.':
		return maketoken(TOK_DOT,
			lexer->current_char, lexer->current_line,
			NULL);
	case ',':
		return maketoken(TOK_COMMA,
			lexer->current_char, lexer->current_line,
			NULL);
	case ';':
		return maketoken(TOK_SEMICOLON,
			lexer->current_char, lexer->current_line,
			NULL);
	case ':':
		return maketoken(TOK_COLON,
			lexer->current_char, lexer->current_line,
			NULL);
	case '(':
		return maketoken(TOK_ROUND_OPEN,
			lexer->current_char, lexer->current_line,
			NULL);
	case ')':
		return maketoken(TOK_ROUND_CLOSE,
			lexer->current_char, lexer->current_line,
			NULL);
	case '[':
		return maketoken(TOK_SQUARE_OPEN,
			lexer->current_char, lexer->current_line,
			NULL);
	case ']':
		return maketoken(TOK_SQUARE_CLOSE,
			lexer->current_char, lexer->current_line,
			NULL);
	case '{':
		return maketoken(TOK_BRACE_OPEN,
			lexer->current_char, lexer->current_line,
			NULL);
	case '}':
		return maketoken(TOK_BRACE_CLOSE,
			lexer->current_char, lexer->current_line,
			NULL);
	case '+':
		return maketoken(
			(lexer_match(lexer, '+') ?
				TOK_INCREMENT :
				lexer_match(lexer, '=') ?
				TOK_PLUS_EQUAL :
				TOK_PLUS),
			lexer->current_char, lexer->current_line,
			NULL);
	case '-':
		return maketoken(
			(lexer_match(lexer, '>') ?
				TOK_ARROW :
				lexer_match(lexer, '-') ?
				TOK_DECREMENT :
				lexer_match(lexer, '=') ?
				TOK_MINUS_EQUAL :
				TOK_MINUS),
			lexer->current_char, lexer->current_line,
			NULL);
	case '*':
		return maketoken(
			(lexer_match(lexer, '*') ?
				TOK_POWER :
				lexer_match(lexer, '=') ?
				TOK_STAR_EQUAL :
				TOK_STAR),
			lexer->current_char, lexer->current_line,
			NULL);
	case '/':
		if (lexer_match(lexer, '/')) {
			while (!lexer_at_end(lexer) && lexer_peek(lexer) != '\n') {
				lexer_advance(lexer);
			}
		}
		return maketoken(
			(lexer_match(lexer, '=') ?
				TOK_SLASH_EQUAL :
				TOK_SLASH),
			lexer->current_char, lexer->current_line,
			NULL);
	case '%':
		return maketoken(
			(lexer_match(lexer, '=') ?
				TOK_MOD_EQUAL :
				TOK_MOD),
			lexer->current_char, lexer->current_line,
			NULL);
	case '<':
		return maketoken(
			(lexer_match(lexer, '<') ?
				TOK_LEFT_SHIFT :
				lexer_match(lexer, '=') ?
				TOK_LESS_EQUAL :
				TOK_LESS),
			lexer->current_char, lexer->current_line,
			NULL);
	case '>':
		return maketoken(
			(lexer_match(lexer, '>') ?
				TOK_RIGHT_SHIFT :
				lexer_match(lexer, '=') ?
				TOK_GREATER_EQUAL :
				TOK_GREATER),
			lexer->current_char, lexer->current_line,
			NULL);
	case '=':
		return maketoken(
			(lexer_match(lexer, '=') ?
				TOK_CMP_EQUAL :
				TOK_EQUAL),
			lexer->current_char, lexer->current_line,
			NULL);
	case '&':
		return maketoken(
			(lexer_match(lexer, '&') ?
				TOK_LOGICAL_AND :
				lexer_match(lexer, '=') ?
				TOK_BIT_AND_EQUAL :
				TOK_BITWISE_AND),
			lexer->current_char, lexer->current_line,
			NULL);
	case '|':
		return maketoken(
			(lexer_match(lexer, '|') ?
				TOK_LOGICAL_OR :
				lexer_match(lexer, '=') ?
				TOK_BIT_OR_EQUAL :
				TOK_BITWISE_OR),
			lexer->current_char, lexer->current_line,
			NULL);
	case '^':
		return maketoken(
			(lexer_match(lexer, '=') ?
				TOK_BIT_XOR_EQUAL :
				TOK_BITWISE_XOR),
			lexer->current_char, lexer->current_line,
			NULL);
	case '~':
		return maketoken(
			(lexer_match(lexer, '=') ?
				TOK_BIT_NOT_EQUAL :
				TOK_BITWISE_NOT),
			lexer->current_char, lexer->current_line,
			NULL);
	case '!':
		return maketoken(
			(lexer_match(lexer, '=') ?
				TOK_LOG_NOT_EQUAL :
				TOK_LOGICAL_NOT),
			lexer->current_char, lexer->current_line,
			NULL);
	case '0':
		switch (lexer_peek_next(lexer)) {
		case '.':
			return lexer_lexnum(lexer);
		case 'x':
		case 'X':
			return lexer_lexhex(lexer);
		case 'b':
			return lexer_lexhex(lexer);
		default:
			return lexer_lexoct(lexer);
		}
	case '"':
		return lexer_lexstr(lexer);
	case ' ':
	case '\t':
		break;
	default:
		if (isdigit(lexer_peek(lexer))) {
			return lexer_lexnum(lexer);
		}
		else if (isalpha(lexer_peek(lexer))) {
			return lexer_lexid(lexer);
		}

		complainf("unrecognized character skill issue '%c'\n", lexer_peek(lexer));
	}

	return maketoken(TOK_NONE, 0, 0, NULL);
}
