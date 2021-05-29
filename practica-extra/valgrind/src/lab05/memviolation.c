#include <stdio.h>
#include <stdlib.h>

#include <string.h>

int main(int argc, char *argv[]) {
  int i;
  char hello[] = "Hello World!";

  //simple copy routine
  char *str = malloc(strlen(hello));
  for(i = 0; i < strlen(hello); i++) {
    str[i] = hello[i];
  }

  printf("%s\n", str);

  free(str);
  return 0;
}
