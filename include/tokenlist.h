#pragma once

#include <common.h>
#include <lexer.h>

typedef struct {
	size_t size;
	size_t cap;
	Token *items;
} TokenList;

void zeroinit_tokenlist(TokenList *list);
void push_tokenlist(TokenList *list, Token item);
void free_tokenlist(TokenList *list);
