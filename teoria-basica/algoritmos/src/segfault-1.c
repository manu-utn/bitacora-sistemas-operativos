#include <stdio.h>
#include <stdlib.h>

int _strlen(char* s){
  char* aux = s;
  while(*aux) aux++;

  return aux-s; // end_address - begin_address = offset
}

int main(void){
  char* palabra;

  palabra = (char*)"Hola";
  printf("strlen(%s)=%d\n", palabra, _strlen(palabra));

  // producimos el segmentation fault, el puntero apunta a 0
  palabra = (char *)0;
  printf("strlen(%s)=%d\n", palabra, _strlen(palabra));

  return 0;
}
