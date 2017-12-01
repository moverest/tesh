#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "test.h"
#include "tools.h"
#include "input_reader.h"

static void test_prompt(test_t* t){
  char test_prompt[1024] = {0} ;
  char expt_prompt[255] = "john@noah:/home/john/Github/rs2017-martinez-calvet$";

  // Redirect stdout (printf in display_prompt) to our test buffer
  //fclose(stdout);
  //stdout=fmemopen(test_prompt, sizeof(test_prompt), "w");
  //setbuf(stdout,NULL);

  display_prompt(test_prompt, 1024, false);

  if(test_prompt != NULL && strncmp(test_prompt, expt_prompt, 255)!=0){
    printf("Got %s but expected %s \n", test_prompt, expt_prompt);
    test_fail(t);
  }
}

static void test_get_input(test_t* t){
  char* test_cmd = (char*) malloc(sizeof(char)*255);
  char expt_cmd[255] = "echo foo ; echo bar";

  // First (bad but easy) solution : ask user to write "echo foo ; echo bar"
  printf("Please write :\"echo foo ; echo bar\" \n");
  int ret = get_input(test_cmd);
  if(ret!=0 || test_cmd == NULL || strncmp(test_cmd, expt_cmd, 255)!=0){
    printf("Got : %s(EOL)\nExp : %s(EOL)\n", test_cmd, expt_cmd);
    free(test_cmd);
    test_fail(t);
  }
  free(test_cmd);
}


int main() {
    test_function_t tests[] = {
        TEST_FUNCTION(test_prompt),
        TEST_FUNCTION(test_get_input)
    };


    return test_run(tests, ARRAY_LEN(tests));
}
