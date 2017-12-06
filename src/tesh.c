#include <stdio.h>

#include "parser.h"
#include "input_reader.h"

int main() {
  char *buffer = get_input(stdin);
  parser_t* parser = new_parser(buffer);


  while (parser->current_token->type != TOKEN_EOF) {
      compound_statement_t* current_compound = parser_compound(parser);
      if (current_compound == NULL) {
        perror("Fail to parse buffer.\n");
        parser_free(parser);
        return 1;
      }

      exec_compound(current_compound);
      free_compound(current_compound);
  }

  parser_free(parser);

    return 0;
}
