#include "test.h"
#include "parser.h"
#include "tools.h"

#include <stdbool.h>
#include <string.h>
#include <unistd.h>

/*
 *
 *
 * static bool command_equals(command_t *c1, command_t *c2) {
 *  size_t i;
 *
 *  for (i = 0; c1->argv[i] != NULL && c2->argv[i] != NULL; i++) {
 *      if (strcmp(c1->argv[i], c2->argv[i]) != 0) {
 *          return false;
 *      }
 *  }
 *
 *  return c1->argv[i] == NULL && c2->argv[i] == NULL;
 * }
 *
 *
 * static bool statement_equals(statement_t *s1, statement_t *s2) {
 *  if ((s1->num_commands != s2->num_commands) ||
 *      (s1->go_on_condition != s2->go_on_condition) ||
 *      (!(((s1->redirect_in_file == NULL) && (s2->redirect_in_file == NULL)) ||
 *         (strcmp(s1->redirect_in_file, s2->redirect_in_file) == 0))) ||
 *      (!(((s1->redirect_out_file == NULL) && (s2->redirect_out_file == NULL)) ||
 *         (strcmp(s1->redirect_out_file, s2->redirect_out_file) == 0))) ||
 *      (s1->redirect_append != s2->redirect_append)) {
 *      return false;
 *  }
 *
 *  for (size_t i = 0; i < s1->num_commands; i++) {
 *      if (!command_equals(&(s1->cmds[i]), &(s2->cmds[i]))) {
 *          return false;
 *      }
 *  }
 *
 *  return true;
 * }
 *
 *
 * static bool compound_equals(compound_statement_t *c1, compound_statement_t *c2) {
 *  if ((c1->num_statements != c2->num_statements) || (c1->bg != c2->bg)) {
 *      return false;
 *  }
 *
 *  for (size_t i = 0; i < c1->num_statements; i++) {
 *      if (!statement_equals(&(c1->statements[i]), &(c2->statements[i]))) {
 *          return false;
 *      }
 *  }
 *
 *  return true;
 * }
 *
 *
 * static void test_parser_exec(test_t *t) {
 *  compound_statement_t test_compound = {
 *      .bg         = false,
 *      .statements = (statement_t[]){
 *          (statement_t){
 *              .cmds              = (command_t[]){
 *                  {
 *                      .argv      = (char *[]){"echo","-e",              "banana\\ncoco\\nfoo\\nssh", NULL }
 *                  },
 *                  {
 *                      .argv      = (char *[]){"echo","coucou \n",       NULL }
 *                  },
 *                  {
 *                      .argv      = (char *[]){"grep","banana",          NULL }
 *                  }
 *              },
 *              .num_commands      = 3,
 *              .go_on_condition   = GO_ON_IF_FAILURE,
 *              .redirect_in_file  = NULL,
 *              .redirect_out_file = NULL,
 *              .redirect_append   = false
 *          }, (statement_t){
 *              .cmds              = (command_t[]){
 *                  {
 *                      .argv      = (char *[]){"true",NULL }
 *                  }
 *              },
 *              .num_commands      = 1,
 *              .go_on_condition   = GO_ON_IF_SUCCESS,
 *              .redirect_in_file  = NULL,
 *              .redirect_out_file = NULL,
 *              .redirect_append   = false
 *          }, (statement_t){
 *              .cmds              = (command_t[]){
 *                  {
 *                      .argv      = (char *[]){"ls", "-la",             NULL }
 *                  }
 *              },
 *              .num_commands      = 1,
 *              .go_on_condition   = GO_ON_NEVER,
 *              .redirect_in_file  = NULL,
 *              .redirect_out_file = NULL,
 *              .redirect_append   = false
 *          }
 *      },
 *      .num_statements = 3,
 *      .bg             = false
 *  };
 *
 *  if (exec_compound(&test_compound) != 0) {
 *      test_fail(t);
 *  }
 * }
 *
 *
 * static void test_parser_cmd(test_t *t) {
 *  char        *test_buf   = "ls -l -a";
 *  tokenizer_t *tokenizer  = new_tokenizer(test_buf);
 *  parser_t    parser_test = {
 *      .tokenizer     = tokenizer,
 *      .current_token = tokenizer_next(tokenizer)
 *  };
 *
 *  command_t expected_command_static = {
 *      .argv = (char *[]) {"ls", "-l", "-a", NULL }
 *  };
 *
 *  command_t *expected_command = &expected_command_static;
 *  command_t *test_command     = parser_cmd(&parser_test);
 *
 *  if (!command_equals(test_command, expected_command)) {
 *      test_fail(t);
 *  }
 * }
 *
 *
 * static void test_parser_statement(test_t *t) {
 *  char        *test_buf   = "echo -e banana\\ncoco\\nfoo\\nssh | echo coucou | grep banana || true && ls -la;";
 *  tokenizer_t *tokenizer  = new_tokenizer(test_buf);
 *  parser_t    parser_test = {
 *      .tokenizer     = tokenizer,
 *      .current_token = tokenizer_next(tokenizer)
 *  };
 *
 *  statement_t expected_statement_static = (statement_t){
 *      .cmds = (command_t[]){
 *          {
 *              .argv = (char *[]){
 *                  "echo", "-e", "banana\\ncoco\\nfoo\\nssh", NULL
 *              }
 *          },
 *          {
 *              .argv = (char *[]){
 *                  "echo", "coucou", NULL
 *              }
 *          },
 *          {
 *              .argv = (char *[]){
 *                  "grep", "banana", NULL
 *              }
 *          }
 *      },
 *      .num_commands      = 3,
 *      .go_on_condition   = GO_ON_IF_FAILURE,
 *      .redirect_in_file  = NULL,
 *      .redirect_out_file = NULL,
 *      .redirect_append   = false
 *  };
 *
 *  statement_t *expected_statement = &expected_statement_static;
 *  statement_t *test_statement     = parser_statement(&parser_test);
 *
 *  if (!statement_equals(expected_statement, test_statement)) {
 *      test_fail(t);
 *  }
 * }
 *
 *
 * static void test_parser_compound(test_t *t) {
 *  char        *test_buf   = "echo -e banana\\ncoco\\nfoo\\nssh | echo coucou | grep banana || true && ls -la";
 *  tokenizer_t *tokenizer  = new_tokenizer(test_buf);
 *  parser_t    parser_test = {
 *      .tokenizer     = tokenizer,
 *      .current_token = tokenizer_next(tokenizer)
 *  };
 *
 *  compound_statement_t expected_compound_static = {
 *      .bg         = false,
 *      .statements = (statement_t[]){
 *          (statement_t){
 *              .cmds              = (command_t[]){
 *                  {
 *                      .argv      = (char *[]){"echo","-e",              "banana\\ncoco\\nfoo\\nssh", NULL }
 *                  },
 *                  {
 *                      .argv      = (char *[]){"echo","coucou",          NULL }
 *                  },
 *                  {
 *                      .argv      = (char *[]){"grep","banana",          NULL }
 *                  }
 *              },
 *              .num_commands      = 3,
 *              .go_on_condition   = GO_ON_IF_FAILURE,
 *              .redirect_in_file  = NULL,
 *              .redirect_out_file = NULL,
 *              .redirect_append   = false
 *          }, (statement_t){
 *              .cmds              = (command_t[]){
 *                  {
 *                      .argv      = (char *[]){"true",NULL }
 *                  }
 *              },
 *              .num_commands      = 1,
 *              .go_on_condition   = GO_ON_IF_SUCCESS,
 *              .redirect_in_file  = NULL,
 *              .redirect_out_file = NULL,
 *              .redirect_append   = false
 *          }, (statement_t){
 *              .cmds              = (command_t[]){
 *                  {
 *                      .argv      = (char *[]){"ls", "-la",             NULL }
 *                  }
 *              },
 *              .num_commands      = 1,
 *              .go_on_condition   = GO_ON_NEVER,
 *              .redirect_in_file  = NULL,
 *              .redirect_out_file = NULL,
 *              .redirect_append   = false
 *          }
 *      },
 *      .num_statements = 3,
 *      .bg             = false
 *  };
 *
 *  compound_statement_t *expected_compound = &expected_compound_static;
 *  compound_statement_t *test_compound     = parser_compound(&parser_test);
 *
 *  if (!compound_equals(expected_compound, test_compound)) {
 *      test_fail(t);
 *  }
 * }
 */
int main() {
    /*test_function_t tests[] = {
     *    TEST_FUNCTION(test_parser_exec),
     *    TEST_FUNCTION(test_parser_cmd),
     *    TEST_FUNCTION(test_parser_statement),
     *    TEST_FUNCTION(test_parser_compound)
     * };
     *
     * return test_run(tests, ARRAY_LEN(tests));
     */
    return 0;
}
