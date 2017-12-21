#ifndef BUILT_IN_H_IMPORTED
#define BUILT_IN_H_IMPORTED

#include "parser.h"


typedef struct {
    char *name;
    int (*func)(command_t *cmd);
} built_in_t;

typedef struct {
    built_in_t *built_in;
} built_ins_t;

extern built_ins_t built_ins;

int build_in_cd(command_t *cmd);
int build_in_fg(command_t *cmd);
int build_in_ex(command_t *cmd);

int find_built_in(char *name, built_ins_t *built_ins);
built_ins_t *get_built_ins();
void free_built_ins(built_ins_t *bi);

#endif
