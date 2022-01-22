#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char *palabra = NULL, *aux1 = NULL, *aux2 = NULL;
  palabra = malloc(sizeof(char) * 30);

  strcpy(palabra, "carlitos");

  /*
   * Si avanzás en el puntero original con el operador ++
   * sin guardar la referencia en otro puntero
   * perdés la referencia al momento de hacer el free
   * a menos.. que luego hagas -- pero queda feo..
   */

  // Alternativa #1
  for (aux1 = palabra; *aux1; aux1++)
    printf("%c", *aux1);
  printf("\n");

  // Alternativa #2
  aux2 = palabra;
  while (*aux2) {
    printf("%c", *aux2);
    aux2++;
  }
  printf("\n");

  free(palabra);

  return 0;
}
