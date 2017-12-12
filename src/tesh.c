#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>

#include "parser.h"
#include "input_reader.h"


#define READLINEMODE    true

int main() {
    char * (*readline)(char *);

    void (*add_history)(char *);

    if (READLINEMODE) {
        void *handle = dlopen("libreadline.so", RTLD_LAZY);

        if (!handle) {
            printf("Erreur au chargement de readline\n");
            return -1;
        }
    }

    char     *buffer;
    parser_t *parser;
    int      status_code = 0;
    bool     at_eof      = false;

    do {
        if (READLINEMODE) {
            buffer = readline(get_prompt());
            if (buffer[0] != 0) {
                add_history(buffer);
            }
        } else {
            buffer = get_input(stdin, &at_eof);
        }

        parser = new_parser(buffer);

        compound_statement_t *cstatement;
        while ((cstatement = parser_compound(parser)) != NULL) {
            status_code = exec_compound(cstatement);
            free(cstatement);
        }
    } while (!at_eof);
    return status_code;
}
