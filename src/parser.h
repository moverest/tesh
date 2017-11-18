#ifndef PARSER_H_IMPORTED
#define PARSER_H_IMPORTED

#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include "tokenizer.h"

typedef struct {
    char **argv;
} command_t;

typedef enum {
    GO_ON_IF_SUCCESS,
    GO_ON_IF_FAILURE,
    GO_ON_NEVER
} go_on_condition_t;

typedef struct {
    command_t         *cmds;
    size_t            num_commands;
    go_on_condition_t go_on_condition;
    char              *redirect_in_file;
    char              *redirect_out_file;
    bool              redirect_append;
} statement_t;

typedef struct {
    statement_t *statements;
    size_t      num_statements;
    bool        bg;
} statements_t;

typedef struct {
    int fd[2];
} pipe_t;

statements_t* parse_statement(tokenizer_t* tokens);
int exec_statements(statements_t *t);
statement_t* new_statement();

#endif
