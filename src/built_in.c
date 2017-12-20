#include <stdio.h>
#include "built_in.h"

built_ins_t *get_built_ins() {
    built_in_t *built_ins = (built_in_t *)malloc(sizeof(built_in_t) * 3);

    built_ins[0] = { "cd", build_in_cd };
    built_ins[1] = { "fg", build_in_fg };
    built_ins[2] = { "ex", build_in_ex };


    built_ins_t *bi = (built_ins_t *)malloc(sizeof(built_ins_t));
    bi->built_in = built_ins;

    /* = {
     *  {"cd", build_in_cd},
     *  {"ex", build_in_ex},
     *  {"fg", build_in_fg}
     * };
     */
    return bi;
}


/*
 * cd :
 * ~ -> go to home (getenv("HOME"))
 *  -> go to home (//)
 * path -> go to path
 *
 * return 0
 */
//exit : exit
int build_in_cd(command_t *cmd) {
    int ret;

    if ((cmd->argv[1] == NULL) || (cmd->argv[1][0] == '~')) {
        ret = chdir(getenv("HOME"));
    } else {
        ret = chdir(getenv("HOME"));
    }
    if (ret == -1) {
        printf("built_in.c:~35 OMG WTF BBQ");
    }
    return 0;
}
