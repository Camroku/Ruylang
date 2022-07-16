#include <lexer.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

tokenlist_t lex(char* buffer)
{
    tokenlist_t tokenlist;
    token_t* tokens = NULL;
    int count = 0;
    int i = 0;
    int line = 1;
    int column = 0;
    for (i = 0; buffer[i] != '\0'; i++)
    {
        if (buffer[i] == '\n')
        {
            line++;
            column = 0;
        }
        else
        {
            column++;
        }
        if (buffer[i] == '+')
        {
            tokens = realloc(tokens, sizeof(token_t) * (count + 1));
            tokens[count].type = TOKEN_PLUS;
            tokens[count].value = "+";
            tokens[count].line = line;
            tokens[count].column = column - 1;
            count++;
        }
        else if (buffer[i] == '-')
        {
            tokens = realloc(tokens, sizeof(token_t) * (count + 1));
            tokens[count].type = TOKEN_MINUS;
            tokens[count].value = "-";
            tokens[count].line = line;
            tokens[count].column = column - 1;
            count++;
        }
        else if (buffer[i] >= '0' && buffer[i] <= '9')
        {
            int j = 0;
            char* number = malloc(sizeof(char) * 10);
            for (j = 0; buffer[i] >= '0' && buffer[i] <= '9'; i++)
            {
                number[j] = buffer[i];
                j++;
            }
            number[j] = '\0';
            i--;
            tokens = realloc(tokens, sizeof(token_t) * (count + 1));
            tokens[count].type = TOKEN_INTEGER;
            tokens[count].value = number;
            tokens[count].line = line;
            tokens[count].column = column - 1;
            count++;
        }
        else if (isspace(buffer[i]))
        {
            continue;
        }
        else
        {
            printf("Unknown character %c at line %d, column %d\n", buffer[i], line, column - 1);
        }
    }
    tokens = realloc(tokens, sizeof(token_t) * (count + 1));
    tokens[count].type = TOKEN_EOF;
    tokens[count].value = NULL;
    tokens[count].line = line;
    tokens[count].column = column - 1;
    count++;
    tokenlist.tokens = tokens;
    tokenlist.count = count;
    return tokenlist;
}

char *tokentype_to_string(tokentype_t type)
{
    switch (type)
    {
    case TOKEN_EOF:
        return "EOF";
    case TOKEN_INTEGER:
        return "NUMBER";
    case TOKEN_PLUS:
        return "PLUS";
    case TOKEN_MINUS:
        return "MINUS";
    }
    return "UNKNOWN";
}