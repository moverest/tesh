#ifndef BUILT_IN_H_IMPORTED
#define BUILT_IN_H_IMPORTED

#include "parser.h"

typedef struct {
  char* name;
  int (*func) (command_t* cmd);
} built_in_t ;

typedef struct {
  built_in_t* built_in;
} built_ins_t ;

int build_in_cd(command_t* cmd);
int build_in_fg(command_t* cmd);
int build_in_ex(command_t* cmd);

#endif
