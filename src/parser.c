#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>


#include "parser.h"
#include "tools.h"
#include "parser.h"
#include "tokenizer.h"
#include "vector.h"
#include "built_in.h"

//built_ins = get_built_ins();

void free_command(command_t *cmd) {
    for (size_t i = 0; cmd->argv[i] != NULL; i++) {
        //printf("freeing '%s' @ id = %ld\n", cmd->argv[i], i);
        free(cmd->argv[i]);
    }
    if (cmd->argv != NULL) {
        free(cmd->argv);
    }
    if (cmd != NULL) {
        free(cmd);
    }
}


void free_statement(statement_t *st) {
    for (size_t i = 0; i < st->num_commands; i++) {
        free_command(st->cmds[i]);
    }

    if (st->redirect_in_file != NULL) {
        free(st->redirect_in_file);
    }
    if (st->redirect_out_file != NULL) {
        free(st->redirect_out_file);
    }
    if (st != NULL) {
        free(st);
    }
}


void free_compound(compound_statement_t *cp) {
    for (size_t i = 0; i < cp->num_statements; i++) {
        free_statement(cp->statements[i]);
    }
    if (cp != NULL) {
        free(cp);
    }
}


void print_command(command_t *cmd) {
    for (size_t i = 0; cmd->argv[i] != NULL; i++) {
        printf("   %ld: '%s'\n", i, cmd->argv[i]);
    }
}


void print_statement(statement_t *st) {
    puts(" Statement");
    printf("   num_commands: %ld\n", st->num_commands);
    char *go_on_condition_str[] = { "SUCCESS", "FAILURE", "NEVER" };
    printf("   go_on_condition: %s\n",
           go_on_condition_str[st->go_on_condition]);
    printf("   redirect_in_file: %s\n", st->redirect_in_file);
    printf("   redirect_out_file: %s\n", st->redirect_out_file);
    printf("   redirect_app: %d\n", st->redirect_append);
    printf("   Cmds : \n");
    for (size_t i = 0; i < st->num_commands; i++) {
        print_command(st->cmds[i]);
    }
    putchar('\n');
}


void print_compound(compound_statement_t *cp) {
    puts("Compound statement");
    printf(" num_statements: %ld\n", cp->num_statements);
    printf(" bg: %d\n", cp->bg);
    for (size_t i = 0; i < cp->num_statements; i++) {
        print_statement(cp->statements[i]);
    }
    putchar('\n');
}


parser_t *new_parser(char *s) {
    parser_t *parser = (parser_t *)malloc(sizeof(parser_t));

    if (parser == NULL) {
        perror("Error while initializing parser.");
        return NULL;
    }

    parser->tokenizer     = new_tokenizer(s);
    parser->current_token = tokenizer_next(parser->tokenizer);
    if (parser->current_token == NULL) {
        return NULL;
    }

    return parser;
}


void parser_free(parser_t *parser) {
    if (parser == NULL) {
        perror("Error while freeing parser.");
        return;
    }

    if (parser->tokenizer != NULL) {
        tokenizer_free(parser->tokenizer);
    }

    if (parser->current_token != NULL) {
        token_free(parser->current_token);
    }
}


command_t *parser_cmd(parser_t *parser) {
    vector_t *argv = make_vector(sizeof(char *));

    if (parser->current_token->type != TOKEN_STRING) {
        printf("type : %d\n", parser->current_token->type);
        perror("Error while parsing a cmd. First token's type is not string.");
        return NULL;
    }

    while (parser->current_token->type == TOKEN_STRING) {
        char *str = token_extract(parser->current_token);
        vector_append(argv, &str);
        parser->current_token = tokenizer_next(parser->tokenizer);
        if (parser->current_token == NULL) {
            return NULL;
        }
    }

    command_t *cmd = (command_t *)malloc(sizeof(command_t));
    cmd->argv = (char **)vector_extract_buffer(argv);

    return cmd;
}


