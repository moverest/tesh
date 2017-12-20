#ifndef TOKENIZER_H_IMPORTED
#define TOKENIZER_H_IMPORTED

#include <stdlib.h>

typedef enum {
    TOKEN_STRING,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_OUT_APP,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_PIPE,
    TOKEN_END,
    TOKEN_BG,
    TOKEN_EOF
} token_type_t;

typedef struct {
    char   *buf;
    size_t pos;
} tokenizer_t;

typedef struct {
    token_type_t type;
    char         *str;
} token_t;

tokenizer_t *new_tokenizer(char *buf);
void tokenizer_free(tokenizer_t *tokenizer);
token_t *tokenizer_next(tokenizer_t *tokenizer);

token_t *new_token(token_type_t type, char *str, size_t len);
void token_free(token_t *token);
char *token_extract(token_t *token);

char *unescape_token_buffer(char *buf);

#endif
