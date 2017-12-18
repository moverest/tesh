#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>

#include "vector.h"

#define HOSTNAME_MAX_LEN     256
#define CWD_MAX_LEN          512
#define INPUT_DEFAULT_CAP    256


char *get_prompt() {
    char *username = getenv("USER");

    if (username == NULL) {
        perror("Error while retrieving user name.\n");
        return NULL;
    }

    char host[HOSTNAME_MAX_LEN];
    int  err = gethostname(host, HOSTNAME_MAX_LEN);

    if (err != 0) {
        perror("Error while retrieving hostname.\n");
        return NULL;
    }

    char cwd[CWD_MAX_LEN];
    if (getcwd(cwd, CWD_MAX_LEN) == NULL) {
        perror("Error while retrieving current working directory.\n");
        return NULL;
    }

    size_t prompt_len = strlen(username) + strlen(host) + strlen(cwd) + 5;
    char   *prompt    = (char *)malloc(sizeof(char) * prompt_len);
    if (prompt == NULL) {
        return NULL;
    }
    snprintf(prompt, prompt_len, "%s@%s:%s$ ", username, host, cwd);
    return prompt;
}


char *get_input(FILE *file, bool *at_eof) {
    int      current_char;
    vector_t *line_vector = make_vector_with_cap(sizeof(char),
                                                 INPUT_DEFAULT_CAP);

    if (isatty(0) && (file == stdin)) {
        char *prompt = get_prompt();
        if (prompt == NULL) {
            perror("Error while computing prompt");
            return NULL;
        }
        printf("%s", prompt);
        free(prompt);
    }

    if (at_eof != NULL) {
        *at_eof = false;
    }

    while ((current_char = getc(file)) != EOF) {
        vector_append(line_vector, &current_char);
        if (current_char == '\n') {
            break;
        }
    }



    if (current_char == EOF) {
        if (at_eof != NULL) {
            *at_eof = true;
        }
    }


    current_char = 0;
    vector_append(line_vector, &current_char);

    return (char *)vector_extract_buffer(line_vector);
}
