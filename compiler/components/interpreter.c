#include <interpreter.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <lexer.h>
#include <parser.h>
#include <variables.h>

int visit_node(vars_t *variables, ast_node_t *node);

int int_binop(vars_t *variables, ast_node_t *node)
{
    int left = 0, right = 0;
    left = visit_node(variables, node->binop.left);
    right = visit_node(variables, node->binop.right);
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
    default:
        return 0;
        break;
    }
}

void int_compound(vars_t *variables, ast_node_t *node, bool create_new_scope)
{
    if (create_new_scope)
    {
        vars_t *new_scope = create_scope(variables);
        for (int i = 0; i < node->compound.count; i++)
        {
            visit_node(new_scope, node->compound.children[i]);
        }
    }
    else
    {
        for (int i = 0; i < node->compound.count; i++)
        {
            visit_node(variables, node->compound.children[i]);
        }
    }
}

void int_assign(vars_t *variables, ast_node_t *node)
{
    varval_t *variable = malloc(sizeof(varval_t));
    variable->type = INTEGER;
    variable->var = node->assign.name;
    variable->integer.value = visit_node(variables, node->assign.value);
    set_var(variables, variable);
    printf("just set %s = %d\n", variable->var, variable->integer.value);
}

int visit_node(vars_t *variables, ast_node_t *node)
{
    int right = 0;
    switch (node->type)
    {
    case AST_NODE_INTEGER:
        return node->integer.value;
        break;
    case AST_NODE_BINOP:
        return int_binop(variables, node);
        break;
    case AST_NODE_UNARYOP:
        right = visit_node(variables, node->unaryop.right);
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
    case AST_NODE_COMPOUND:
        int_compound(variables, node, true);
        return 0;
        break;
    case AST_NODE_ASSIGN:
        int_assign(variables, node);
        return 0;
        break;
    case AST_NODE_VAR:
        varval_t *variable = get_var(variables, node->var.name);
        if (variable == 0)
        {
            printf("Variable '%s' not found.\n", node->var.name);
            exit(1);
        }
        else
        {
            switch (variable->type)
            {
            case INTEGER:
                return variable->integer.value;
                break;
            }
        }
        break;
    case AST_NODE_EMPTY:
        return 0;
        break;
    }
    return 0;
}

void interpret(ast_node_t *node)
{
    vars_t *variables = malloc(sizeof(vars_t));
    init_vars(variables);
    int_compound(variables, node, false);
    printf("cnt %d\nprinting the main scope...\n", variables->count);
    for (int i = 0; i < variables->count; i++)
    {
        switch (variables->vars[i]->type)
        {
        case INTEGER:
            printf("%s = %d\n", variables->vars[i]->var, variables->vars[i]->integer.value);
            break;
        }
    }
}
