#pragma once
#include <enum.h>
#include <lexer.h>
#include <stdbool.h>
#include <variables.h>
#include <types.h>

ast_node_t *parse(tokenlist_t tokenlist);
char *ast_node_to_string(ast_node_t *type, int depth);
