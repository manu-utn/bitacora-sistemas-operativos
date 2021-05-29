#include <stdio.h>
#include <stdlib.h>

int main(){
  int *b = malloc(sizeof(int));

  if(*b){
    printf("Dependemos de una variable no inicializada\n");
  }

  free(b);

  return 0;
}
