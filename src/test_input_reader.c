#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "test.h"
#include "tools.h"
#include "input_reader.h"

static void test_prompt(test_t* t){
  // ONLY RELEVANT WHEN RUN ON MY MACHINE. PLEASE UPDATE TO USE
  char* test_prompt ;
  char expt_prompt[255] = "john@noah:/home/john/Github/rs2017-martinez-calvet$";

  // Redirect stdout (printf in display_prompt) to our test buffer
  //fclose(stdout);
  //stdout=fmemopen(test_prompt, sizeof(test_prompt), "w");
  //setbuf(stdout,NULL);

  test_prompt = display_prompt(true, false);

  if(test_prompt == NULL || strncmp(test_prompt, expt_prompt, 255)!=0){
    printf("Got %s but expected %s \n", test_prompt, expt_prompt);

    if(test_prompt != NULL)
      free(test_prompt);

    test_fail(t);
  }
  free(test_prompt);
}

static void test_get_input(test_t* t){
  char* test_cmd;
  char expt_cmd[255] = "echo foo ; echo bar";

  // First (bad but easy) solution : ask user to write "echo foo ; echo bar"
  printf("Please write :\"echo foo ; echo bar\" \n");
  test_cmd = get_input(stdin);
  if(test_cmd == NULL || strncmp(test_cmd, expt_cmd, 255)!=0){
    printf("Got : %s(EOL)\nExp : %s(EOL)\n", test_cmd, expt_cmd);

    if(test_cmd != NULL)
      free(test_cmd);

    test_fail(t);
  }
  free(test_cmd);
}


int main() {
    test_function_t tests[] = {
        //TEST_FUNCTION(test_prompt),
        //TEST_FUNCTION(test_get_input)
    };


    return test_run(tests, ARRAY_LEN(tests));
}
