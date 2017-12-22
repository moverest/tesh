#include "tokenizer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tools.h"


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
    case '\n':
        incr  = 1;
        token = new_token(TOKEN_END, NULL, 0);
        break;

    case '\0':
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
        if (tokenizer->buf[tokenizer->pos] == '\"') {
            incr++;
            while (tokenizer->buf[tokenizer->pos + incr] != '\"' ||
                   (
                       tokenizer->buf[tokenizer->pos + incr] == '\"' &&
                       tokenizer->buf[tokenizer->pos + incr - 1] == '\\'
                   )) {
                //printf("l93 : %c\n", tokenizer->buf[tokenizer->pos + incr]);
                incr++;
                if ((tokenizer->pos + incr) == (strlen(tokenizer->buf) + 2)) {
                    perror("Unbalanced \" encontred. Please verify your input");
                    return NULL;
                }
            }
            token = new_token(TOKEN_STRING, tokenizer->buf + tokenizer->pos + 1, incr - 1);
            //we take from pos+1 until incr-1 because we don't want echo "banana" to display "banana" but banana
            incr++;
        } else {
            while (tokenizer->buf[tokenizer->pos + incr] != ' ' &&
                   tokenizer->buf[tokenizer->pos + incr] != '\0' &&
                   tokenizer->buf[tokenizer->pos + incr] != '\n') {
                if ((tokenizer->pos + incr > 0) && (tokenizer->buf[tokenizer->pos + incr - 1] != '\\') && (
                        (tokenizer->buf[tokenizer->pos + incr] == '<') ||
                        (tokenizer->buf[tokenizer->pos + incr] == '>') ||
                        (tokenizer->buf[tokenizer->pos + incr] == ';') ||
                        (tokenizer->buf[tokenizer->pos + incr] == '|') ||
                        (tokenizer->buf[tokenizer->pos + incr] == '&')
                        )) {
                    break;
                }
                incr++;
            }
            token = new_token(TOKEN_STRING, tokenizer->buf + tokenizer->pos, incr);
        }

        if (strchr(token->str, '\\')) {
            token->str = unescape_token_buffer(token->str);
        }
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
    if (token == NULL) {
        return;
    }

    if (token->str != NULL) {
        free(token->str);
    }
    free(token);
}


/*** Code from stack overflow ***/
char *remove_char(char *word, int idToDel) {
    return (char *)memmove(&word[idToDel], &word[idToDel + 1], strlen(word) - idToDel);
}


/*** Code from http://nicolasj.developpez.com/articles/libc/string/#LIII-B ***/
int find_substring_index(const char *string, const char *substring) {
    int index = -1;

    if ((string != NULL) && (substring != NULL)) {
        char *ptr_pos = NULL;
        ptr_pos = (char *)strstr(string, substring);
        if (ptr_pos != NULL) {
            index = ptr_pos - string;
        }
    }
    return index;
}


char *unescape_token_buffer(char *buf) {
    char list[]   = { '&', '|', '<', '>', ';', '"' };
    char *escaped = (char *)malloc(sizeof(char *) * 3);
    int  idToDel;

    escaped[0] = '\\';
    escaped[2] = '\0';
    for (size_t i = 0; i < ARRAY_LEN(list); i++) {
        escaped[1] = list[i];
        while ((idToDel = find_substring_index(buf, escaped)) != -1) {
            remove_char(buf, idToDel);
        }
    }
    // We are good for everything except //
    return buf;
}
