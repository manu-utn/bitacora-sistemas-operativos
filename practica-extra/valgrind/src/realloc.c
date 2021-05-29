#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  char *buffer;

  buffer = (char *) malloc(sizeof(char)*5);
  strcpy(buffer, "hola");
  printf("string: %s, address=%u\n", buffer, buffer);

  buffer = (char *) realloc(buffer, 20);
  // con strcat() concatenamos el string que ya contiene el buffer
  // con el string del segundo parámeto
  strcat(buffer, " que tal?");

  printf("string: %s, address=%u\n", buffer, buffer);

  // Si solo quisieramos mostrar los datos sin formatear, usemos fputs() ó puts()
  // fputs(buffer, stdout);
  // puts(buffer);

  // lo colocamos a modo de que NO finalice el programa, y quede en ejecución
  getchar();

  free(buffer);

  return 0;
}
