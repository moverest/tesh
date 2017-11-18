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

void print_statements(compound_statement_t* statements){
  printf("Statement : \n");
  for (size_t i = 0; i < statements->num_statements; i++) {
    print_statement(&statements->statements[i]);
  }
  printf("num_statements : %ld\n", statements->num_statements);
  printf("bg : %d\n", statements->bg);
}

void parser_main(tokenizer_t* tokens){

}

command_t* parser_cmd(parser_t* parser){

  vector_t* argv = make_vector(sizeof(char**));

  while(parser->current_token->type == TOKEN_STRING){
    char* str = token_extract(parser->current_token);
    vector_append(argv, &str);
    parser->current_token = tokenizer_next(parser->tokenizer);
  }
  command_t* cmd = (command_t*)malloc(sizeof(command_t));
  cmd->argv=(char**)vector_extract_buffer(argv);

  return cmd;
}

statement_t* parser_statement(parser_t* p){
  //
  statement_t* current_statement = new_statement();

  vector_t* cmds = make_vector(sizeof(command_t*));

  vector_append(cmds,parser_cmd(p));

  while((p->current_token->type == TOKEN_REDIRECT_IN) ||
        (p->current_token->type == TOKEN_REDIRECT_OUT) ||
        (p->current_token->type == TOKEN_REDIRECT_OUT_APP) ||
        (p->current_token->type == TOKEN_PIPE)){
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
        vector_append(cmds,parser_cmd(p));
        //p->current_token = tokenizer_next(p->tokenizer);
        break;

      default :
        break;
    }
  }
  // After a list of commands, there is a &&, || or ;
  switch (p->current_token->type) {
    case TOKEN_AND:
      current_statement->go_on_condition = GO_ON_IF_SUCCESS;
      break;
    case TOKEN_OR:
      current_statement->go_on_condition = GO_ON_IF_FAILURE;
      break;
    default :
      //TODO errors
      //TODO or maybe not
      break;
  }
  current_statement->num_commands = cmds->size;
  current_statement->cmds=vector_extract_buffer(cmds);

  return current_statement;
}

compound_statement_t* parser_compound(parser_t* parser){
  compound_statement_t* temp;
  return temp;
}


