#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>

/*
 * gcc string.c -l commons -o string && ./string
 * gcc string.c -l commons -o string && valgrind ./string
 */
char *_string_repeat(char c, int count){
  char* s = malloc(sizeof(char)*count+1);

  for(int i=0; i < count; ++i) s[i] = c;

  return s;
}

char* _string_duplicate(const char* original){
  return strdup(original);
}

int main(void){
  char* palabra1 = NULL, *palabra2 = NULL;
  palabra1 = _string_repeat('d', 10);
  palabra2 = _string_duplicate(palabra1);

  printf("%s\n", palabra1);
  printf("%s\n", palabra2);

  free(palabra1);
  free(palabra2);

  return 0;
}
