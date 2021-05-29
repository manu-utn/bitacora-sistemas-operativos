#include <stdio.h>
#include <stdlib.h>

int main(){
  // declaramos un puntero a entero
  int *numeros;
  // hacemos un arreglo con capacidad para dos enteros
  // las únicas posiciones posibles serían 0 y 1
  // (porque 0 es siempre el primero, al menos en C)
  numeros = malloc(sizeof(int)*2);

  free(numeros);
  // tratamos de asignar un valor a la posición 2 del vector
  // osea numeros[1] = 3 pero... "debería" arrojar error
  *(numeros+1) = 3;

  return 0;
}
==24843== Invalid write of size 4
  ==24843==    at 0x109173: main (invalid-write2.c:15)
  ==24843==  Address 0x4a4a044 is 4 bytes inside a block of size 8 free'd
  ==24843==    at 0x483F9AB: free (vg_replace_malloc.c:538)
  ==24843==    by 0x10916A: main (invalid-write2.c:12)
  ==24843==  Block was alloc'd at
  ==24843==    at 0x483E77F: malloc (vg_replace_malloc.c:307)
  ==24843==    by 0x10915A: main (invalid-write2.c:10)
