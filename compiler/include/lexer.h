#pragma once

typedef enum tokentype_enum
{
    TOKEN_EOF,
    TOKEN_INTEGER,
    TOKEN_PLUS,
    TOKEN_MINUS,
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
} tokenlist_t;

tokenlist_t lex(char* buffer);
char *tokentype_to_string(tokentype_t type);