statement_t *parser_statement(parser_t *p) {
    statement_t *current_statement = new_statement();
    vector_t    *cmds = make_vector(sizeof(command_t *));

    command_t *current_command = parser_cmd(p);

    if (current_command == NULL) {
        perror("There was an error while computing commande");
        return NULL;
    }

    vector_append(cmds, &current_command); //TODO errors

    if (p->current_token->type == TOKEN_REDIRECT_IN) {
        token_free(p->current_token);
        p->current_token = tokenizer_next(p->tokenizer);
        if (p->current_token->type != TOKEN_STRING) {
            perror("Error while parsing a redirection. First next token's type is not string.");
            return NULL;
        }
        current_statement->redirect_in_file = token_extract(p->current_token);
        p->current_token = tokenizer_next(p->tokenizer);
        if (p->current_token == NULL) {
            return NULL;
        }
    }

    while (p->current_token->type == TOKEN_PIPE) {
        token_free(p->current_token);
        p->current_token = tokenizer_next(p->tokenizer);
        if (p->current_token == NULL) {
            return NULL;
        }
        current_command = parser_cmd(p);
        if (current_command == NULL) {
            perror("There was an error while computing commande");
            return NULL;
        }
        vector_append(cmds, &current_command); //TODO errors
    }

    bool append;
    if ((append = (p->current_token->type == TOKEN_REDIRECT_OUT_APP)) ||
        (p->current_token->type == TOKEN_REDIRECT_OUT)) {
        current_statement->redirect_append = append;
        token_free(p->current_token);
        p->current_token = tokenizer_next(p->tokenizer);
        if (p->current_token == NULL) {
            return NULL;
        }
        if (p->current_token->type != TOKEN_STRING) {
            perror("Error while parsing a redirection. First next token's type is not string.");
            return NULL;
        }
        current_statement->redirect_out_file = token_extract(p->current_token);
        p->current_token = tokenizer_next(p->tokenizer);
        if (p->current_token == NULL) {
            return NULL;
        }
    }

    // After a list of commands, there is a &&, || or ;
    switch (p->current_token->type) {
    case TOKEN_AND:
        current_statement->go_on_condition = GO_ON_IF_SUCCESS;
        p->current_token = tokenizer_next(p->tokenizer);
        if (p->current_token == NULL) {
            return NULL;
        }
        break;

    case TOKEN_OR:
        current_statement->go_on_condition = GO_ON_IF_FAILURE;
        p->current_token = tokenizer_next(p->tokenizer);
        if (p->current_token == NULL) {
            return NULL;
        }
        break;

    default:
        //TODO errors
        break;
    }

    current_statement->num_commands = cmds->size;
    current_statement->cmds         = (command_t **)vector_extract_buffer(cmds);

    return current_statement;
}


compound_statement_t *parser_compound(parser_t *p) {
    while (p->current_token->type == TOKEN_END) {
        token_free(p->current_token);
        p->current_token = tokenizer_next(p->tokenizer);
        if (p->current_token == NULL) {
            return NULL;
        }
    }

    if (p->current_token->type == TOKEN_EOF) {
        // We have reach the end of the buffer.
        // There are no more statement to parse.
        return NULL;
    }

    compound_statement_t *current_compound = new_compound();
    vector_t             *statements       = make_vector(sizeof(statement_t *));
    statement_t          *cs;

    cs = parser_statement(p);
    if (cs == NULL) {
        perror("There was an error while computing statement");
        return NULL;
    }
    vector_append(statements, &cs);

    while (p->current_token->type != TOKEN_END &&
           p->current_token->type != TOKEN_BG &&
           p->current_token->type != TOKEN_EOF) {
        cs = parser_statement(p);
        if (cs == NULL) {
            perror("There was an error while computing statement");
            return NULL;
        }
        vector_append(statements, &cs);
    }

    if (p->current_token->type == TOKEN_BG) {
        current_compound->bg = true;
        token_free(p->current_token);
        p->current_token = tokenizer_next(p->tokenizer);
    }

    current_compound->num_statements = statements->size;
    current_compound->statements     = (statement_t **)
                                       vector_extract_buffer(statements);

    return current_compound;
}


compound_statement_t *new_compound() {
    compound_statement_t *statements = (compound_statement_t *)
                                       malloc(sizeof(compound_statement_t));

    statements->bg             = false;
    statements->num_statements = 0;
    return statements;
}


