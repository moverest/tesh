#include "parser.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tools.h"
#include "parser.h"
#include "tokenizer.h"
#include "vector.h"



void print_command(command_t *cmd) {
    for (size_t i = 0; cmd->argv[i] != NULL; i++) {
        printf("%s ", cmd->argv[i]);
    }
    puts("");
}


void print_statement(statement_t *statement) {
    printf("Cmds :\n");
    for (size_t i = 0; i < statement->num_commands; i++) {
        print_command(&statement->cmds[i]);
    }
    printf("num_commands %ld\n", statement->num_commands);
    printf("GoOnCdt : %d\n", statement->go_on_condition);
    printf("redirect_in_file : %s\n", statement->redirect_in_file);
    printf("redirect_out_file : %s\n", statement->redirect_out_file);
    printf("redirect_app : %d\n", statement->redirect_append);
    puts("");
}


void print_compound(compound_statement_t *statements) {
    printf("Statement : \n");
    printf("num_statements : %ld\n", statements->num_statements);
    printf("bg : %d\n", statements->bg);
    for (size_t i = 0; i < statements->num_statements; i++) {
        print_statement(&statements->statements[i]);
    }
}


void parser_main(tokenizer_t *tokenizer) {
    parser_t parser = {
        .tokenizer     = tokenizer,
        .current_token = tokenizer_next(tokenizer)
    };

    while (parser.current_token->type != TOKEN_EOF) {
        exec_compound(parser_compound(&parser));
    }
}


command_t *parser_cmd(parser_t *parser) {
    vector_t *argv = make_vector(sizeof(char *));

    while (parser->current_token->type == TOKEN_STRING) {
        char *str = token_extract(parser->current_token);
        vector_append(argv, &str);
        parser->current_token = tokenizer_next(parser->tokenizer);
    }
    command_t *cmd = (command_t *)malloc(sizeof(command_t));
    cmd->argv = (char **)vector_extract_buffer(argv);

    return cmd;
}


statement_t *parser_statement(parser_t *p) {
    statement_t *current_statement = new_statement();
    vector_t    *cmds = make_vector(sizeof(command_t));

    vector_append(cmds, parser_cmd(p));

    while ((p->current_token->type == TOKEN_REDIRECT_IN) ||
           (p->current_token->type == TOKEN_REDIRECT_OUT) ||
           (p->current_token->type == TOKEN_REDIRECT_OUT_APP) ||
           (p->current_token->type == TOKEN_PIPE)) {
        switch (p->current_token->type) {
        case TOKEN_REDIRECT_IN:
            token_free(p->current_token);
            p->current_token = tokenizer_next(p->tokenizer); //TODO errors
            current_statement->redirect_in_file = token_extract(p->current_token);
            p->current_token = tokenizer_next(p->tokenizer);
            break;

        case TOKEN_REDIRECT_OUT_APP:
            current_statement->redirect_append = true;

        case TOKEN_REDIRECT_OUT:
            token_free(p->current_token);
            p->current_token = tokenizer_next(p->tokenizer); //TODO errors
            current_statement->redirect_out_file = token_extract(p->current_token);
            p->current_token = tokenizer_next(p->tokenizer);
            break;

        case TOKEN_PIPE:
            token_free(p->current_token);
            p->current_token = tokenizer_next(p->tokenizer); //TODO errors
            vector_append(cmds, parser_cmd(p));
            //p->current_token = tokenizer_next(p->tokenizer);
            break;

        default:
            break;
        }
    }
    // After a list of commands, there is a &&, || or ;
    switch (p->current_token->type) {
    case TOKEN_AND:
        current_statement->go_on_condition = GO_ON_IF_SUCCESS;
        p->current_token = tokenizer_next(p->tokenizer);
        break;

    case TOKEN_OR:
        current_statement->go_on_condition = GO_ON_IF_FAILURE;
        p->current_token = tokenizer_next(p->tokenizer);
        break;

    default:
        //TODO errors
        //TODO or maybe not
        break;
    }
    current_statement->num_commands = cmds->size;
    current_statement->cmds         = (command_t *)vector_extract_buffer(cmds);

    return current_statement;
}


