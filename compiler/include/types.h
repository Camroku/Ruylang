#pragma once
#include <enum.h>
#include <stdbool.h>
#pragma region AST definitions
typedef struct integer_ast
{
    int value;
} integer_ast_t;

typedef struct binop_ast
{
    struct ast_node *left;
    struct ast_node *right;
    char op;
} binop_ast_t;

typedef struct unaryop_ast
{
    struct ast_node *right;
    char op;
} unaryop_ast_t;

typedef struct compound_ast
{
    struct ast_node **children;
    int count;
} compound_ast_t;

typedef struct ast_list // TODO: rename this to statement_list_ast
{
    struct ast_node **statements;
    int count;
} ast_list_t;

typedef struct list_ast
{
    struct ast_node **values;
    int count;
} list_ast_t;

typedef struct assign_ast
{
    char *name;
    struct ast_node *value;
} assign_ast_t;

typedef struct func_ast
{
    char *name;
    char *type;
    struct args *arguments;
    struct ast_node *compound;
} func_ast_t;

typedef struct funccall_ast
{
    char *name;
    struct list_ast *arguments;
} funccall_ast_t;

typedef struct declare_ast
{
    char *name;
    char *type;
    struct ast_node *value;
} declare_ast_t;

typedef struct return_ast
{
    struct ast_node *value;
} return_ast_t;

typedef struct var_ast
{
    char *name;
} var_ast_t;

typedef struct empty_ast
{
} empty_ast_t;

typedef struct ast_node // TODO: rename this to node_ast
{
    bool syntax_semicolon_pass;
    ast_node_type_t type;
    union
    {
        struct integer_ast integer;
        struct binop_ast binop;
        struct unaryop_ast unaryop;
        struct compound_ast compound;
        struct assign_ast assign;
        struct var_ast var;
        struct func_ast func;
        struct declare_ast declare;
        struct return_ast returnast;
        struct funccall_ast funccall;
        struct empty_ast empty;
    };
} ast_node_t;
#pragma endregion
#pragma region Variable definitions
typedef struct valint
{
    int value;
} inttype_t;

typedef struct function_arg
{
    char *var;
    vartype_t type;
} func_arg_t;

typedef struct args
{
    struct function_arg **args;
    int count;
} args_t;

typedef struct valfunc
{
    char *type;
    struct args *arguments;
    struct ast_node *compound;
} functype_t;

typedef struct varval
{
    char *var;
    vartype_t type;
    union
    {
        struct valint integer;
        struct valfunc function;
    };
} varval_t;

typedef struct vars
{
    struct varval **vars;
    int count;
    struct vars *parent_scope;
} vars_t;
#pragma endregion
#pragma region Lexer definitions
typedef struct token_struct
{
    tokentype_t type;
    char* value;
    int line;
    int column;
} token_t;

typedef struct tokenlist_struct
{
    struct token_struct* tokens;
    int count;
    int index;
} tokenlist_t;

typedef struct keyword_identifier {
    char* key;
    int value;
} keyword_id_t;
#pragma endregion
