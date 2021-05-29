#include <stdio.h>
#include <stdlib.h>

int main(){
  // declaramos un puntero a entero
  int *numeros;

  // reservamos memoria para dos enteros,
  // "similar" a un arreglo int numeros[2]
  // con capacidad para 2 elementos numeros[0] y numeros[1]
  numeros = malloc(sizeof(int)*2);

  printf("el valor de numeros[2] es %d\n", *(numeros+2));
  free(numeros);
  // esto "debería" arroja error
  printf("el valor de numeros[3] es %d\n", *(numeros+3));

  return 0;
}

==24329== Invalid read of size 4
  ==24329==    at 0x1091A0: main (invalid-read2.c:16)
  ==24329==  Address 0x4a4a04c is 4 bytes after a block of size 8 free'd
  ==24329==    at 0x483F9AB: free (vg_replace_malloc.c:538)
  ==24329==    by 0x109197: main (invalid-read2.c:14)
  ==24329==  Block was alloc'd at
  ==24329==    at 0x483E77F: malloc (vg_replace_malloc.c:307)
  ==24329==    by 0x10916A: main (invalid-read2.c:11)

