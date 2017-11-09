#include "tokenizer.h"

#include <stdlib.h>
#include <string.h>

// Lexer

tokenizer_t *new_tokenizer(char *buf) {
    tokenizer_t *tokenizer = malloc(sizeof(tokenizer_t));

    if (tokenizer == NULL) {
        return NULL;
    }
    tokenizer->buf = buf;
    tokenizer->pos = 0;
    return tokenizer;
}


void tokenizer_free(tokenizer_t *tokenizer) {
    free(tokenizer);
}


static void skip_spaces(tokenizer_t *tokenizer) {
    while (tokenizer->buf[tokenizer->pos] == ' ') {
        tokenizer->pos++;
    }
}


token_t *tokenizer_next(tokenizer_t *tokenizer) {
    skip_spaces(tokenizer);


    token_t *token = NULL;
    size_t  incr   = 0;


    switch (tokenizer->buf[tokenizer->pos]) {
    case '\0':
    case ';':
        incr  = 1;
        token = new_token(TOKEN_END, NULL, 0);
        break;

    case '<':
        incr  = 1;
        token = new_token(TOKEN_REDIRECT_IN, NULL, 0);
        break;

    case '>':
        if (tokenizer->buf[tokenizer->pos + 1] == '>') {
            incr  = 2;
            token = new_token(TOKEN_REDIRECT_OUT_APP, NULL, 0);
        } else {
            incr  = 1;
            token = new_token(TOKEN_REDIRECT_OUT, NULL, 0);
        }
        break;

    case '&':
        if (tokenizer->buf[tokenizer->pos + 1] == '&') {
            incr  = 2;
            token = new_token(TOKEN_AND, NULL, 0);
        } else {
            incr  = 1;
            token = new_token(TOKEN_BG, NULL, 0);
        }
        break;

    case '|':
        if (tokenizer->buf[tokenizer->pos + 1] == '|') {
            incr  = 2;
            token = new_token(TOKEN_OR, NULL, 0);
        } else {
            incr  = 1;
            token = new_token(TOKEN_PIPE, NULL, 0);
        }
        break;

    default:
        while (tokenizer->buf[tokenizer->pos + incr] != ' ' &&
               tokenizer->buf[tokenizer->pos + incr] != '\0') {
            incr++;
        }
        token = new_token(TOKEN_STRING, tokenizer->buf + tokenizer->pos, incr);
    }

    tokenizer->pos += incr;
    return token;
}


token_t *new_token(token_type_t type, char *str, size_t len) {
    token_t *token = malloc(sizeof(token_t));

    if (token == NULL) {
        return NULL;
    }
    token->type = type;

    if (str != NULL) {
        token->str = malloc(sizeof(char) * (len + 1));
        if (token->str == NULL) {
            free(token);
            return NULL;
        }

        memcpy(token->str, str, len);
        token->str[len] = 0;
    }


    return token;
}


void token_free(token_t *token) {
    if (token->str != NULL) {
        free(token->str);
    }
    free(token);
}
