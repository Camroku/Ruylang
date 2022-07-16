#include <interpreter.h>
#include <stdio.h>
#include <stdlib.h>
#include <lexer.h>
#include <parser.h>

int visit_node(ast_node_t *node)
{
    switch (node->type)
    {
    case AST_NODE_INTEGER:
        return node->integer.value;
        break;
    case AST_NODE_BINOP:
        int left = visit_node(node->binop.left);
        int right = visit_node(node->binop.right);
        switch (node->binop.op)
        {
        case '+':
            return left + right;
            break;
        case '-':
            return left - right;
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

