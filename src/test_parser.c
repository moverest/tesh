#include "test.h"
#include "parser.h"
#include "tools.h"

#include <unistd.h>

static void test_parser_exec(test_t *t) {
    statements_t test_statements = {
        .bg         = false,
        .statements = (statement_t[]){
            (statement_t){
                .cmds              = (command_t[]){
                    {
                        .argv      = (char *[]){"echo","-e",              "banana\n coco\n bite\n ssh", NULL }
                    },
                    {
                        .argv      = (char *[]){"echo","coucou \n",       NULL }
                    },
                    {
                        .argv      = (char *[]){"grep","banana",          NULL }
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
                        .argv      = (char *[]){"ls", "-la",             NULL }
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


int main() {
    test_function_t tests[] = {
        TEST_FUNCTION(test_parser_exec),
    };

    return test_run(tests, ARRAY_LEN(tests));
}
