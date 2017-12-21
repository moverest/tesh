#include <stdio.h>
#include <string.h>
#include "built_in.h"
#include "tools.h"


int find_builtin(char *name) {
    if (name == NULL) {
        return NOT_A_BUILTIN;
    }

    for (size_t i = 0; i < num_builtins; i++) {
        if (strcmp(name, builtins[i].name) == 0) {
            return i;
        }
    }

    return NOT_A_BUILTIN;
}


int builtin_cd(command_t *cmd) {
    int  ret;
    char *path = cmd->argv[1];

    if ((path == NULL) || (path[0] == '~')) {
        ret = chdir(getenv("HOME"));
    } else {
        if ((strlen(path) > 2) && (path[strlen(path) - 1] == '/')) {
            // FIX ME: We should not be editing arguments.
            path[strlen(path) - 1] = '\0';
        }
        ret = chdir(cmd->argv[1]);
    }
    if (ret == -1) {
        printf("built_in.c:~35 OMG WTF BBQ");
    }
    return 0;
}


int builtin_exit(command_t *cmd) {
    exit(0);
}


int builtin_fg(command_t *cmd) {
    return 0;
}


builtin_t builtins[] = {
    { "cd",   builtin_cd   },
    { "exit", builtin_exit },
    { "fg",   builtin_fg   }
};

size_t num_builtins = ARRAY_LEN(builtins);
