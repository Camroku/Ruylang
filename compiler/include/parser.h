#pragma once
#include <lexer.h>

typedef enum ast_node_type
{
    AST_NODE_INTEGER,
    AST_NODE_BINOP,
    AST_NODE_UNARYOP,
} ast_node_type_t;

typedef struct integer_ast
{
    int value;
} integer_ast_t;

typedef struct binop_ast
{
    struct ast_node_t* left;
    struct ast_node_t* right;
    char op;
} binop_ast_t;

typedef struct unaryop_ast
{
    struct ast_node_t* right;
    char op;
} unaryop_ast_t;

typedef struct ast_node_t
{
    ast_node_type_t type;
    union
    {
        integer_ast_t integer;
        binop_ast_t binop;
        unaryop_ast_t unaryop;
    };
} ast_node_t;

ast_node_t *parse(tokenlist_t tokenlist);
char *ast_node_to_string(ast_node_t *type, int depth);
