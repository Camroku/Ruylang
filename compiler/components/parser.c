#include <parser.h>
#include <stdlib.h>
#include <lexer.h>
#include <stdio.h>
#include <string.h>
#include <variables.h>
#include <enum.h>

#pragma region General definitions
tokenlist_t tokens;
token_t current_token;

void eat(tokentype_t type);
ast_node_t *factor();
ast_node_t *term();
ast_node_t *expr();
ast_node_t *function_statement();
ast_node_t *return_statement();
ast_node_t *funccall();
ast_node_t *assignment_statement();
ast_node_t *declaration_statement();
ast_node_t *empty();
ast_node_t *statement();
ast_list_t *statement_list();
ast_node_t *compound_statement();
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

token_t peek(int inc)
{
    return tokens.tokens[tokens.index + inc];
}
#pragma endregion

#pragma region Expression parsing
ast_node_t *factor()
{
    if (peek(0).type == TOKEN_INTEGER)
    {
        ast_node_t *node = malloc(sizeof(ast_node_t));
        node->type = AST_NODE_INTEGER;
        node->integer.value = atoi(current_token.value);
        eat(TOKEN_INTEGER);
        return node;
    }
    else if (peek(0).type == TOKEN_LPAREN)
    {
        eat(TOKEN_LPAREN);
        ast_node_t *node = expr();
        eat(TOKEN_RPAREN);
        return node;
    }
    else if (peek(0).type == TOKEN_PLUS)
    {
        eat(TOKEN_PLUS);
        ast_node_t *node = malloc(sizeof(ast_node_t));
        node->type = AST_NODE_UNARYOP;
        node->unaryop.op = '+';
        node->unaryop.right = factor();
        return node;
    }
    else if (peek(0).type == TOKEN_MINUS)
    {
        eat(TOKEN_MINUS);
        ast_node_t *node = malloc(sizeof(ast_node_t));
        node->type = AST_NODE_UNARYOP;
        node->unaryop.op = '-';
        node->unaryop.right = factor();
        return node;
    }
    else if (peek(0).type == TOKEN_ID)
    {
        if (peek(1).type == TOKEN_LPAREN)
        {
            return funccall();
        }
        else
        {
            ast_node_t *node = malloc(sizeof(ast_node_t));
            node->type = AST_NODE_VAR;
            node->var.name = current_token.value;
            eat(TOKEN_ID);
            return node;
        }
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
    while (peek(0).type == TOKEN_MUL || peek(0).type == TOKEN_DIV)
    {
        ast_node_t *tmp = malloc(sizeof(ast_node_t));
        tmp->type = AST_NODE_BINOP;
        tmp->binop.op = current_token.value[0];
        tmp->binop.left = node;
        eat(peek(0).type);
        tmp->binop.right = factor();
        node = tmp;
    }
    return node;
}

ast_node_t *expr()
{
    ast_node_t *result = term();
    while (peek(0).type == TOKEN_PLUS || peek(0).type == TOKEN_MINUS)
    {
        if (peek(0).type == TOKEN_PLUS)
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
        else if (peek(0).type == TOKEN_MINUS)
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
#pragma endregion

#pragma region Functions
ast_node_t *function_statement()
{
    eat(TOKEN_FUNC);
    char *type = current_token.value;
    eat(TOKEN_TYPE);
    char *id = current_token.value;
    eat(TOKEN_ID);
    eat(TOKEN_LPAREN);
    args_t *arguments = malloc(sizeof(args_t));
    arguments->count = 0;
    bool eat_arg = false;
    if (peek(0).type == TOKEN_TYPE)
        eat_arg = true;
    while (eat_arg)
    {
        char *arg_type = current_token.value;
        eat(TOKEN_TYPE);
        char *arg_id = current_token.value;
        eat(TOKEN_ID);

        func_arg_t *argument = malloc(sizeof(func_arg_t));
        argument->type = type_str_to_enum(arg_type);
        argument->var = arg_id;

        arguments->args = realloc(arguments->args, sizeof(func_arg_t) * (arguments->count + 1));
        arguments->args[arguments->count] = argument;
        arguments->count++;
        
        if (peek(0).type != TOKEN_COMMA)
            eat_arg = false;
        else
            eat(TOKEN_COMMA);
    }
    eat(TOKEN_RPAREN);
    ast_node_t *compound = compound_statement();

    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->type = AST_NODE_FUNCTION_DECLARE;
    node->func.name = id;
    node->func.type = type;
    node->func.arguments = arguments;
    node->func.compound = compound;
    node->syntax_semicolon_pass = true;
    return node;
}

ast_node_t *return_statement()
{
    eat(TOKEN_RETURN);
    ast_node_t *value = expr();
    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->type = AST_NODE_RETURN;
    node->returnast.value = value;
    return node;
}

ast_node_t *funccall()
{
    char *id = current_token.value;
    eat(TOKEN_ID);
    eat(TOKEN_LPAREN);
    list_ast_t *args = malloc(sizeof(list_ast_t));
    args->count = 0;
    bool eat_arg = false;
    if (peek(0).type != TOKEN_RPAREN)
        eat_arg = true;
    while (eat_arg)
    {
        ast_node_t *value = expr();
        args->values = realloc(args->values, sizeof(ast_node_t) * (args->count + 1));
        args->values[args->count] = value;
        args->count++;
        
        if (peek(0).type != TOKEN_COMMA)
            eat_arg = false;
        else
            eat(TOKEN_COMMA);
    }
    eat(TOKEN_RPAREN);
    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->type = AST_NODE_FUNCCALL;
    node->funccall.name = id;
    node->funccall.arguments = args;
    return node;
}
#pragma endregion

#pragma region Assignment
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

ast_node_t *declaration_statement()
{
    char *type = current_token.value;
    eat(TOKEN_TYPE);
    char *id = current_token.value;
    eat(TOKEN_ID);
    eat(TOKEN_ASSIGN);
    ast_node_t *value = expr();
    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->type = AST_NODE_DECLARE;
    node->declare.name = id;
    node->declare.type = type;
    node->declare.value = value;
    return node;
}
#pragma endregion

#pragma region General syntax handling
ast_node_t *empty()
{
    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->type = AST_NODE_EMPTY;
    return node;
}

ast_node_t *statement()
{
    switch (peek(0).type)
    {
    case TOKEN_LCBRCKT:
        return compound_statement();
        break;
    case TOKEN_ID:
        return assignment_statement();
        break;
    case TOKEN_TYPE:
        return declaration_statement();
        break;
    case TOKEN_FUNC:
        return function_statement();
        break;
    case TOKEN_RETURN:
        return return_statement();
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
    while (peek(0).type == TOKEN_SEMI || statements[stcnt-1]->syntax_semicolon_pass)
    {
        if (!statements[stcnt-1]->syntax_semicolon_pass) eat(TOKEN_SEMI);
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

ast_node_t *compound_statement()
{
    eat(TOKEN_LCBRCKT);
    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->type = AST_NODE_COMPOUND;
    ast_list_t *statements = statement_list();
    node->compound.children = statements->statements;
    node->compound.count = statements->count;
    eat(TOKEN_RCBRCKT);
    node->syntax_semicolon_pass = true;
    return node;
}
#pragma endregion

#pragma region Main parser functions
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
    case AST_NODE_DECLARE:
        sprintf(out, "%sDeclare(\n  %s%s\n%s\n%s)", indent, indent, node->declare.name, ast_node_to_string(node->declare.value, depth+1), indent);
        break;
    case AST_NODE_FUNCTION_DECLARE:
        sprintf(out, "%sFuncDec(\n  %s%s\n  %sArguments:\n", indent, indent, node->func.name, indent);
        for (int i = 0; i < node->func.arguments->count; i++)
        {
            sprintf(out + strlen(out), "%s    %s %s\n", indent, type_enum_to_str(node->func.arguments->args[i]->type), node->func.arguments->args[i]->var);
        }
        sprintf(out + strlen(out), "%s\n%s)", ast_node_to_string(node->func.compound, depth+1), indent);
        break;
    case AST_NODE_RETURN:
        sprintf(out, "%sReturn(\n%s\n%s)", indent, ast_node_to_string(node->returnast.value, depth+1), indent);
        break;
    case AST_NODE_FUNCCALL:
        sprintf(out, "%sFuncCall(\n  %s%s\n  %sArguments:\n", indent, indent, node->func.name, indent);
        for (int i = 0; i < node->funccall.arguments->count; i++)
        {
            sprintf(out + strlen(out), "%s\n", ast_node_to_string(node->funccall.arguments->values[i], depth+2));
        }
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
#pragma endregion
