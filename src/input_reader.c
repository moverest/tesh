#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>

#include "vector.h"

int display_prompt(char* prompt, bool display){
  struct passwd *p = getpwuid(getuid());
  char* user = p->pw_name;
  if (user==NULL)
    perror("Error while retrieving username.\n");

  char host[256];
  if(gethostname(host,256)!=0 || host == NULL)
    perror("Error while retrieving hostname.\n");

  char cwd[256];
  if(getcwd(cwd, 256)== NULL)
    perror("Error while retrieving current working directory.\n");

  // Computed prompt
  int prompt_size = snprintf(NULL, 0, "%s@%s:%s$", user, host, cwd);
  snprintf(prompt, prompt_size+1, "%s@%s:%s$", user, host, cwd);
  if(display)
    printf("%s", prompt);
  return 0;
}

int get_input(char* buffer){

  return 0;
}
