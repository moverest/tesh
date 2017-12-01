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

char* display_prompt(bool return_prompt, bool display){
  int size ;
  char* prompt;
  struct passwd *p = getpwuid(getuid());
  char* user = p->pw_name;
  if (user==NULL){
    perror("Error while retrieving username.\n");
    return NULL;
  }

  char host[256];
  if(gethostname(host,256)!=0 || host == NULL){
    perror("Error while retrieving hostname.\n");
    return NULL;
  }

  char cwd[512];
  if(getcwd(cwd, 512)== NULL){
    perror("Error while retrieving current working directory.\n");
    return NULL;
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

  if(return_prompt){
    prompt = (char*) malloc(sizeof(char)*size);
    if(prompt == NULL){
      free(prompt);
      return NULL;
    }
    memcpy(prompt, prompt_buffer, size);

  } else {
    prompt = (char*) malloc(sizeof(char));
    if(prompt == NULL){
      free(prompt);
      return NULL;
    }
    prompt[0] = '\0';
  }

  if(display)
    printf("%s", prompt_buffer);

  free(prompt_buffer);
  return prompt;
}

char* get_input(FILE* fd){
  char tmp ;
  vector_t* line_vector = make_vector(sizeof(char));

  // display_prompt
  if(fd == stdin){
    char* ret = display_prompt(false, true);
    if(ret == NULL){
      perror("Error while computing prompt");
      return NULL;
    }
    free(ret);
  }

  while((tmp = getc(fd))!= EOF && tmp != '\n')
    vector_append(line_vector, &tmp);

  int size = line_vector->size;
  char* line_buffer = (char *) vector_extract_buffer(line_vector);
  char* line = (char*) malloc(sizeof(char)*size);
  if(line == NULL){
    free(line);
    return NULL;
  }
  memcpy(line, line_buffer, size);
  free(line_buffer);
  return line;
}
