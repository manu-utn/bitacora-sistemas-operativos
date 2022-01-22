// c allocate memory for double pointer
// https://stackoverflow.com/questions/2257735/assigning-memory-to-double-pointer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
  int* numero = malloc(sizeof(int)); // puntero a entero, aloca memoria para 1 entero
  *numero = 5;

  // al ser un puntero, el identificador retornará la dirección de memoria del principio de bloque de memoria alocado
  printf("address:%x\n", numero);

  // 1. al usar el operador de desreferencia/indirección (el asterisco), traemos el valor al que apunta el puntero a entero
  // 2.a podemos castear la variable a un puntero a entero y luego desreferenciarlo para que nos de el valor
  // 2.b podemos desreferenciar sin aclarar que es un puntero a entero (la opción anterior me parece mejor semánticamente)
  printf("valor, %d, valor: %d\n", *numero, *(int*)numero);
  free(numero);

  //-----------------------------------------------------------

  int* numeros = malloc(sizeof(int)*3); // puntero a entero, aloca memoria para 3 enteros (un array int[3] dinámico en heap)
  *numero = 5; // numeros[0]
  *(numero+1) = 10; // numeros[1]
  *(numero+2) = 20; // numeros[2]

  printf("%d %d %d\n", *(int*)numero, *((int*)numero+1), *(numero+2));

  free(numeros);

  //-----------------------------------------------------------

  char* nombre = (char*) malloc(sizeof(char)*30);   // aloca memoria para 30 caracteres, un String de longitud 30
  char** nombres = (char**) malloc(sizeof(char*)*5); // aloca memoria para 5 (char*) osea 5 Strings pero no definí su longitud

  // alocamos memoria para cada string del array de strings
  for(int i=0; i < 5; i++){
    nombres[i] = (char*) malloc(sizeof(char)*30); // aloca memoria para un string de 30 caracteres, longitud=30
  }

  strcpy(nombre, "samuel");
  // nombre="samuel"; // <---- de esta manera no podes hacer free(nombre)

  strcpy(nombres[0], "pedro");
  strcpy(nombres[1], "rodriguez");
  //nombres[1] = "rodriguez"; // <-- de esta forma no podes hacer free(nombres[1])

  printf("nombre:%s\n", nombre);
  printf("nombres[0]:%s, nombres[1]:%s\n", nombres[0], *(nombres+1));


  free(nombre);
  for(int i=0; i < 5; i++) free(nombres[i]); // [3] y [4] no me deja..?
  free(nombres);

  return 0;
}
