#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>

#include "parser.h"
#include "input_reader.h"

int main(int argc, char const *argv[]) {
    char *(*readline)(char *);
    void (*add_history)(char *);
    void *handle;
    FILE* fdIn = stdin ;
    bool READLINEMOD = false ;
    bool ERRORMOD = false ;

    if (argc == 2){
      if (argv[1] != NULL && argv[1][0] == '-'){
        if (argv[1] != NULL && argv[1][1] == 'r'){
          READLINEMOD = true;
        } else if (argv[1] != NULL && argv[1][1] == 'e') {
          ERRORMOD = true;
        }
      } else {
        fdIn = fopen(argv[1], "r");
      }
    }

    if (READLINEMOD) {
        handle = dlopen("libreadline.so", RTLD_LAZY);

        if (!handle) {
            printf("Erreur au chargement de readline\n");
            return -1;
        }

        readline = (char *(*)(char *)) dlsym(handle, "readline");
        add_history = (void(*)(char *)) dlsym(handle, "add_history");
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
    } while (!at_eof);
    return status_code;
}
