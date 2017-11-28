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
} compound_statement_t;

typedef struct {
    int fd[2];
} pipe_t;

typedef struct {
    tokenizer_t *tokenizer;
    token_t     *current_token;
} parser_t;


void free_parser(parser_t* parser);

command_t *new_commande();
command_t *parser_cmd(parser_t *parser);
void print_command(command_t *cmd);
void free_command(command_t* cmd);

statement_t *new_statement();
statement_t *parser_statement(parser_t *parser);
void print_statement(statement_t *st);
void free_statement(statement_t* st);

compound_statement_t *new_compound();
compound_statement_t *parser_compound(parser_t *parser);
void print_compound(compound_statement_t *cp);
int exec_compound(compound_statement_t *cp);
void free_compound(compound_statement_t* cp);

#endif
