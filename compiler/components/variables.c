#include <variables.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

vars_t *variabless;

void init_vars(vars_t *varsptr)
{
    variabless = varsptr;
    variabless->count = 0;
}

varval_t *get_var(char *name)
{
    for (int i = 0; i < variabless->count; i++)
    {
        if (strcmp(variabless->vars[i]->var, name) == 0)
        {
            return variabless->vars[i];
        }
    }
    return 0;
}

void set_var(varval_t *variable)
{
    bool found = false;
    for (int i = 0; i < variabless->count; i++)
    {
        if (strcmp(variabless->vars[i]->var, variable->var) == 1)
        {
            variabless->vars[i] = variable;
            found = true;
        }
    }
    if (!found)
    {
        variabless->vars = realloc(variabless->vars, sizeof(varval_t) * (variabless->count + 1));
        variabless->vars[variabless->count] = variable;
        variabless->count++;
    }
}