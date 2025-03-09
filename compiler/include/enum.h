#pragma once

typedef enum ast_node_type
{
    AST_NODE_INTEGER,
    AST_NODE_BINOP,
    AST_NODE_UNARYOP,
    AST_NODE_COMPOUND,
    AST_NODE_ASSIGN,
    AST_NODE_VAR,
    AST_NODE_FUNCTION_DECLARE,
    AST_NODE_DECLARE,
    AST_NODE_RETURN,
    AST_NODE_FUNCCALL,
    AST_NODE_EMPTY,
} ast_node_type_t;

typedef enum tokentype_enum
{
    TOKEN_EOF,
    TOKEN_INTEGER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_ASSIGN,
    TOKEN_SEMI,
    TOKEN_LCBRCKT,
    TOKEN_RCBRCKT,
    TOKEN_ID,
    TOKEN_FUNC,
    TOKEN_TYPE,
    TOKEN_RETURN,
    TOKEN_COMMA,
} tokentype_t;

typedef enum vartypes {
    VAR_INTEGER,
    VAR_FUNCTION,
    VAR_UNKNOWN,
} vartype_t;
