#include <variables.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void init_vars(vars_t *varsptr)
{
    varsptr->count = 0;
    varsptr->parent_scope = NULL;
    varsptr->vars = NULL;
}

vars_t *create_scope(vars_t *varsptr)
{
    vars_t *new_scope = malloc(sizeof(vars_t));
    new_scope->count = 0;
    new_scope->parent_scope = varsptr;
    new_scope->vars = NULL;
    return new_scope;
}

varval_t *get_var(vars_t *cr_scope, char *name)
{
    for (int i = 0; i < cr_scope->count; i++)
    {
        if (strcmp(cr_scope->vars[i]->var, name) == 0)
        {
            return cr_scope->vars[i];
        }
    }
    if (cr_scope->parent_scope != NULL)
    {
        return get_var(cr_scope->parent_scope, name);
    }

    return NULL;
}

void set_var(vars_t *cr_scope, varval_t *variable)
{
    varval_t *var = get_var(cr_scope, variable->var);
    if (var != NULL)
    {
        *var = *variable;
    }
    else
    {
        cr_scope->vars = realloc(cr_scope->vars, sizeof(varval_t) * (cr_scope->count + 1));
        cr_scope->vars[cr_scope->count] = variable;
        cr_scope->count++;
    }
}