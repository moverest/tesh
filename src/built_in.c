#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "built_in.h"

#define BUILT_IN_NUMBER    3

built_ins_t *get_built_ins() {
    built_in_t *built_ins = (built_in_t *)malloc(sizeof(built_in_t) * BUILT_IN_NUMBER);

    built_ins[0] = (built_in_t){
        "cd", &build_in_cd
    };
    built_ins[1] = (built_in_t){
        "fg", &build_in_fg
    };
    built_ins[2] = (built_in_t){
        "exit", &build_in_ex
    };


    built_ins_t *bi = (built_ins_t *)malloc(sizeof(built_ins_t));
    bi->built_in = built_ins;

    return bi;
}


int find_built_in(char *name, built_ins_t *built_ins) {
    char *built_in_name;

    for (int i = 0; i < BUILT_IN_NUMBER; i++) {
        built_in_name = built_ins->built_in[i].name;
        if ((strlen(name) == strlen(built_in_name)) && (strcmp(name, built_in_name) == 0)) {
            return i;
        }
    }
    return -1;
}


void free_built_ins(built_ins_t *bi) {
    if (bi != NULL) {
        if (bi->built_in != NULL) {
            free(bi->built_in);
        }
        free(bi);
    }
}


int build_in_cd(command_t *cmd) {
    int  ret;
    char *path = cmd->argv[1];

    if ((path == NULL) || (path[0] == '~')) {
        ret = chdir(getenv("HOME"));
    } else {
        if ((strlen(path) > 2) && (path[strlen(path) - 1] == '/')) {
            path[strlen(path) - 1] = '\0';
        }
        ret = chdir(cmd->argv[1]);
    }
    if (ret == -1) {
        printf("built_in.c:~35 OMG WTF BBQ");
    }
    return 0;
}


int build_in_ex(command_t *cmd) {
    exit(0);
}


int build_in_fg(command_t *cmd) {
    int   status;
    pid_t pid_to_wait;

    if (cmd->argv[1] == NULL) {
        pid_to_wait = wait(&status);
    } else {
        pid_to_wait = (pid_t)atoi(cmd->argv[1]);
        pid_to_wait = waitpid(pid_to_wait, &status, 0);
    }
    if (pid_to_wait == -1) {
        perror("wait failed");
        return -1;
    }
    int exit_status = WEXITSTATUS(status);
    printf("[%d->%d]\n", pid_to_wait, exit_status);
    return 0;
}
