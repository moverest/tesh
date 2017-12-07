#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "test.h"
#include "tools.h"
#include "tokenizer.h"

static bool token_equals(token_t *t1, token_t *t2) {
    return(t1->type == t2->type &&
           (t1->type != TOKEN_STRING || !strcmp(t1->str, t2->str)));
}


static void token_print(token_t *t) {
    printf("{%d, %s}", t->type, t->str);
}


static void test_tokenizer_1(test_t *t) {
    char    *test_buf          = "echo 1 && echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_AND,    NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_NEXT,   NULL,   0)
    };


    tokenizer_t *tokenizer = new_tokenizer(test_buf);

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_t *token_temp = tokenizer_next(tokenizer);
        if (!token_equals(token_temp, expected_tokens[i])) {
            printf("Got ");
            token_print(token_temp);
            printf(" but expected ");
            token_print(expected_tokens[i]);
            puts("");
            test_fail(t);
        }
        free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }
}


int main() {
    test_function_t tests[] = {
        TEST_FUNCTION(test_tokenizer_1),
    };


    return test_run(tests, ARRAY_LEN(tests));
}
