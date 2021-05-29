#include <stdio.h>
#include <stdlib.h>

int main(){
  // declaramos un puntero a entero
  int *numeros;

  // reservamos memoria para dos enteros,
  // "similar" a un arreglo int numeros[2]
  // con capacidad para 2 elementos numeros[0] y numeros[1]
  numeros = malloc(sizeof(int)*2);

  // tratamos de acceder a una posición de memoria "inválida"
  // porque el índice/posición del último elemento, debería ser [1]
  //
  // "deberia" de arroja error
  printf("el valor de numeros[2] es %d\n", *(numeros+2));
  printf("el valor de numeros[3] es %d\n", *(numeros+3));
  free(numeros);

  return 0;
}
/*
  Este es el error que debería de arrojar valgrind..

  ==24120== Invalid read of size 4
  ==24120==    at 0x109194: main (invalid-read.c:18)
  ==24120==  Address 0x4a4a04c is 4 bytes after a block of size 8 alloc'd
  ==24120==    at 0x483E77F: malloc (vg_replace_malloc.c:307)
  ==24120==    by 0x10916A: main (invalid-read.c:11)
*/
