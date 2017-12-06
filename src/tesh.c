#include <stdio.h>

#include "parser.h"
#include "input_reader.h"

int main() {
    char     *buffer = get_input(stdin);
    parser_t *parser = new_parser(buffer);

    while (parser->current_token->type != TOKEN_EOF) {

        compound_statement_t *current_compound = parser_compound(parser);
        //print_compound(current_compound);
        if (current_compound == NULL) {
            perror("Fail to parse buffer.\n");
            free(buffer);
            parser_free(parser);
            return 1;
        }
        exec_compound(current_compound);
        free_compound(current_compound);

        if(parser->current_token->type != TOKEN_NEXT){
          free(buffer);
          parser_free(parser);
          // Execution succeed (or not), so we go on
          buffer = get_input(stdin);
          //printf("Da buffer : %s(EOL)\n", buffer);
          parser = new_parser(buffer);
        } else {
          // current_token->type == TOKEN_NEXT. On avance d'un token, et on
          // laisse la boucle exec la suite :)
          free(parser->current_token);
          parser->current_token = tokenizer_next(parser->tokenizer);
        }
    }

    parser_free(parser);

    return 0;
}
