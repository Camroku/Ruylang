#include <interpreter.h>
#include <stdio.h>
#include <stdlib.h>
#include <lexer.h>

tokenlist_t tokens;

int term()
{
    int result = 0;
    if (tokens.index < tokens.count)
    {
        if (tokens.tokens[tokens.index].type == TOKEN_INTEGER)
        {
            result = atoi(tokens.tokens[tokens.index].value);
            tokens.index++;
        }
        else
        {
            printf("Error: expected integer, got %s\n", tokentype_to_string(tokens.tokens[tokens.index].type));
            exit(1);
        }
    }
    return result;
}

int expr()
{
    int result = term();
    while (tokens.tokens[tokens.index].type == TOKEN_PLUS || tokens.tokens[tokens.index].type == TOKEN_MINUS)
    {
        if (tokens.tokens[tokens.index].type == TOKEN_PLUS)
        {
            tokens.index++;
            result += term();
        }
        else if (tokens.tokens[tokens.index].type == TOKEN_MINUS)
        {
            tokens.index++;
            result -= term();
        }
        else
        {
            printf("Error: Expected + or -\n");
            return 0;
        }
    }
    return result;
}

void interpret(tokenlist_t tokenlist)
{
    tokens = tokenlist;
    tokens.index = 0;
    printf("%d\n", expr());
}