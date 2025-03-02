#include <lexer.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <enum.h>

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

keyword_id_t* linear_keyword_search(keyword_id_t* keywords, size_t size, const char* key) {
    for (size_t i=0; i<size; i++) {
        if (strcmp(keywords[i].key, key) == 0) {
            return &keywords[i];
        }
    }
    return NULL;
}
void lex_id()
{
    keyword_id_t keywords[] = {
        {"func", TOKEN_FUNC},
        {"return", TOKEN_RETURN},
        {"int", TOKEN_TYPE},
    };
    size_t num_keywords = sizeof(keywords) / sizeof(keyword_id_t);
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
    keyword_id_t *keywordness = linear_keyword_search(keywords, num_keywords, identf);
    if (keywordness != NULL)
    {
        tokenss[count].type = keywordness->value;
    }
    else
    {
        tokenss[count].type = TOKEN_ID;
    }
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
        else if (buffer[pos] == ',')
        {
            lex_onechar(TOKEN_COMMA);
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
    case TOKEN_FUNC:
        return "FUNC";
    case TOKEN_TYPE:
        return "TYPE";
    case TOKEN_RETURN:
        return "RETURN";
    case TOKEN_COMMA:
        return "COMMA";
    default:
        return "UNKNOWN";
    }
}