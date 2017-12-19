#include "built_in.h"

built_ins_t* get_built_ins (){

  built_in_t* built_ins = (built_in_t*) malloc (sizeof(built_in_t)*3);

  built_ins[0] = {"cd", build_in_cd} ;
  built_ins[1] = {"fg", build_in_fg} ;
  built_ins[2] = {"ex", build_in_ex} ;


  built_ins_t* bi = (built_ins_t*) malloc(sizeof(built_ins_t));
  bi->built_in = built_ins ;
  /* = {
      {"cd", build_in_cd},
      {"ex", build_in_ex},
      {"fg", build_in_fg}
    };
  */
  return bi ;
}
