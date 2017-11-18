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



void print_command(command_t* cmd){
  for (size_t i = 0; cmd->argv[i] != NULL; i++) {
      printf("%s ", cmd->argv[i]);
  }
  puts("");
}

void print_statement(statement_t* statement){
    printf("Cmds :\n");
    for (size_t i = 0; i < statement->num_commands; i++) {
      print_command(&statement->cmds[i]);
    }
    printf("num_commands %ld\n", statement->num_commands);
    printf("GoOnCdt : %d\n", statement->go_on_condition);
    printf("redirect_in_file : %s\n", statement->redirect_in_file );
    printf("redirect_out_file : %s\n", statement->redirect_out_file );
    printf("redirect_app : %d\n", statement->redirect_append );
    puts("");
}

void print_statements(statements_t* statements){
  printf("Statement : \n");
  for (size_t i = 0; i < statements->num_statements; i++) {
    print_statement(&statements->statements[i]);
  }
  printf("num_statements : %ld\n", statements->num_statements);
  printf("bg : %d\n", statements->bg);
}


statements_t* parse_statement(tokenizer_t* tokens){
  /*
  Premierement, on a des tokens. Pour séparer les Statement, c'est qu'on
  croise &&, || ou ;.

  Ensuite a l'intérieur d'un statement, on sépare les commandes quand on
  croise un |.

  Si on croise une redirection de flux, on est content, on la stock au bon
  endroit et on implementera ça plus tard ^^
  */
  // Token
  token_t* current_token ;

  // Statements
  vector_t* statement_vector = make_vector(sizeof(statement_t*));
  statement_t* current_statement = new_statement();

  // Statement
  vector_t* cmds = make_vector(sizeof(command_t*));
  // command_t* current_command_pipeline = new_commande();

  // Commande
  vector_t* argv = make_vector(sizeof(char**));
  char** current_argv;

  current_token = tokenizer_next(tokens);
  while( (current_token)->type != TOKEN_END ){
    // On construit un statements
    while( ((current_token)->type != TOKEN_OR )||
            (current_token->type != TOKEN_AND )){
      // On constrtuit un statement
      while(( (current_token)->type != TOKEN_PIPE)||
              (current_token->type != TOKEN_REDIRECT_IN) ||
              (current_token->type != TOKEN_REDIRECT_OUT) ||
              (current_token->type != TOKEN_REDIRECT_OUT_APP)){
        // On construit une cmd
        if(current_token->type == TOKEN_STRING){
          char* str = token_extract(current_token);
          vector_append(argv, &str);
          current_token = tokenizer_next(tokens);
          printf("On ajoute %s à notre commande courante\n", str);
          fflush(stdout);

        } else {
          //TODO errors
          return NULL;
        }
      }

      switch (current_token->type) {
        case TOKEN_REDIRECT_IN:
          token_free(current_token);
          current_token = tokenizer_next(tokens);
          //TODO errors
          current_statement->redirect_in_file = token_extract(current_token);

          break;

        case TOKEN_REDIRECT_OUT_APP:
          current_statement->redirect_append = true;
        case TOKEN_REDIRECT_OUT:
          token_free(current_token);
          current_token = tokenizer_next(tokens);
          //if(current_token->type == TOKEN_STRING){
            current_statement->redirect_out_file = token_extract(current_token);
          //} else {
            //TODO errors
          //}
          break;

        case TOKEN_PIPE:
          current_argv = (char**) vector_extract_buffer(argv);
          vector_append(cmds, current_argv);
          argv = make_vector(sizeof(char**));
          break;

        default :
          //TODO Handle error correctly
          printf("Syntax error\n");
          return NULL;
      }
    }

    if (current_token->type != TOKEN_OR && current_token->type != TOKEN_AND) {
      //TODO Handle error correctly
      printf("Syntax error\n");
      return NULL;

    } else {

      switch (current_token->type) {
        case TOKEN_AND:
          current_statement->go_on_condition = GO_ON_IF_SUCCESS;
          break;
        case TOKEN_OR:
          current_statement->go_on_condition = GO_ON_IF_FAILURE;
          break;
        default :
          //TODO errors
          break;
      }

      vector_append(statement_vector, &current_statement);
      current_statement = new_statement();

    }

  }

  statements_t* statement_pipeline = (statements_t*) malloc (sizeof(statements_t)) ;
  statement_pipeline->bg = false;
  statement_pipeline->num_statements =  statement_vector->size;
  statement_pipeline->statements = vector_extract_buffer(statement_vector);
  return statement_pipeline ;
}

void static close_all_pipes(pipe_t *pipes, int size) {
    for (size_t i = 0; i < size; i++) {
        close(pipes[i].fd[0]);
        close(pipes[i].fd[1]);
    }
}


statement_t* new_statement(){
  statement_t* statement = (statement_t*) malloc(sizeof(statement_t));
    statement->redirect_append = false;
    statement->num_commands = 0;
    statement->go_on_condition = GO_ON_NEVER;
  return statement;
}

int exec_statements(statements_t *t) {
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
            pid_t current_pid = fork(); //TODO errors
            if (current_pid == 0) {              // On est dans le fils
                if (j > 0) {                     // Si ce n'est pas la premiere commande de la chaine
                    // On redirige l'entrée
                    dup2(pipes[j - 1].fd[0], 0);
                }
                if (j < count_cmd - 1) { // Si ce n'est pas la derniere commande de la chaine
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

        temp1 = (WIFEXITED(status) && (WEXITSTATUS(status) != 0)
                && st.go_on_condition == GO_ON_IF_FAILURE);
        // Vaut true si la condition est || et que le code de retour est est différent de zéro (erreur)
        temp2 = (WIFEXITED(status) && (WEXITSTATUS(status) == 0)
                && st.go_on_condition == GO_ON_IF_SUCCESS);
        // Vaut true si la condition est && et que le code de retour est est égale à zéro (tout)
        //printf("WIFEXITED : %d, WEXITSTATUS: %d, st.goOnCdt : %d (&&:%d, ||:%d), temp1 : %d, temp2 : %d \n", WIFEXITED(status), WEXITSTATUS(status),st.go_on_condition,GO_ON_IF_SUCCESS,GO_ON_IF_FAILURE, temp1, temp2 );
        if (!(temp1 || temp2)) { // Si on ne doit pas continuer, pour une raison ou l'autre
            break;
        }
    }

    return 0;
}
