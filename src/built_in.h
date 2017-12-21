#ifndef BUILT_IN_H_IMPORTED
#define BUILT_IN_H_IMPORTED

#include "parser.h"

typedef struct {
    char *name;
    int (*func)(command_t *cmd);
} builtin_t;

// `builtins` is the builtins vector. To find if a given command is a builtin
// use the `find_builtin` function.
// To lauch a given builtin, use `builtins[i].func(argv)` where `i` is the index
// returned by `find_builtin` and `argv` the arguments.
extern builtin_t builtins[];
extern size_t    num_builtins;

#define NOT_A_BUILTIN    -1

// `find_builtin` tries to find out if the given command name is a built in or
// not. If it is, it returns its position in the builtins vector. Otherwise it
// returns NOT_A_BUILTIN.
int find_builtin(char *name);

#endif
