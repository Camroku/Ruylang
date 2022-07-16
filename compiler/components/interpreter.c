#include <interpreter.h>
#include <stdio.h>
#include <stdlib.h>
#include <lexer.h>
#include <parser.h>

int visit_node(ast_node_t *node)
{
    int left = 0, right = 0;
    switch (node->type)
    {
    case AST_NODE_INTEGER:
        return node->integer.value;
        break;
    case AST_NODE_BINOP:
        left = visit_node(node->binop.left);
        right = visit_node(node->binop.right);
        switch (node->binop.op)
        {
        case '+':
            return left + right;
            break;
        case '-':
            return left - right;
            break;
        case '*':
            return left * right;
            break;
        case '/':
            return left / right;
            break;
        }
        break;
    case AST_NODE_UNARYOP:
        right = visit_node(node->unaryop.right);
        switch (node->unaryop.op)
        {
        case '+':
            return +right;
            break;
        case '-':
            return -right;
            break;
        }
        break;
    }
    return 0;
}

void interpret(ast_node_t *node)
{
    printf("%d\n", visit_node(node));
}

