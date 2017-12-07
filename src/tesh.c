#include <stdio.h>
#include <stdbool.h>

#include "parser.h"
#include "input_reader.h"

int main() {
    char     *buffer;
    parser_t *parser;
    int      status_code = 0;
    bool     at_eof      = false;

    do {
        buffer = get_input(stdin, &at_eof);
        parser = new_parser(buffer);

        compound_statement_t *cstatement;
        while ((cstatement = parser_compound(parser)) != NULL) {
            status_code = exec_compound(cstatement);
            free(cstatement);
        }
    } while (!at_eof);
    return status_code;
}
