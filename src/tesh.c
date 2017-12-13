#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <string.h>

#include "parser.h"
#include "input_reader.h"

int update_flags(int argc, char const *argv[], FILE **fdIn, bool *SCRIPTMOD, bool *READLINEMOD, bool *ERRORMOD) {
    int error;

    for (int i = 1; i < argc; i++) {
        if (argv[i] != NULL) {
            if ((strlen(argv[i]) >= 2) && (argv[i][0] == '-')) {
                if ((strlen(argv[i]) == 2) && (argv[i][1] == 'r')) {
                    //printf("MODE READLINE\n");
                    *READLINEMOD = true;
                    error       += 1;
                } else if ((strlen(argv[i]) == 2) && (argv[i][1] == 'e')) {
                    //printf("MODE QUIT_ON_ERROR\n");
                    *ERRORMOD = true;
                    error    += 2;
                } else {
                    printf("NOT RECOGNIZED OPTION GIVEN.\n");
                    error -= 999;
                }
            } else {
                //printf("MODE READ FROM SCRIPT FILE\n");
                *SCRIPTMOD = true;
                *fdIn      = fopen(argv[i], "r");
                error     += 4;
            }
        } else {
            printf("NOT RECOGNIZED ARG GIVEN.\n");
            error -= 999;
        }
    }
    return error;
}


int main(int argc, char const *argv[]) {
    char *(*readline)(char *);

    void (*add_history)(char *);
    void *handle;
    FILE *fdIn       = stdin;
    bool READLINEMOD = false;
    bool ERRORMOD    = false;
    bool SCRIPTMOD   = false;
    int  err         = 0;

    err = update_flags(argc, argv, &fdIn, &SCRIPTMOD, &READLINEMOD, &ERRORMOD);
    if (err < 0) {
        printf("ERROR WHILE PARSING ARGV\n");
        return -1;
    }

    if (READLINEMOD && !isatty(0)) {
        printf("ERROR : CAN'T READ BOTH FROM SCRIPT AND READLINE INPUT\n");
        return -1;
    }

    if (READLINEMOD) {
        handle = dlopen("libreadline.so", RTLD_LAZY);

        if (!handle) {
            printf("Erreur au chargement de readline\n");
            return -1;
        }

        readline    = (char *(*)(char *))dlsym(handle, "readline");
        add_history = (void (*)(char *))dlsym(handle, "add_history");
    }

    char     *buffer;
    parser_t *parser;
    int      status_code = 0;
    bool     at_eof      = false;

    do {
        if (READLINEMOD) {
            buffer = readline(get_prompt());
            if (buffer[0] != 0) {
                add_history(buffer);
            }
        } else {
            buffer = get_input(fdIn, &at_eof);
        }

        parser = new_parser(buffer);

        compound_statement_t *cstatement;
        while ((cstatement = parser_compound(parser)) != NULL) {
            status_code = exec_compound(cstatement);
            free(cstatement);
        }
        free(buffer);
    } while (!at_eof);

    if (SCRIPTMOD) {
        fclose(fdIn);
    }

    return status_code;
}
