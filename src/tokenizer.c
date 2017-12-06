#include "tokenizer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Lexer

tokenizer_t *new_tokenizer(char *buf) {
    tokenizer_t *tokenizer = (tokenizer_t *)malloc(sizeof(tokenizer_t));

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
    case ';':
        incr  = 1;
        token = new_token(TOKEN_NEXT, NULL, 0);
        break;
    case '\n':
        token = new_token(TOKEN_END, NULL, 0);
        break;

    case '\0':
    case EOF:
        token = new_token(TOKEN_EOF, NULL, 0);
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
               tokenizer->buf[tokenizer->pos + incr] != '\0' &&
               tokenizer->buf[tokenizer->pos + incr] != '\n') {
            incr++;
        }
        token = new_token(TOKEN_STRING, tokenizer->buf + tokenizer->pos, incr);
    }

    tokenizer->pos += incr;
    return token;
}


token_t *new_token(token_type_t type, char *str, size_t len) {
    token_t *token = (token_t *)malloc(sizeof(token_t));

    if (token == NULL) {
        return NULL;
    }
    token->type = type;
    token->str  = NULL;
    if (str != NULL) {
        token->str = (char *)malloc(sizeof(char) * (len + 1));
        if (token->str == NULL) {
            free(token);
            return NULL;
        }

        memcpy(token->str, str, len);
        token->str[len] = 0;
    }


    return token;
}


char *token_extract(token_t *token) {
    char *str = token->str;

    free(token);
    return str;
}


void token_free(token_t *token) {
    if (token->str != NULL) {
        free(token->str);
    }
    free(token);
}
