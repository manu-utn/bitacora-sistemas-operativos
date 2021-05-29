#include <stdio.h>
#include <stdlib.h> // malloc() y calloc()
#include <string.h> // memset()

int main(void){
  int* arr;

  /******************************* MALLOC ****************************************************/
  // Reservamos memoria con malloc() pero no inicializamos el bloque de memoria

  arr = (int *)malloc(5* sizeof(int)); // 5 * 4 = 20 Bytes = int arr[5]

  if(arr != NULL){
    /* printf("Pudimos reservar espacio en memoria para 5 enteros en un puntero a enteros\n"); */
  }

  arr[0] = 2;   // podemos manejarnos con indices
  *(arr+1) = 5; // ó podemos manejarnos con la arimética de punteros
  *(arr+2) = 7;

  printf("Malloc: ");
  for(int i=0; i < 5; i++){
    /* printf("arr[%d]=%d ", i, arr[i]); */
  }
  printf("\n");

  free(arr);

  /******************************* CALLOC ****************************************************/
  // Reservamos memoria con calloc() e inicializamos el bloque de memoria con 0 (cero)

  arr = (int *)calloc(5, sizeof(int));
  *(arr+2) = 7;

  printf("Calloc: ");
  for(int i=0; i < 5; i++){
    /* printf("arr[%d]=%d ", i, arr[i]); */
  }
  printf("\n");

  free(arr);

  /******************************* MALLOC+MEMSET+REALLOC ************************************************/
  // - Podemos liberar el espacio en memoria que ocupamos, utilizano realloc() en vez de free()
  // - Reservamos memoria con malloc() y con memset() inicializamos el bloque de memoria con 0 (cero)

  arr = (int *)malloc(5* sizeof(int));
  memset(arr, 0, 5*sizeof(int)); // 5*4 = 20 bytes

  printf("Malloc+Memset: ");
  for(int i=0; i < 5; i++)
    printf("arr[%d]=%d ", i, arr[i]);
  printf("\n");

  realloc(arr, 0);
  //free(arr);

  return 0;
}
