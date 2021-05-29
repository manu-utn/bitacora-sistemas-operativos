#include <stdio.h>
#include <stdlib.h>

int main(){
  // declaramos un puntero a entero
  int *numeros;
  // hacemos un arreglo con capacidad para dos enteros
  // las únicas posiciones posibles serían 0 y 1
  // (porque 0 es siempre el primero, al menos en C)
  numeros = malloc(sizeof(int)*2);

  // "debería" arrojar error
  *(numeros+2) = 3;
  free(numeros);

  return 0;
}
==24697== Invalid write of size 4
  ==24697==    at 0x109167: main (invalid-write1.c:13)
  ==24697==  Address 0x4a4a048 is 0 bytes after a block of size 8 alloc'd
  ==24697==    at 0x483E77F: malloc (vg_replace_malloc.c:307)
  ==24697==    by 0x10915A: main (invalid-write1.c:10)
