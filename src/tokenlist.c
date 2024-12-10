#include <lexer.h>
#include <memory.h>

void zeroinit_tokenlist(TokenList *list) {
	list->size = 0;
	list->cap = 0;
	list->items = NULL;
}

void push_tokenlist(TokenList *list, Token item) {
	if (list->size >= list->cap) {
		int old = list->cap;
		list->cap = (list->cap <= 0) ? 8 : list->cap * 2;
		list->items = GROW_ARR(Token, list->items, old, list->cap);
	}

	list->items[list->size++] = item;
}

void free_tokenlist(TokenList *list) {
	FREE_ARR(Token, list->items, list->cap);
	zeroinit_tokenlist(list);
}
