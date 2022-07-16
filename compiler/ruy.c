#include <stdio.h>
#include <lexer.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *file;
    char *buffer;
    int size;
    int i;

    if (argc != 2)
    {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }
    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file %s\n", argv[1]);
        return 1;
    }
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    buffer = (char *)malloc(size + 1);
    for (i = 0; i < size; i++)
    {
        buffer[i] = fgetc(file);
    }
    buffer[size] = '\0';
    fclose(file);
    tokenlist_t tokenlist = lex(buffer);
    for (i = 0; i < tokenlist.count; i++)
    {
        printf("%s\t%s\t%d:%d\n", tokentype_to_string(tokenlist.tokens[i].type), tokenlist.tokens[i].value, tokenlist.tokens[i].line, tokenlist.tokens[i].column);
    }
    return 0;
}