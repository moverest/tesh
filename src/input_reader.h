#ifndef INPUT_READER_H_IMPORTED
#define INPUT_READER_H_IMPORTED

#include <stdbool.h>

/** Compute prompt for interactive mode.
    Args :
      prompt : where to store compupted prompt
      max_len : maximum length for computed prompt
      display : weither or not to display computed prompt
    Return :
      0 - success
      -1 - else
*/
int display_prompt(char* prompt, int max_len, bool display);

/** Wait for user to type a command, and return a clean buffer of 255 char max
terminated by \0 */
int get_input(char* buffer);

#endif
