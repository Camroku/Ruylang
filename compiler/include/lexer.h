#pragma once
#include <enum.h>
#include <types.h>

void init_lexer(char* buffer);
tokenlist_t lex();
char *tokentype_to_string(tokentype_t type);