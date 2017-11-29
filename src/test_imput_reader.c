#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "test.h"
#include "tools.h"
#include "input_reader.h"

static void test_prompt(test_t* t){
  char test_prompt[255] {0} ;
  char expt_prompt[255] = "john@noah:/home/john/Github/rs2017-martinez-calvet$";

  // Redirect stdout (printf in display_prompt) to our test buffer
  fclose(stdout);
  stdout=fmemopen(test_prompt, sizeof(test_prompt), "w");
  setbuf(stdout,NULL);

  display_prompt();

  if(test_prompt != NULL && strncmp(test_prompt, expt_prompt, 255)!=0){
    printf("Got %s but expected %s \n", test_prompt, expt_prompt);
    test_fail(t);
  }
}


int main() {
    test_function_t tests[] = {
        TEST_FUNCTION(test_prompt),
    };


    return test_run(tests, ARRAY_LEN(tests));
}
