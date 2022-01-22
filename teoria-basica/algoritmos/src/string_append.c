#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *
 * - El primer argumento es la dirección de memoria de un puntero a char &(ptr) ó (char**)
 * - Se le pasa la dirección de memoria del puntero porque hace un realloc
 *
 * - Se le puede pasar solo el puntero, y funciona pero podría fallar...
 * Ej. string_append(&saludo, nombre);
 */
void string_append(char** original, char* string_to_append){
  // al usar el operador (*) estamos desreferenciando el doble puntero,
  // y trabajar con un puntero simple, en este caso un puntero a char osea (char*)
  //
  // un string contiene el principio de la dirección alocada (la que retornó malloc cuando se reservó)
  *original = realloc(*original, strlen(*original) + strlen(string_to_append) + 1);
  strcat(*original, string_to_append);
}

/*
 * - Esta sería la forma "no recomendada"
 *
 * Ej. strcpy(nombre, "Samuel");
 */
void string_append2(char* original, char* string_to_append){
  original = realloc(original, strlen(original) + strlen(string_to_append) + 1);
  strcat(original, string_to_append);
}

int main(void){
  //-------------------------------------------------------------------------------
  char* saludo = malloc(sizeof(char)*20);
  char* saludoFormal = malloc(sizeof(char)*20);
  char* despedida = malloc(sizeof(char)*20);
  char* nombre = malloc(sizeof(char)*40);

  //saludo=NULL;  // <- si hacés esto perdés la referencia de la memoria alocada y que retorna malloc()
  strcpy(despedida, "Chau ");
  strcpy(saludo, "Hola ");

  /*
   * - Resultado: En vez de "Hola" retorna "Cola"
   * - Si hacemos *ptr1 = *ptr2, y ambos son String, es lo mismo que cadena1[0]=cadena[1]
   * estamos haciendo es cambiar el primer caracter de la primera cadena de caracteres
   */
  printf("%c, %c\n", *saludo, *despedida);
  *saludo = *despedida;
  printf("%s\n", saludo);
  *saludo = 'b';
  printf("%s\n", saludo);

  //--------------------------------------------------------------------------------------

  printf("%s %x\n", nombre, &nombre);

  // - Al usar el operado & le estamos pasando la dirección de memoria del puntero (char*) es decir (char**)
  //saludo = NULL; // --> así pierde la referencia del bloque de memoria alocado, y realloc arroja error
  string_append(&saludo, nombre);
  printf("%s, %x, %x\n", saludo, &nombre, &saludo);

  // - Le pasamos la dirección de memoria a la que apunta el puntero (char*)
  // - Si el puntero llegase apuntara otra dirección...?
  // despedida=NULL; // --> pierde la eferencia del bloque de memoria alocado y realloc arroja error
  string_append2(despedida, nombre);
  printf("%s\n", despedida);

  free(saludo);
  free(despedida);
  free(nombre);

  return 0;
}
