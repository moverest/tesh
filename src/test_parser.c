#include "test.h"
#include "parser.h"
#include "tools.h"

#include <string.h>
#include <unistd.h>

static void test_parser_exec(test_t *t) {
    compound_statement_t test_statements = {
        .bg         = false,
        .statements = (statement_t[]){
            (statement_t){
                .cmds              = (command_t[]){
                    {
                        .argv      = (char *[]){"echo","-e", "banana\n coco\n foo\n ssh", NULL }
                    },
                    {
                        .argv      = (char *[]){"echo","coucou \n", NULL }
                    },
                    {
                        .argv      = (char *[]){"grep","banana", NULL }
                    }
                },
                .num_commands      = 3,
                .go_on_condition   = GO_ON_IF_FAILURE,
                .redirect_in_file  = NULL,
                .redirect_out_file = NULL,
                .redirect_append   = false
            }, (statement_t){
                .cmds              = (command_t[]){
                    {
                        .argv      = (char *[]){"true",NULL }
                    }
                },
                .num_commands      = 1,
                .go_on_condition   = GO_ON_IF_SUCCESS,
                .redirect_in_file  = NULL,
                .redirect_out_file = NULL,
                .redirect_append   = false
            }, (statement_t){
                .cmds              = (command_t[]){
                    {
                        .argv      = (char *[]){"ls", "-la", NULL }
                    }
                },
                .num_commands      = 1,
                .go_on_condition   = GO_ON_NEVER,
                .redirect_in_file  = NULL,
                .redirect_out_file = NULL,
                .redirect_append   = false
            }
        },
        .num_statements = 3,
        .bg             = false
    };

    if (exec_statements(&test_statements) != 0) {
        test_fail(t);
    }
}

static void test_parser_cmd(test_t *t) {
  char    *test_buf          = "ls -l -a";
  tokenizer_t *tokenizer = new_tokenizer(test_buf);
  parser_t parser_test = {
    .tokenizer = tokenizer,
    .current_token = tokenizer_next(tokenizer)
  };

  command_t expected_command_static = {
    .argv = (char *[]) {"ls","-l", "-a", NULL }
  };

  command_t* expected_command = &expected_command_static;
  command_t* test_command = parser_cmd(&parser_test);

  /*printf("Test parser cmd\nExp : ");
  print_command(expected_command);
  printf("Got : ");
  print_command(test_command); */

  for (size_t i = 0; i < sizeof(test_command->argv)/sizeof(test_command->argv[0]); i++) {
    if (strcmp(expected_command->argv[i],test_command->argv[i])!=0) {
        printf("Got %s but expected %s",test_command->argv[i],expected_command->argv[i]);
        puts("");
        test_fail(t);
    }
  }
}

static void test_parser_statement(test_t *t) {
  char    *test_buf          = "echo -e banana coco foo ssh | echo coucou | grep banana || true && ls -la;";
  tokenizer_t *tokenizer = new_tokenizer(test_buf);
  parser_t parser_test = {
    .tokenizer = tokenizer,
    .current_token = tokenizer_next(tokenizer)
  };

  statement_t expected_statement_static = (statement_t){
      .cmds              = (command_t[]){
          {
              .argv      = (char *[]){"echo","-e", "banana coco foo ssh", NULL }
          },
          {
              .argv      = (char *[]){"echo","coucou ", NULL }
          },
          {
              .argv      = (char *[]){"grep","banana", NULL }
          }
      },
      .num_commands      = 3,
      .go_on_condition   = GO_ON_IF_FAILURE,
      .redirect_in_file  = NULL,
      .redirect_out_file = NULL,
      .redirect_append   = false
  };

  statement_t* expected_statement = &expected_statement_static;
  statement_t* test_statement = parser_statement(&parser_test);

  printf("Test parser statement\nExp : ");
  print_statement(expected_statement);
  printf("Got : ");
  print_statement(test_statement);

  /*for (size_t i = 0; i < sizeof(test_command->argv)/sizeof(test_command->argv[0]); i++) {
    if (strcmp(expected_command->argv[i],test_command->argv[i])!=0) {
        printf("Got %s but expected %s",test_command->argv[i],expected_command->argv[i]);
        puts("");
        test_fail(t);
    }
  }*/
}


static void test_parse_statement(test_t *t){
  char    *test_buf          = "echo 1 | cat && echo banane";
  tokenizer_t *tokenizer = new_tokenizer(test_buf);
  compound_statement_t* statements = parse_statement(tokenizer);
  print_statements(statements);
}


int main() {
    test_function_t tests[] = {
        //TEST_FUNCTION(test_parser_exec),
        TEST_FUNCTION(test_parser_cmd),
        TEST_FUNCTION(test_parser_statement)
    };

    return test_run(tests, ARRAY_LEN(tests));
}
