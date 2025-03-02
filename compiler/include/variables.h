#pragma once

typedef enum vartypes {INTEGER} vartype_t;

typedef struct valint
{
    int value;
} inttype_t;

typedef struct varval
{
    char *var;
    vartype_t type;
    union
    {
        inttype_t integer;
    };
} varval_t;

typedef struct vars
{
    varval_t **vars;
    int count;
    struct vars *parent_scope;
} vars_t;

void init_vars(vars_t *varsptr);
vars_t *create_scope(vars_t *varsptr);
varval_t *get_var(vars_t *cr_scope, char *name);
void set_var(vars_t *cr_scope, varval_t *name);