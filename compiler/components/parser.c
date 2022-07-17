#include <parser.h>
#include <stdlib.h>
#include <lexer.h>
#include <stdio.h>
#include <string.h>

tokenlist_t tokens;
token_t current_token;

void eat(tokentype_t type);
ast_node_t *factor();
ast_node_t *term();
ast_node_t *expr();
ast_node_t *compound_statement();
ast_node_t *assignment_statement();
ast_node_t *empty();
ast_node_t *statement();
ast_list_t *statement_list();
ast_node_t *program();

void raise_err()
{
    printf("Unexpected error at %d:%d\n", current_token.line, current_token.column);
    exit(1);
}

void eat(tokentype_t type)
{
    if (current_token.type == type)
    {
        tokens.index++;
        current_token = tokens.tokens[tokens.index];
    }
    else
    {
        printf("Error: Expected %s\n", tokentype_to_string(type));
        exit(1);
    }
}

ast_node_t *factor()
{
    if (current_token.type == TOKEN_INTEGER)
    {
        ast_node_t *node = malloc(sizeof(ast_node_t));
        node->type = AST_NODE_INTEGER;
        node->integer.value = atoi(current_token.value);
        eat(TOKEN_INTEGER);
        return node;
    }
    else if (current_token.type == TOKEN_LPAREN)
    {
        eat(TOKEN_LPAREN);
        ast_node_t *node = expr();
        eat(TOKEN_RPAREN);
        return node;
    }
    else if (current_token.type == TOKEN_PLUS)
    {
        eat(TOKEN_PLUS);
        ast_node_t *node = malloc(sizeof(ast_node_t));
        node->type = AST_NODE_UNARYOP;
        node->unaryop.op = '+';
        node->unaryop.right = factor();
        return node;
    }
    else if (current_token.type == TOKEN_MINUS)
    {
        eat(TOKEN_MINUS);
        ast_node_t *node = malloc(sizeof(ast_node_t));
        node->type = AST_NODE_UNARYOP;
        node->unaryop.op = '-';
        node->unaryop.right = factor();
        return node;
    }
    else if (current_token.type == TOKEN_ID)
    {
        ast_node_t *node = malloc(sizeof(ast_node_t));
        node->type = AST_NODE_VAR;
        node->var.name = current_token.value;
        eat(TOKEN_ID);
        return node;
    }
    else
    {
        raise_err();
        exit(1); // raise_err() already does this, this is just to suspend compiler warnings
    }
}

ast_node_t *term()
{
    ast_node_t *node = factor();
    while (current_token.type == TOKEN_MUL || current_token.type == TOKEN_DIV)
    {
        ast_node_t *tmp = malloc(sizeof(ast_node_t));
        tmp->type = AST_NODE_BINOP;
        tmp->binop.op = current_token.value[0];
        tmp->binop.left = node;
        eat(current_token.type);
        tmp->binop.right = factor();
        node = tmp;
    }
    return node;
}

ast_node_t *expr()
{
    ast_node_t *result = term();
    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS)
    {
        if (current_token.type == TOKEN_PLUS)
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
        else if (current_token.type == TOKEN_MINUS)
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

ast_node_t *compound_statement()
{
    eat(TOKEN_LCBRCKT);
    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->type = AST_NODE_COMPOUND;
    ast_list_t *statements = statement_list();
    node->compound.children = statements->statements;
    node->compound.count = statements->count;
    eat(TOKEN_RCBRCKT);
    return node;
}

ast_node_t *assignment_statement()
{
    char *id = current_token.value;
    eat(TOKEN_ID);
    eat(TOKEN_ASSIGN);
    ast_node_t *value = expr();
    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->type = AST_NODE_ASSIGN;
    node->assign.name = id;
    node->assign.value = value;
    return node;
}

ast_node_t *empty()
{
    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->type = AST_NODE_EMPTY;
    return node;
}

ast_node_t *statement()
{
    switch (current_token.type)
    {
    case TOKEN_LCBRCKT:
        return compound_statement();
        break;
    case TOKEN_ID:
        return assignment_statement();
        break;
    default:
        return empty();
        break;
    }
}

ast_list_t *statement_list()
{
    int stcnt = 1;
    ast_node_t **statements = malloc(sizeof(ast_node_t) * stcnt);
    statements[0] = statement();
    while (current_token.type == TOKEN_SEMI)
    {
        eat(TOKEN_SEMI);
        statements = realloc(statements, sizeof(ast_node_t) * stcnt + 1);
        statements[stcnt] = statement();
        stcnt++;
    }
    if (statements[stcnt - 1]->type != AST_NODE_EMPTY)
        raise_err();
    ast_list_t *stlist = malloc(sizeof(ast_list_t));
    stlist->statements = statements;
    stlist->count = stcnt;
    return stlist;
}

ast_node_t *program()
{
    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->type = AST_NODE_COMPOUND;
    ast_list_t *statements = statement_list();
    node->compound.children = statements->statements;
    node->compound.count = statements->count;
    return node;
}

ast_node_t *parse(tokenlist_t tokenlist)
{
    tokens = tokenlist;
    current_token = tokens.tokens[0];
    return program();
}

char *ast_node_to_string(ast_node_t *node, int depth)
{
    char *out = malloc(sizeof(char) * 1024 * 1024);
    memset(out, 0, 1024 * 1024);
    char *indent = malloc(sizeof(char) * (depth * 2 + 1));
    indent[depth * 2] = '\0';
    memset(indent, ' ', depth * 2);
    switch (node->type)
    {
    case AST_NODE_INTEGER:
        sprintf(out, "%sInteger(%d)", indent, node->integer.value);
        break;
    case AST_NODE_BINOP:
        sprintf(out, "%sBinOp(\n%s, %c,\n%s\n%s)", indent, ast_node_to_string(node->binop.left, depth + 1), node->binop.op, ast_node_to_string(node->binop.right, depth + 1), indent);
        break;
    case AST_NODE_UNARYOP:
        sprintf(out, "%sUnaryOp( %c,\n%s\n%s)", indent, node->unaryop.op, ast_node_to_string(node->unaryop.right, depth + 1), indent);
        break;
    case AST_NODE_COMPOUND:
        sprintf(out, "%sCompound(\n", indent);
        for (int i = 0; i < node->compound.count; i++)
        {
            sprintf(out + strlen(out), "%s\n", ast_node_to_string(node->compound.children[i], depth+1));
        }
        sprintf(out + strlen(out), "%s)", indent);
        break;
    case AST_NODE_ASSIGN:
        sprintf(out, "%sAssign(\n  %s%s\n%s\n%s)", indent, indent, node->assign.name, ast_node_to_string(node->assign.value, depth+1), indent);
        break;
    case AST_NODE_EMPTY:
        sprintf(out, "%sEmpty()", indent);
        break;
    case AST_NODE_VAR:
        sprintf(out, "%sVar(%s)", indent, node->var.name);
        break;
    }
    out = realloc(out, strlen(out) + 1);
    return out;
}