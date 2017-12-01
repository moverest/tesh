#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>

#include "vector.h"

int display_prompt(char* prompt, int max_len, bool display){
  struct passwd *p = getpwuid(getuid());
  char* user = p->pw_name;
  if (user==NULL){
    perror("Error while retrieving username.\n");
    return -1;
  }

  char host[256];
  if(gethostname(host,256)!=0 || host == NULL){
    perror("Error while retrieving hostname.\n");
    return -1;
  }

  char cwd[512];
  if(getcwd(cwd, 512)== NULL){
    perror("Error while retrieving current working directory.\n");
    return -1;
  }

  // Computed prompt
  int prompt_size = snprintf(NULL, 0, "%s@%s:%s$", user, host, cwd);
  if(prompt_size > max_len){
    perror("Error : max length for prompt exceeded");
    return -1;
  }
  snprintf(prompt, prompt_size+1, "%s@%s:%s$", user, host, cwd);
  if(display)
    printf("%s", prompt);
  return 0;
}

int get_input(char* buffer){
  char tmp ;
  vector_t* line = make_vector(sizeof(char));

  while((tmp = getchar())!= EOF && tmp != '\n')
    vector_append(line, &tmp);

  int size = line->size;
  memcpy(buffer, (char *) vector_extract_buffer(line), size);
  return 0;
}