statement_t *new_statement() {
    statement_t *statement = (statement_t *)malloc(sizeof(statement_t));

    statement->redirect_append   = false;
    statement->num_commands      = 0;
    statement->redirect_in_file  = NULL;
    statement->redirect_out_file = NULL;
    statement->go_on_condition   = GO_ON_NEVER;

    return statement;
}


int exec_statement(statement_t *statement, int *status, bool bg_mod) {
    if (statement->num_commands <= 0) {
        return 0;
    }

    int builtinId = find_builtin(statement->cmds[0]->argv[0]);
    if (builtinId != NOT_A_BUILTIN) {
        *status = builtins[builtinId].func(statement->cmds[0]);
        return 0;
    }

    int pd_in[2], pd_out[2];

    pid_t *pids = (pid_t *)malloc(sizeof(pid_t) * statement->num_commands);

    if (pids == NULL) {
        return 1;
    }

#define IS_FIRST_CMD(i)    (i == 0)
#define IS_LAST_CMD(i)     (i + 1 == statement->num_commands)

    for (size_t i = 0; i < statement->num_commands; i++) {
        command_t *cmd_i = statement->cmds[i];
        *pd_in = *pd_out;
        if (!IS_LAST_CMD(i)) {
            pipe(pd_out);
        }

        pid_t child_pid = fork();
        if (child_pid == 0) {
            if (!IS_LAST_CMD(i)) {
                dup2(pd_out[1], 1);
                close(pd_out[0]);
                close(pd_out[1]);
            } else {
                if (statement->redirect_out_file != NULL) {
                    int fd_out;
                    if (statement->redirect_append) {
                        fd_out = open(statement->redirect_out_file,
                                      O_WRONLY | O_CREAT | O_APPEND, 0644);
                    } else {
                        fd_out = open(statement->redirect_out_file,
                                      O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    }
                    if (fd_out == -1) {
                        perror("Error opening redirect out file ");
                        return 1;
                    }

                    dup2(fd_out, 1);
                    close(fd_out);
                }
            }

            if (!IS_FIRST_CMD(i)) {
                dup2(pd_in[0], 0);
                close(pd_in[0]);
            } else {
                if (statement->redirect_in_file != NULL) {
                    int fd_in = open(statement->redirect_in_file, O_RDONLY);

                    if (fd_in == -1) {
                        perror("Error opening redirect in file ");
                        return 1;
                    }

                    dup2(fd_in, 0);
                    close(fd_in);
                }
            }
            int err = execvp(cmd_i->argv[0], cmd_i->argv);
            if (err) {
                int error_num = errno;
                error(1, error_num, "error while starting '%s'",
                      cmd_i->argv[0]);
            }
            exit(2);
        }

        if (!IS_LAST_CMD(i)) {
            close(pd_out[1]);
        }

        if (!IS_FIRST_CMD(i)) {
            close(pd_in[0]);
        }

        pids[i] = child_pid;
    }
    if (bg_mod) {
        printf("[%d]\n", pids[(statement->num_commands) - 1]);
    } else {
        for (size_t i = 0; i < statement->num_commands; i++) {
            waitpid(pids[i], status, 0);
        }
    }

    return 0;
}


int exec_compound(compound_statement_t *cstatement) {
    int last_status_code = 0;

    for (size_t i = 0; i < cstatement->num_statements; i++) {
        int status;
        int err = exec_statement(cstatement->statements[i], &status, cstatement->bg);
        if (err) {
            return err + 100;
        }

        last_status_code = WEXITSTATUS(status);

        go_on_condition_t go_on = cstatement->statements[i]->go_on_condition;

        if ((last_status_code != 0) && (go_on == GO_ON_IF_SUCCESS)) {
            while (i < cstatement->num_statements && cstatement->statements[i]->go_on_condition == GO_ON_IF_SUCCESS) {
                i++;
            }
        } else if ((last_status_code == 0) && (go_on == GO_ON_IF_FAILURE)) {
            while (i < cstatement->num_statements && cstatement->statements[i]->go_on_condition == GO_ON_IF_FAILURE) {
                i++;
            }
        }
    }

    return last_status_code;
}
