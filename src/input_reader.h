#ifndef INPUT_READER_H_IMPORTED
#define INPUT_READER_H_IMPORTED

#include <stdbool.h>
#include <stdio.h>

/** Compute prompt for interactive mode.
    Args :
      return_prompt : weither or not to return computed prompt
      display : weither or not to display computed prompt
    Return :
      if return_prompt, return computed prompt on success
      if not, return '\0'
      anyway, return NULL on failure
*/
char* display_prompt(bool return_prompt, bool display);

/** Retrieve user input from fd.
    Args :
      if fd == stdin, launch interactive mode (AKA display prompt and wait for
        user input).
      else, read from fd.
    Return :
      
      */
char* get_input(FILE* fd);

#endif
