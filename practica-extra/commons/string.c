#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <string.h>

typedef struct {
  char* nombre;
  void* tipoEmpleado;
  void (*saludar)();
} t_persona;

typedef struct {
  int sueldo;
} t_cajero;

typedef struct {
  int cantidadEmpleados;
} t_jefe;

void saludoFormal(){
  printf("Buen día\n");
}

void saludoInformal(){
  printf("Hola wachin\n");
}

t_jefe* tipoEmpleado(t_persona* persona){
  return ((t_jefe*) persona->tipoEmpleado);
}

// c struct member function
// https://stackoverflow.com/questions/5385147/dereferencing-structure-from-void-type
// https://stackoverflow.com/questions/17052443/c-function-inside-struct

int main(void){
  t_jefe* jefeAmargado = malloc(sizeof(t_jefe));
  jefeAmargado->cantidadEmpleados = 5;

  t_persona* pablo = malloc(sizeof(t_persona));
  pablo->nombre = "hola";
  pablo->saludar = saludoFormal;
  pablo->tipoEmpleado = jefeAmargado;

  //printf("%d\n", ((t_jefe*) pablo->tipoEmpleado)->cantidadEmpleados);
  printf("before %d\n", tipoEmpleado(pablo)->cantidadEmpleados);
  tipoEmpleado(pablo)->cantidadEmpleados = 10;
  printf("after %d\n", tipoEmpleado(pablo)->cantidadEmpleados);
  //strcpy(pablo->nombre, "carlos");

  pablo->saludar();
  return 0;
}
