#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "test.h"
#include "tools.h"
#include "input_reader.h"

/*
 * static void test_get_input(test_t *t) {
 *  char *test_cmd;
 *  char expt_cmd[255] = "echo foo ; echo bar\n";
 *
 *  printf("Please type: \"%s\"\n", expt_cmd);
 *
 *  // First (bad but easy) solution : ask user to write "echo foo ; echo bar"
 *  printf("Please write :\"echo foo ; echo bar\" \n");
 *  test_cmd = get_input(stdin);
 *  if ((test_cmd == NULL) || (strncmp(test_cmd, expt_cmd, 255) != 0)) {
 *      printf("Got : %s(EOL)\nExp : %s(EOL)\n", test_cmd, expt_cmd);
 *
 *      if (test_cmd != NULL) {
 *          free(test_cmd);
 *      }
 *
 *      test_fail(t);
 *  }
 *  free(test_cmd);
 * }
 *
 *
 * int main() {
 *  test_function_t tests[] = {
 *      TEST_FUNCTION(test_get_input)
 *  };
 *
 *
 *  return test_run(tests, ARRAY_LEN(tests));
 * }*/
int main() {
    return 0;
}
