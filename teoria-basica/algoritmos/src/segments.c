#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * BSS Segment: (BBS es Block Started by Symbol) Variables sin inicializar
 * Data Segment: Variables inicializadas + Variables globales y Variables Estáticas
 * Text Segment: Instrucciones Stack Segment: Variables locales,
 * parámetros de funciones, ..
 *
 * - Al principio de todo sin variables declaradas,inicializadas,...
 * - Si declaramos en "main" tres variables, se mantiene
 *
 *  text    data     bss     dec     hex filename
 *  1360     528       8    1896     768 test
 */

// Si declaramos 2 variables globales sin inicializar,
// aumenta +8 Bytes el sector "bss" (variables sin inicializar)
int a, b;

// Si declaramos e inicializamos 1 variable global,
// aumenta +4 Bytes el sector "data"
int c = 90, d;

const int genero = 2; // +4 Bytes en Text Segment

int main(void) {

  const int confirmado = 1; // +4 Bytes en Text Segment
  printf("Hola! tu address: %p\n", &confirmado);
  //printf("probando: %d\n", *(int *)0x7ffd1598a054);

  return 0;
}
