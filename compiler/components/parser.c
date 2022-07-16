#include <parser.h>
#include <stdlib.h>
#include <lexer.h>
#include <stdio.h>

tokenlist_t tokens;

void eat(tokentype_t type);
ast_node_t *factor();
ast_node_t *term();
ast_node_t *expr();

void eat(tokentype_t type)
{
    if (tokens.tokens[tokens.index].type == type)
    {
        tokens.index++;
    }
    else
    {
        printf("Error: Expected %s\n", tokentype_to_string(type));
        exit(1);
    }
}

ast_node_t *factor()
{
    if (tokens.tokens[tokens.index].type == TOKEN_INTEGER)
    {
        ast_node_t *node = malloc(sizeof(ast_node_t));
        node->type = AST_NODE_INTEGER;
        node->integer.value = atoi(tokens.tokens[tokens.index].value);
        eat(TOKEN_INTEGER);
        return node;
    }
    else if (tokens.tokens[tokens.index].type == TOKEN_LPAREN)
    {
        eat(TOKEN_LPAREN);
        ast_node_t *node = expr();
        eat(TOKEN_RPAREN);
        return node;
    }
    else
    {
        printf("Error: Expected integer or left paren\n");
        exit(1);
    }
}

ast_node_t *term()
{
    ast_node_t *node = factor();
    while (tokens.tokens[tokens.index].type == TOKEN_MUL || tokens.tokens[tokens.index].type == TOKEN_DIV)
    {
        ast_node_t *tmp = malloc(sizeof(ast_node_t));
        tmp->type = AST_NODE_BINOP;
        tmp->binop.op = tokens.tokens[tokens.index].value[0];
        tmp->binop.left = node;
        eat(tokens.tokens[tokens.index].type);
        tmp->binop.right = factor();
        node = tmp;
    }
    return node;
}

ast_node_t *expr()
{
    ast_node_t *result = term();
    while (tokens.tokens[tokens.index].type == TOKEN_PLUS || tokens.tokens[tokens.index].type == TOKEN_MINUS)
    {
        if (tokens.tokens[tokens.index].type == TOKEN_PLUS)
        {
            eat(TOKEN_PLUS);
            ast_node_t *right = term();
            ast_node_t *left = malloc(sizeof(ast_node_t));
            *left = *result;
            result->type = AST_NODE_BINOP;
            result->binop.left = left;
            result->binop.right = right;
            result->binop.op = '+';
        }
        else if (tokens.tokens[tokens.index].type == TOKEN_MINUS)
        {
            eat(TOKEN_MINUS);
            ast_node_t *right = term();
            ast_node_t *left = malloc(sizeof(ast_node_t));
            *left = *result;
            result->type = AST_NODE_BINOP;
            result->binop.left = left;
            result->binop.right = right;
            result->binop.op = '-';
        }
    }
    return result;
}

ast_node_t *parse(tokenlist_t tokenlist)
{
    tokens = tokenlist;
    return expr();
}

char *ast_node_to_string(ast_node_t *type, int depth)
{
    char *out = malloc(sizeof(char) * 1024);
    char *indent = malloc(sizeof(char) * (depth * 2 + 1));
    indent[depth * 2] = '\0';
    for (int i = 0; i < depth * 2; i++)
    {
        indent[i] = ' ';
    }
    switch (type->type)
    {
    case AST_NODE_INTEGER:
        sprintf(out, "%sInteger(%d)", indent, type->integer.value);
        break;
    case AST_NODE_BINOP:
        sprintf(out, "%sBinOp(\n%s, %c,\n%s\n%s)", indent, ast_node_to_string(type->binop.left, depth + 1), type->binop.op, ast_node_to_string(type->binop.right, depth + 1), indent);
        break;
    }
    return out;
}