#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>

#include "vector.h"

int vector_append_all_char(vector_t *vector, void *element, int size) {
    char* addr = (char*) element;
    for (int i = 0; i < size && (addr[i] != '\0') && (&addr[i] != NULL); i++) {
      int err = vector_append(vector, &addr[i]);
      if (err) {
          return err;
      }
    }
    return 0;
}

int display_prompt(char* prompt, bool display){
  int size ;
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
  char sep1 = '@';
  char sep2 = ':';
  char sep3 = '$';

  vector_t *prompt_vector = make_vector(sizeof(char));
  vector_append_all_char(prompt_vector, user, sizeof(user));
  vector_append(prompt_vector, &sep1);
  vector_append_all_char(prompt_vector, host, sizeof(host));
  vector_append(prompt_vector, &sep2);
  vector_append_all_char(prompt_vector, cwd, sizeof(cwd));
  vector_append(prompt_vector, &sep3);
  size = prompt_vector->size;
  char* prompt_buffer = (char *) vector_extract_buffer(prompt_vector);

  if(prompt != NULL)
    memcpy(prompt, prompt_buffer, size);

  if(display)
    printf("%s", prompt_buffer);
  return 0;
}

int get_input(char* buffer){
  char tmp ;
  vector_t* line = make_vector(sizeof(char));

  // display_prompt
  display_prompt(NULL, true);

  while((tmp = getchar())!= EOF && tmp != '\n')
    vector_append(line, &tmp);

  int size = line->size;
  memcpy(buffer, (char *) vector_extract_buffer(line), size);
  return 0;
}
