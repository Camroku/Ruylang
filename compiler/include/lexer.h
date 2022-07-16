#pragma once

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
} tokentype_t;

typedef struct token_struct
{
    tokentype_t type;
    char* value;
    int line;
    int column;
} token_t;

typedef struct tokenlist_struct
{
    token_t* tokens;
    int count;
    int index;
} tokenlist_t;

tokenlist_t lex(char* buffer);
char *tokentype_to_string(tokentype_t type);