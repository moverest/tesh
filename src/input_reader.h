#ifndef INPUT_READER_H_IMPORTED
#define INPUT_READER_H_IMPORTED

#include <stdio.h>
#include <stdbool.h>

// get_prompt returns a string with the prompt (to be displayed).
char *get_prompt();

// get_input retrieves the user input from fd.
// if fd == stdin, it launches the interactive mode (aka. display prompt and
// wait for user input).
// Otherwise it reads from given file.
// Sets *at_eof to true if we've reach the end of the file (false otherwise).
// Returns a string with the entered line.
char *get_input(FILE *file, bool *at_eof);

#endif