compound_statement_t *parser_compound(parser_t *p) {
    compound_statement_t *current_compound = new_compound();
    vector_t             *statements       = make_vector(sizeof(statement_t));
    statement_t          *cs;

    cs = parser_statement(p);
    //print_statement(cs);
    vector_append(statements, cs);

    while (p->current_token->type != TOKEN_END &&
           p->current_token->type != TOKEN_BG &&
           p->current_token->type != TOKEN_EOF) {
        cs = parser_statement(p);
        //print_statement(cs);
        vector_append(statements, cs);
    }

    if (p->current_token->type == TOKEN_BG) {
        current_compound->bg = true;
    }

    current_compound->num_statements = statements->size;
    current_compound->statements     = (statement_t *)vector_extract_buffer(statements);

    return current_compound;
}


void static close_all_pipes(pipe_t *pipes, int size) {
    for (size_t i = 0; i < size; i++) {
        close(pipes[i].fd[0]);
        close(pipes[i].fd[1]);
    }
}


compound_statement_t *new_compound() {
    compound_statement_t *statements = (compound_statement_t *)malloc(sizeof(compound_statement_t));

    statements->bg             = false;
    statements->num_statements = 0;
    return statements;
}


statement_t *new_statement() {
    statement_t *statement = (statement_t *)malloc(sizeof(statement_t));

    statement->redirect_append = false;
    statement->num_commands    = 0;
    statement->go_on_condition = GO_ON_NEVER;
    return statement;
}


command_t *new_commande() {
    command_t *command = (command_t *)malloc(sizeof(command_t));

    return command;
}


int exec_compound(compound_statement_t *t) {
    int         count_cmd;
    pid_t       *sons;
    int         status;
    statement_t st;
    bool        temp1;
    bool        temp2;

    for (size_t i = 0; i < t->num_statements; i++) { // Pour chaque statement_t
        st        = t->statements[i];
        count_cmd = st.num_commands;
        pipe_t pipes[count_cmd - 1];

        for (size_t i = 0; i < count_cmd - 1; i++) {
            pipe(pipes[i].fd);
        }

        sons = (int *)malloc(sizeof(int) * count_cmd);

        for (size_t j = 0; j < count_cmd; j++) { // Pour chaque command_t
            pid_t current_pid = fork();          //TODO errors
            if (current_pid == 0) {              // On est dans le fils
                if (j > 0) {
                    // Si ce n'est pas la premiere commande de la chaine
                    // On redirige l'entrée
                    dup2(pipes[j - 1].fd[0], 0);
                }
                if (j < count_cmd - 1) {
                    // Si ce n'est pas la derniere commande de la chaine
                    // On redirige la sortie
                    dup2(pipes[j].fd[1], 1);
                }

                close_all_pipes(pipes, count_cmd - 1);

                execvp(st.cmds[j].argv[0], st.cmds[j].argv);
                //TODO errors
                exit(1);
            } else {
                sons[j] = current_pid;
            }
        }

        close_all_pipes(pipes, count_cmd - 1);

        for (size_t j = 0; j < count_cmd; j++) {
            //printf("Pid du fils qu'on attend : %d\n", (int) sons[j]);
            waitpid(sons[j], &status, 0);
        }/**/
        //waitpid(sons[count_cmd-1], &status, 0);
        free(sons);

        temp1 = (WIFEXITED(status) && (WEXITSTATUS(status) != 0) &&
                 st.go_on_condition == GO_ON_IF_FAILURE);
        // Vaut true si la condition est || et que le code de retour est est
        //différent de zéro (erreur)
        temp2 = (WIFEXITED(status) && (WEXITSTATUS(status) == 0) &&
                 st.go_on_condition == GO_ON_IF_SUCCESS);
        // Vaut true si la condition est && et que le code de retour est est
        // égale à zéro (succes)

        /*printf("WIFEXITED : %d, WEXITSTATUS: %d, st.goOnCdt : %d (&&:%d, ||:%d),
         * temp1 : %d, temp2 : %d \n",
         * WIFEXITED(status),
         * WEXITSTATUS(status),
         * st.go_on_condition,GO_ON_IF_SUCCESS,GO_ON_IF_FAILURE, temp1, temp2 );
         */
        if (!(temp1 || temp2)) {
            // Si on ne doit pas continuer, pour une raison ou l'autre
            break;
        }
    }

    return 0;
}
