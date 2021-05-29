#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  const char* nombre   = "carlos";
  const char* apellido = "fernandez";
  char* nombreApellido;

  nombreApellido = malloc(sizeof(char)*60);

  strcat(nombreApellido, nombre);
  puts(nombreApellido);

  strcat(nombreApellido, apellido);
  puts(nombreApellido);

  free(nombreApellido);

  return 0;
}
