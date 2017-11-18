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
    statement_t* statements;
    size_t       num_statements;
    bool         bg;
} compound_statement_t;

typedef struct {
    int fd[2];
} pipe_t;

typedef struct {
  tokenizer_t* tokenizer;
  token_t* current_token;
} parser_t;


command_t* parser_cmd(parser_t* parser);
statement_t* parser_statement(parser_t* parser);
compound_statement_t* parser_compound(parser_t* parser);


command_t* new_commande();
void print_command(command_t* cmd);
void print_statement(statement_t* statement);
void print_statements(compound_statement_t* statements);

compound_statement_t* new_compound();
compound_statement_t* parse_statement(tokenizer_t* tokens);
int exec_statements(compound_statement_t *t);
statement_t* new_statement();

#endif
