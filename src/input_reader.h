#ifndef INPUT_READER_H_IMPORTED
#define INPUT_READER_H_IMPORTED

/** Compute prompt and display it. */
int display_prompt();

/** wait for user to type a command, and return a clean buffer of 255 char max
terminated by \0 */
int get_input(char* buffer);

#endif
