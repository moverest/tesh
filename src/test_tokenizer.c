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
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_simple(test_t *t) {
    char    *test_buf          = "echo banane";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo",   4),
        new_token(TOKEN_STRING, "banane", 6)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_up_1(test_t *t) {
    char    *test_buf          = "echo \"banane\"";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo",   4),
        new_token(TOKEN_STRING, "banane", 6)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_up_2(test_t *t) {
    char *test_buf = "echo \"banane";
    //token_t *expected_tokens[] = NULL;


    tokenizer_t *tokenizer = new_tokenizer(test_buf);
    token_t     *temp      = tokenizer_next(tokenizer);

    token_free(temp);
    temp = tokenizer_next(tokenizer);
    if (temp != NULL) {
        printf("Got non null return whereas buffer is not valid");
        puts("");
        token_free(temp);
        test_fail(t);
    }
}


static void test_tokenizer_up_3(test_t *t) {
    char    *test_buf          = "echo \"banane\\\"\"";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo",     4),
        new_token(TOKEN_STRING, "banane\"", 7)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_up_4(test_t *t) {
    char    *test_buf          = "echo \"\\\"banane\"";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo",     4),
        new_token(TOKEN_STRING, "\"banane", 7)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_bg_1(test_t *t) {
    char    *test_buf          = "echo 1& echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_BG,     NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_bg_2(test_t *t) {
    char    *test_buf          = "echo 1 &echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_BG,     NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_bg_3(test_t *t) {
    char    *test_buf          = "echo 1&echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_BG,     NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_and_1(test_t *t) {
    char    *test_buf          = "echo 1&& echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_AND,    NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_and_2(test_t *t) {
    char    *test_buf          = "echo 1 &&echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_AND,    NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_and_3(test_t *t) {
    char    *test_buf          = "echo 1&&echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_AND,    NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_or_1(test_t *t) {
    char    *test_buf          = "echo 1|| echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_OR,     NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_or_2(test_t *t) {
    char    *test_buf          = "echo 1 ||echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_OR,     NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_or_3(test_t *t) {
    char    *test_buf          = "echo 1||echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_OR,     NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_pipe_1(test_t *t) {
    char    *test_buf          = "echo 1| echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_PIPE,   NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_pipe_2(test_t *t) {
    char    *test_buf          = "echo 1 |echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_PIPE,   NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_pipe_3(test_t *t) {
    char    *test_buf          = "echo 1|echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_PIPE,   NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_in_1(test_t *t) {
    char    *test_buf          = "echo 1< echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING,      "echo", 4),
        new_token(TOKEN_STRING,      "1",    1),
        new_token(TOKEN_REDIRECT_IN, NULL,   0),
        new_token(TOKEN_STRING,      "echo", 4),
        new_token(TOKEN_STRING,      "2",    1),
        new_token(TOKEN_END,         NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_in_2(test_t *t) {
    char    *test_buf          = "echo 1 <echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING,      "echo", 4),
        new_token(TOKEN_STRING,      "1",    1),
        new_token(TOKEN_REDIRECT_IN, NULL,   0),
        new_token(TOKEN_STRING,      "echo", 4),
        new_token(TOKEN_STRING,      "2",    1),
        new_token(TOKEN_END,         NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_in_3(test_t *t) {
    char    *test_buf          = "echo 1<echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING,      "echo", 4),
        new_token(TOKEN_STRING,      "1",    1),
        new_token(TOKEN_REDIRECT_IN, NULL,   0),
        new_token(TOKEN_STRING,      "echo", 4),
        new_token(TOKEN_STRING,      "2",    1),
        new_token(TOKEN_END,         NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_out_1(test_t *t) {
    char    *test_buf          = "echo 1> echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING,       "echo", 4),
        new_token(TOKEN_STRING,       "1",    1),
        new_token(TOKEN_REDIRECT_OUT, NULL,   0),
        new_token(TOKEN_STRING,       "echo", 4),
        new_token(TOKEN_STRING,       "2",    1),
        new_token(TOKEN_END,          NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_out_2(test_t *t) {
    char    *test_buf          = "echo 1 >echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING,       "echo", 4),
        new_token(TOKEN_STRING,       "1",    1),
        new_token(TOKEN_REDIRECT_OUT, NULL,   0),
        new_token(TOKEN_STRING,       "echo", 4),
        new_token(TOKEN_STRING,       "2",    1),
        new_token(TOKEN_END,          NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_out_3(test_t *t) {
    char    *test_buf          = "echo 1>echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING,       "echo", 4),
        new_token(TOKEN_STRING,       "1",    1),
        new_token(TOKEN_REDIRECT_OUT, NULL,   0),
        new_token(TOKEN_STRING,       "echo", 4),
        new_token(TOKEN_STRING,       "2",    1),
        new_token(TOKEN_END,          NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_append_1(test_t *t) {
    char    *test_buf          = "echo 1>> echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING,           "echo", 4),
        new_token(TOKEN_STRING,           "1",    1),
        new_token(TOKEN_REDIRECT_OUT_APP, NULL,   0),
        new_token(TOKEN_STRING,           "echo", 4),
        new_token(TOKEN_STRING,           "2",    1),
        new_token(TOKEN_END,              NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_append_2(test_t *t) {
    char    *test_buf          = "echo 1 >>echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING,           "echo", 4),
        new_token(TOKEN_STRING,           "1",    1),
        new_token(TOKEN_REDIRECT_OUT_APP, NULL,   0),
        new_token(TOKEN_STRING,           "echo", 4),
        new_token(TOKEN_STRING,           "2",    1),
        new_token(TOKEN_END,              NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_append_3(test_t *t) {
    char    *test_buf          = "echo 1>>echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING,           "echo", 4),
        new_token(TOKEN_STRING,           "1",    1),
        new_token(TOKEN_REDIRECT_OUT_APP, NULL,   0),
        new_token(TOKEN_STRING,           "echo", 4),
        new_token(TOKEN_STRING,           "2",    1),
        new_token(TOKEN_END,              NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_end_1(test_t *t) {
    char    *test_buf          = "echo 1; echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_END,    NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_end_2(test_t *t) {
    char    *test_buf          = "echo 1 ;echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_END,    NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


static void test_tokenizer_end_3(test_t *t) {
    char    *test_buf          = "echo 1;echo 2 ;";
    token_t *expected_tokens[] = {
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "1",    1),
        new_token(TOKEN_END,    NULL,   0),
        new_token(TOKEN_STRING, "echo", 4),
        new_token(TOKEN_STRING, "2",    1),
        new_token(TOKEN_END,    NULL,   0)
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
        token_free(token_temp);
    }

    for (size_t i = 0; i < ARRAY_LEN(expected_tokens); i++) {
        token_free(expected_tokens[i]);
    }

    tokenizer_free(tokenizer);
}


int main() {
    test_function_t tests[] = {
        TEST_FUNCTION(test_tokenizer_1),
        TEST_FUNCTION(test_tokenizer_simple),
        TEST_FUNCTION(test_tokenizer_bg_1),
        TEST_FUNCTION(test_tokenizer_bg_2),
        TEST_FUNCTION(test_tokenizer_bg_3),
        TEST_FUNCTION(test_tokenizer_and_1),
        TEST_FUNCTION(test_tokenizer_and_2),
        TEST_FUNCTION(test_tokenizer_and_3),
        TEST_FUNCTION(test_tokenizer_or_1),
        TEST_FUNCTION(test_tokenizer_or_2),
        TEST_FUNCTION(test_tokenizer_or_3),
        TEST_FUNCTION(test_tokenizer_pipe_1),
        TEST_FUNCTION(test_tokenizer_pipe_2),
        TEST_FUNCTION(test_tokenizer_pipe_3),
        TEST_FUNCTION(test_tokenizer_in_1),
        TEST_FUNCTION(test_tokenizer_in_2),
        TEST_FUNCTION(test_tokenizer_in_3),
        TEST_FUNCTION(test_tokenizer_out_1),
        TEST_FUNCTION(test_tokenizer_out_2),
        TEST_FUNCTION(test_tokenizer_out_3),
        TEST_FUNCTION(test_tokenizer_append_1),
        TEST_FUNCTION(test_tokenizer_append_2),
        TEST_FUNCTION(test_tokenizer_append_3),
        TEST_FUNCTION(test_tokenizer_end_1),
        TEST_FUNCTION(test_tokenizer_end_2),
        TEST_FUNCTION(test_tokenizer_end_3),
        TEST_FUNCTION(test_tokenizer_up_1),
        TEST_FUNCTION(test_tokenizer_up_2),
        TEST_FUNCTION(test_tokenizer_up_3),
        TEST_FUNCTION(test_tokenizer_up_4)
    };


    return test_run(tests, ARRAY_LEN(tests));
}
