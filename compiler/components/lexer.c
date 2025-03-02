#include <lexer.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

token_t *tokenss = NULL;
int count = 0;
int pos = 0;
int line = 1;
int column = 0;
char *buffer = NULL;

void init_lexer(char *buff)
{
    buffer = buff;
    line = 1;
    column = 0;
    pos = 0;
    count = 0;
}

void lex_num()
{
    int j = 0;
    char *number = malloc(sizeof(char) * 20);
    while (isdigit(buffer[pos]))
    {
        number[j] = buffer[pos];
        j++;
        pos++;
    }
    number[j] = '\0';
    pos--;
    tokenss = realloc(tokenss, sizeof(token_t) * (count + 1));
    tokenss[count].type = TOKEN_INTEGER;
    tokenss[count].value = number;
    tokenss[count].line = line;
    tokenss[count].column = column - 1;
    count++;
}

void lex_onechar(tokentype_t type)
{
    tokenss = realloc(tokenss, sizeof(token_t) * (count + 1));
    char *charp = malloc(sizeof(char) * 2);
    charp[0] = buffer[pos];
    charp[1] = '\0';
    tokenss[count].type = type;
    tokenss[count].value = charp;
    tokenss[count].line = line;
    tokenss[count].column = column;
    count++;
}

void lex_id()
{
    int j = 0;
    char *identf = malloc(sizeof(char) * 20);
    while (isalnum(buffer[pos]) || buffer[pos] == '_' || buffer[pos] == '.')
    {
        identf[j] = buffer[pos];
        j++;
        pos++;
    }
    identf[j] = '\0';
    pos--;
    tokenss = realloc(tokenss, sizeof(token_t) * (count + 1));
    tokenss[count].type = TOKEN_ID;
    tokenss[count].value = identf;
    tokenss[count].line = line;
    tokenss[count].column = column - 1;
    count++;
}

tokenlist_t lex()
{
    tokenlist_t tokenlist;
    while (buffer[pos] != '\0')
    {
        if (buffer[pos] == '\n')
        {
            line++;
            column = 0;
        }
        else
        {
            column++;
        }
        if (buffer[pos] == '#' && buffer[pos+1] == '#')
        {
            pos += 2;
            while (!(buffer[pos] == '#' && buffer[pos+1] == '#'))
            {
                pos++;
            }
            pos += 2;
            continue;
        }
        else if (buffer[pos] == '#')
        {
            while (buffer[pos] != '\n')
            {
                pos++;
            }
            pos++;
            continue;
        }
        else if (buffer[pos] == '+')
        {
            lex_onechar(TOKEN_PLUS);
        }
        else if (buffer[pos] == '-')
        {
            lex_onechar(TOKEN_MINUS);
        }
        else if (buffer[pos] == '*')
        {
            lex_onechar(TOKEN_MUL);
        }
        else if (buffer[pos] == '/')
        {
            lex_onechar(TOKEN_DIV);
        }
        else if (buffer[pos] == '(')
        {
            lex_onechar(TOKEN_LPAREN);
        }
        else if (buffer[pos] == ')')
        {
            lex_onechar(TOKEN_RPAREN);
        }
        else if (isdigit(buffer[pos]))
        {
            lex_num();
        }
        else if (buffer[pos] == '=')
        {
            lex_onechar(TOKEN_ASSIGN);
        }
        else if (buffer[pos] == ';')
        {
            lex_onechar(TOKEN_SEMI);
        }
        else if (buffer[pos] == '{')
        {
            lex_onechar(TOKEN_LCBRCKT);
        }
        else if (buffer[pos] == '}')
        {
            lex_onechar(TOKEN_RCBRCKT);
        }
        else if (isalpha(buffer[pos]) || buffer[pos] == '_')
        {
            lex_id();
        }
        else if (isspace(buffer[pos]))
        {
            pos++;
            continue;
        }
        else
        {
            printf("Unknown character %c at line %d, column %d\n", buffer[pos], line, column - 1);
            exit(1);
        }
        pos++;
    }
    tokenss = realloc(tokenss, sizeof(token_t) * (count + 1));
    tokenss[count].type = TOKEN_EOF;
    tokenss[count].value = NULL;
    tokenss[count].line = line;
    tokenss[count].column = column - 1;
    count++;
    tokenlist.tokens = tokenss;
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
    case TOKEN_MUL:
        return "MUL";
    case TOKEN_DIV:
        return "DIV";
    case TOKEN_LPAREN:
        return "LPAREN";
    case TOKEN_RPAREN:
        return "RPAREN";
    case TOKEN_ASSIGN:
        return "ASSIGN";
    case TOKEN_SEMI:
        return "SEMI";
    case TOKEN_LCBRCKT:
        return "LCBRCKT";
    case TOKEN_RCBRCKT:
        return "RCBRCKT";
    case TOKEN_ID:
        return "ID";
    default:
        return "UNKNOWN";
    }
}