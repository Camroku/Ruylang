#include <interpreter.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <lexer.h>
#include <parser.h>
#include <variables.h>
#include <enum.h>

int visit_node(vars_t *variables, ast_node_t *node);

int int_binop(vars_t *variables, ast_node_t *node)
{
    int left = 0, right = 0;
    char op = 0;
    if (node->type == AST_NODE_BINOP)
    {
        left = visit_node(variables, node->binop.left);
        right = visit_node(variables, node->binop.right);
        op = node->binop.op;
    }
    else if (node->type == AST_NODE_UNARYOP)
    {
        right = visit_node(variables, node->unaryop.right);
        op = node->unaryop.op;
    }
    switch (op)
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
    varval_t *existingvar = get_var(variables, node->assign.name);
    if (existingvar == NULL)
    {
        printf("Undefined reference to '%s'.\n", node->assign.name);
        exit(1);
    }
    existingvar->type = VAR_INTEGER;
    existingvar->var = node->assign.name;
    existingvar->integer.value = visit_node(variables, node->assign.value);
    printf("just set %s = %d\n", existingvar->var, existingvar->integer.value);
}

void int_declare(vars_t *variables, ast_node_t *node)
{
    varval_t *existingvar = get_var(variables, node->assign.name);
    if (existingvar != NULL)
    {
        printf("'%s' already exists.\n", node->assign.name);
        exit(1);
    }
    varval_t *variable = malloc(sizeof(varval_t));
    variable->type = VAR_INTEGER;
    variable->var = node->declare.name;
    variable->integer.value = visit_node(variables, node->declare.value);
    set_var(variables, variable);
    printf("just set %s = %d\n", variable->var, variable->integer.value);
}

void int_func(vars_t *variables, ast_node_t *node)
{
    varval_t *existingvar = get_var(variables, node->assign.name);
    if (existingvar != NULL)
    {
        printf("'%s' already exists.\n", node->assign.name);
        exit(1);
    }
    varval_t *variable = malloc(sizeof(varval_t));
    variable->type = VAR_FUNCTION;
    variable->var = node->func.name;
    variable->function.type = node->func.type;
    variable->function.arguments = node->func.arguments;
    variable->function.compound = node->func.compound;
    set_var(variables, variable);
    printf("created function '%s'\n", variable->var);
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
        return int_binop(variables, node);
        break;
    case AST_NODE_COMPOUND:
        int_compound(variables, node, true);
        return 0;
        break;
    case AST_NODE_ASSIGN:
        int_assign(variables, node);
        return 0;
        break;
    case AST_NODE_DECLARE:
        int_declare(variables, node);
        return 0;
        break;
    case AST_NODE_FUNCTION_DECLARE:
        int_func(variables, node);
        return 0;
        break;
    case AST_NODE_RETURN:
        // int_return(variables, node);
        return 0;
        break;
    case AST_NODE_FUNCCALL:
        // int_funccall(variables, node);
        return 0;
        break;
    case AST_NODE_VAR:
        varval_t *variable = get_var(variables, node->var.name);
        if (variable == 0)
        {
            printf("Undefined reference to '%s'.\n", node->var.name);
            exit(1);
        }
        else
        {
            switch (variable->type)
            {
            case VAR_INTEGER:
                return variable->integer.value;
                break;
            case VAR_FUNCTION:
                printf("Invalid - %s is a function", variable->var);
                exit(1);
                break;
            case VAR_UNKNOWN:
                return 0;
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
        case VAR_INTEGER:
            printf("%s = %d\n", variables->vars[i]->var, variables->vars[i]->integer.value);
            break;
        case VAR_FUNCTION:
            printf("%s (func)\n  arguments:\n", variables->vars[i]->var);
            for (int j = 0; j < variables->vars[i]->function.arguments->count; j++)
            {
                printf("    %s %s\n", type_enum_to_str(variables->vars[i]->function.arguments->args[j]->type), variables->vars[i]->function.arguments->args[j]->var);
            }
            break;
        case VAR_UNKNOWN:
            printf("unknown variable %s\n", variables->vars[i]->var);
            break;
        }
    }
}