compound_statement_t* parse_statement(tokenizer_t* tokens){
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
  statement_t* current_statement= new_statement();
  statement_t* statement_temp;

  // Statement
  vector_t* cmd_vector = make_vector(sizeof(command_t*));
  command_t* current_cmds;

  // Commande
  vector_t* argv = make_vector(sizeof(char**));
  char** current_argv;

  char* str;

  current_token = tokenizer_next(tokens);

  while(current_token->type != TOKEN_END){
    switch (current_token->type) {

      case TOKEN_OR:
      case TOKEN_AND:

        // We just read && or ||
        // We have to extract last cmd first,
          // cmd.append(argv)
        // And then we fill current_statement:
          // cmd.append(argv)
          // set go_on_condition
          // set num_commands
          // extract cmds from cmd_vector
          // set cmds
          // statement_vector.append(current_statement);
          // reset current_statement

        // 1 cmd.append(argv)
        current_argv = (char**) vector_extract_buffer(argv);
        vector_append(cmd_vector, &current_argv);
        argv = make_vector(sizeof(char**));
        printf("line 99 : cmd_vector.append(argv) ; argv = new vector\n");
        //printf("\t cmd_vector -> size : %ld\n", cmd_vector->size);

        // 2 set go_on_condition
        switch (current_token->type) {
          case TOKEN_AND:
            printf("line 105 : setting go_on_condition to &&\n");
            current_statement->go_on_condition = GO_ON_IF_SUCCESS;
            break;
          case TOKEN_OR:
            printf("line 109 : setting go_on_condition to ||\n" );
            current_statement->go_on_condition = GO_ON_IF_FAILURE;
            break;
          default :
            //TODO errors
            //TODO or maybe not
            break;
        }

        // 3 set num_commands
        current_statement->num_commands = cmd_vector->size;
        // 4 extract cmds from cmd_vector
        current_cmds = (command_t*) vector_extract_buffer(cmd_vector);
        // 5 set cmds
        current_statement->cmds = current_cmds;
        // 6 append
        vector_append(statement_vector, current_statement);

        printf("line 127 : test print(current_statement)\n");
        print_statement(current_statement);
        printf("line 127 : statement_vector.append(cur_statement); cur_statement = new vector; \n");
        printf("\t statement_vector -> size : %ld\n", statement_vector->size);

        // 7 reset
        current_statement = new_statement();
        cmd_vector = make_vector(sizeof(command_t*));

        // get next tokens
        token_free(current_token);
        current_token = tokenizer_next(tokens);
        break;



      case TOKEN_REDIRECT_IN:
        token_free(current_token);
        current_token = tokenizer_next(tokens); //TODO errors
        current_statement->redirect_in_file = token_extract(current_token);
        current_token = tokenizer_next(tokens);
        break;

      case TOKEN_REDIRECT_OUT_APP:
        current_statement->redirect_append = true;
      case TOKEN_REDIRECT_OUT:
        token_free(current_token);
        current_token = tokenizer_next(tokens); //TODO errors
        current_statement->redirect_out_file = token_extract(current_token);
        current_token = tokenizer_next(tokens);
        break;

      case TOKEN_PIPE:
        current_argv = (char**) vector_extract_buffer(argv);
        vector_append(cmd_vector, &current_argv);
        argv = make_vector(sizeof(char**));
        token_free(current_token);
        current_token = tokenizer_next(tokens);
        printf("line 165 : cmd_vector.append(argv) ; argv = new vector\n");
        printf("\t cmd_vector -> size : %ld\n", cmd_vector->size);
        break;

      case TOKEN_STRING:
          str = token_extract(current_token);
          vector_append(argv, &str);
          current_token = tokenizer_next(tokens);
          printf("line 173 : argv.append(%s)\n", str);
          fflush(stdout);
          break;

      default :
        //TODO Handle error correctly
        //printf("Syntax error\n");
        //return NULL;
        break;

    }
  }
  // We read ;
  // We have to extract last cmd first,
    // cmd.append(argv)
  // And then we fill current_statement:
    // set go_on_condition
    // set num_commands
    // extract cmds from cmd_vector
    // set cmds
    // statement_vector.append(current_statement);
    // reset current_statement
  // Then we fill compound_statement
    // set bg
    // set num_statements
    // extract statements from statement_vector
    // set statement
  // Clean

  // 1 cmd.append(argv)
  current_argv = (char**) vector_extract_buffer(argv);
  vector_append(cmd_vector, &current_argv);
  argv = make_vector(sizeof(char**));
  printf("line 206 : cmd_vector.append(argv) ; argv = new vector\n");
  //printf("\t cmd_vector -> size : %ld\n", cmd_vector->size);

  // 2 set go_on_condition
  current_statement->go_on_condition = GO_ON_NEVER;

  // 3 set num_commands
  current_statement->num_commands = cmd_vector->size;
  // 4 extract cmds from cmd_vector
  current_cmds = (command_t*) vector_extract_buffer(cmd_vector);
  // 5 set cmds
  current_statement->cmds = current_cmds;
  // 6 append
  vector_append(statement_vector, current_statement);

  printf("line 221 : test print(current_statement)\n");
  print_statement(current_statement);
  printf("line 223 : statement_vector.append(cur_statement); cur_statement = new vector; \n");
  printf("\t statement_vector -> size : %ld\n", statement_vector->size);

  // 7 reset
  cmd_vector = make_vector(sizeof(command_t*));

  // get next tokens
  token_free(current_token);
  current_token = tokenizer_next(tokens);


  compound_statement_t* compound_statement = (compound_statement_t*) malloc (sizeof(compound_statement_t)) ;
  compound_statement->bg = false;
  compound_statement->num_statements =  statement_vector->size;
  statement_temp = vector_extract_buffer(statement_vector);
  compound_statement->statements = statement_temp;


  printf("line 241 : test print(compound_statement)\n");
  print_statements(compound_statement);
  //TODO clean
  return compound_statement;
}

void static close_all_pipes(pipe_t *pipes, int size) {
    for (size_t i = 0; i < size; i++) {
        close(pipes[i].fd[0]);
        close(pipes[i].fd[1]);
    }
}

compound_statement_t* new_compound(){
  compound_statement_t* statements = (compound_statement_t*) malloc(sizeof(compound_statement_t));
  statements->bg = false;
  statements->num_statements = 0;
  return statements;
}

statement_t* new_statement(){
  statement_t* statement = (statement_t*) malloc(sizeof(statement_t));
    statement->redirect_append = false;
    statement->num_commands = 0;
    statement->go_on_condition = GO_ON_NEVER;
  return statement;
}

command_t* new_commande(){
  command_t* command = (command_t*) malloc(sizeof(command_t));
  return command;
}


int exec_statements(compound_statement_t *t) {
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
            if (current_pid == 0) {// On est dans le fils
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

        temp1 = (WIFEXITED(status) && (WEXITSTATUS(status) != 0)
                && st.go_on_condition == GO_ON_IF_FAILURE);
        // Vaut true si la condition est || et que le code de retour est est
        //différent de zéro (erreur)
        temp2 = (WIFEXITED(status) && (WEXITSTATUS(status) == 0)
                && st.go_on_condition == GO_ON_IF_SUCCESS);
        // Vaut true si la condition est && et que le code de retour est est
        // égale à zéro (succes)
        /*printf("WIFEXITED : %d, WEXITSTATUS: %d, st.goOnCdt : %d (&&:%d, ||:%d),
        temp1 : %d, temp2 : %d \n",
        WIFEXITED(status),
        WEXITSTATUS(status),
        st.go_on_condition,GO_ON_IF_SUCCESS,GO_ON_IF_FAILURE, temp1, temp2 );
        */
        if (!(temp1 || temp2)) {
          // Si on ne doit pas continuer, pour une raison ou l'autre
            break;
        }
    }

    return 0;
}
