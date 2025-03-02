#pragma once
#include <parser.h>
#include <enum.h>
#include <types.h>

void init_vars(vars_t *varsptr);
vars_t *create_scope(vars_t *varsptr);
varval_t *get_var(vars_t *cr_scope, char *name);
void set_var(vars_t *cr_scope, varval_t *name);
vartype_t type_str_to_enum(char *type);
char *type_enum_to_str(vartype_t type);
