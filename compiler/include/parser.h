#pragma once
#include <lexer.h>

typedef enum ast_node_type
{
    AST_NODE_INTEGER,
    AST_NODE_BINOP,
    AST_NODE_UNARYOP,
    AST_NODE_COMPOUND,
    AST_NODE_ASSIGN,
    AST_NODE_VAR,
    AST_NODE_EMPTY,
} ast_node_type_t;

typedef struct integer_ast
{
    int value;
} integer_ast_t;

typedef struct binop_ast
{
    struct ast_node *left;
    struct ast_node *right;
    char op;
} binop_ast_t;

typedef struct unaryop_ast
{
    struct ast_node *right;
    char op;
} unaryop_ast_t;

typedef struct compound_ast
{
    struct ast_node **children;
    int count;
} compound_ast_t;

typedef struct ast_list
{
    struct ast_node **statements;
    int count;
} ast_list_t;


typedef struct assign_ast
{
    char *name;
    struct ast_node *value;
} assign_ast_t;

typedef struct var_ast
{
    char *name;
} var_ast_t;

typedef struct empty_ast
{
} empty_ast_t;

typedef struct ast_node
{
    ast_node_type_t type;
    union
    {
        integer_ast_t integer;
        binop_ast_t binop;
        unaryop_ast_t unaryop;
        compound_ast_t compound;
        assign_ast_t assign;
        var_ast_t var;
        empty_ast_t empty;
    };
} ast_node_t;

ast_node_t *parse(tokenlist_t tokenlist);
char *ast_node_to_string(ast_node_t *type, int depth);
