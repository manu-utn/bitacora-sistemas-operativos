#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

int main(void){
  char* nombre;
  printf(COLOR_RED "[BEFORE MALLOC]\n" COLOR_RESET);
  printf("> Value: %s, (STACK) address_var=%p\n", nombre, (void*)&nombre);
  printf("> address_begin=%u, address_end=%p\n\n", nombre, (void*)&nombre[4]);

  nombre = (char*) malloc(sizeof(char)*5);
  strcpy(nombre, "fede");
  printf(COLOR_GREEN "[AFTER MALLOC]\n" COLOR_RESET);
  printf("> Value: %s, Length=%d, (STACK) address_var=%p\n", nombre, strlen(nombre), (void*)&nombre);
  printf("> (heap_by_malloc) address_begin=%p, (heap_by_malloc) address_end=%p\n\n", nombre, (void*)&nombre[strlen(nombre)-1]);

  nombre = realloc(nombre, sizeof(char)*6);
  strcpy(nombre, "fedes");
  printf(COLOR_BLUE "[AFTER REALLOC]\n" COLOR_RESET);
  printf("> Value: %s, Length=%d, (STACK) address_var=%p\n", nombre, strlen(nombre), (void*)&nombre);
  printf("> (heap_by_malloc) address_begin=%p, (heap_by_malloc) address_end=%p\n\n", nombre, (void*)&nombre[strlen(nombre)-1]);

  // - Con strlen(cadena) retorna la cant. de caracteres,
  // como un (char) representa 1 Byte (8 bits) podemos definir su nuevo tamaño sólo diciendo la longitud de la cadena
  // en este caso le agregamos capacidad de 10 Bytes más osea 10 caracteres más.. y le sumamos 1 por el \0 (fin de cadena)
  nombre = realloc(nombre, strlen(nombre)+10+1);
  strcpy(nombre, "federico lopez");
  printf(COLOR_YELLOW "[AFTER REALLOC]\n" COLOR_RESET);
  printf("> Value: %s, Length=%d, (STACK) address_var=%p\n", nombre, strlen(nombre), (void*)&nombre);
  printf("> (heap_by_realloc) address_begin=%p, (heap_by_realloc) address_end=%p\n\n", nombre, (void*)&nombre[strlen(nombre)-1]);

  free(nombre);

  printf("-----------------------------------------------------------------------------------------------------\n\n");
  //-------------------------------------------------------------------------------

  char* palabra = (char*) malloc(sizeof(char)*2);
  strcpy(palabra, "pe");
  //printf("begin_address=%x, end_address=%x, string=%s\n", palabra, &(*(palabra+4)), palabra);

  /*
   * - Hacer &palabra[0] es lo mismo que solo poner "palabra" (sin comillas),
   * porque el String ó (char*) contiene el comienzo de la dirección de memoria de su bloque alocado
   *
   * - Hacer sizeof(ptr) ó sizeof(&cadena) ó sizeof(&cadena[0]) retorna el tamaño en bytes
   * de la dirección de memoria pero como stream, es decir la considera como una cadena sin el \0 al final
   *
   * - Si hacemos la diferencia entre principio  y el fin del bloque de memoria que habíamos reservado con malloc
   * nos devolverá el tamaño en Bytes que estamos usando
   */
  printf("> Value: %s | string[0]=First Char: %c | Length=%d | (STACK) address_var=%p\n", palabra, *(palabra+0), strlen(palabra), (void*)&palabra);
  printf("> (heap_by_malloc) address_begin=%p | (heap_by_malloc) address_end=%p\n", palabra, (void*)&palabra[strlen(palabra)-1]);
  printf("> sizeof_an_address_as_string=%d Bytes\n", sizeof(palabra));
  printf("> %d Bytes\n", (void*)&palabra[strlen(palabra)] - (void*)&palabra[0]);

  free(palabra);

  printf("\n-----------------------------------------------------------------------------------------------------\n\n");
  //-------------------------------------------------------------------------------

  int16_t a = 5, b=2;
  //int *c;
  int16_t *c;

  c = (int16_t*) malloc(sizeof(int16_t));
  /*
   * - Definimos un int de 8 bits, pero al guardar en memoria se guarda en Bytes
   * (si hubieramos definido int16_t serían 16bits que representan 2 Bytes)
   *
   * - La memoria principal (RAM) lee/guarda datos en tamaño de Bytes, que es la mínima unidad de lectura/escritura en MP
   * (en el caso del disco osea filesystem, los datos se leen/guardan en tamaño de cluster)
   *
   * - Si definimos un puntero, luego le reservamos un espacio en memoria con malloc
   * y luego hacemos que apunte a una dirección de STACK (una variable que no es puntero)
   * entonces no podremos hacer free
   */

  // liberamos el bloque reservado, porque más adelante haremos que apunte a direcciones del STACK
  free(c);

  c = &a;
  printf("> (STACK) dirección de la variable del puntero=%p\n", (void*)&c);
  printf("> (STACK) dirección de la variable a la que hace referencia=%p\n", (void*)&a);
  printf("> sizeof(int8_t)=%zu Byte (8 bits)\n", sizeof(int8_t)); // retorna 1 Bytes (8bits)
  printf("> sizeof(int16_t)=%zu Bytes (16 bits)\n", sizeof(int16_t)); // retorna 2 Bytes (16bits)
  printf("\n");

  c = &b;
  printf("> (STACK) dirección de la nueva variable a la que hace referencia=%p\n", (void*)&b);
  printf("> (HEAP) nueva dirección a la que apunta del puntero=%p\n", c);
  printf("> (STACK) dirección de memoria de la variable=%p\n", (void*)&c);
  printf("\n");

  /*
   * - Esto de abajo devolverá un "free(): invalid pointer", porque 'c' ahora apunta a una dirección de Stack
   * - No podés liberar del STACK (memoria alocada de forma estática, se reservó al principio)
   * - Sólo podemos liberar del HEAP (memoria alocada de forma dinámica, se reserva en tiempo de ejecución)
   */

  // free(c); // <- si tratamos de descomentar, producirá un error >:(

  return 0;
}
