#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <string.h>

typedef struct{
  char* nombre;
  int sueldo;
} t_empleado;

t_config* cargar_config(char* nombre);
char* config_cargar_string(char* nombre);
void imprimir_empleado(t_empleado* empleado);
t_empleado* create_empleado(char* nombre, int sueldo);
//------------------------------------------------------------------

int main(void){
  t_config* config = cargar_config("config.cfg");
  char* ip;
  char* dominio;
  int puerto;

  dominio = config_get_string_value(config, "DOMINIO");
  ip = config_get_string_value(config, "IP");
  puerto = config_get_int_value(config, "PORT");

  printf("dominio=%s\n", dominio);
  printf("ip=%s\n", ip);
  printf("puerto=%d\n", puerto);

  // ---------------------------
  /*
  // probando..
  int cantidadNombres = 10;
  int longitudNombre = 30;
  char** nombres = malloc(sizeof(char*) * cantidadNombres);
  for(int i=0; i < cantidadNombres; i++){
    nombres[i] = malloc(sizeof(char)*longitudNombre);
  }

  nombres[0] = "pedro";
  nombres[1] = "roberto";

  printf("%s, %s\n", *nombres, *(nombres+1));
  */

  char** colores = config_get_array_value(config, "COLORES");
  printf("primer color: %s \n", colores[0]);
  printf("cantidad strings: %d \n", string_array_size(colores));

  void _imprimir_color(char* color){
    printf("%s ", color);
  }

  string_iterate_lines(colores, _imprimir_color);
  printf("\n");

  // --------------------------------------------------------------
  // arrays

  char** prioridades = config_get_array_value(config, "PRIORIDADES");

  void _imprimir_prioridad(char* prioridad){
    int prioridadFormateada = atoi(prioridad);
    printf("%d ", prioridadFormateada);
  }

  string_iterate_lines(prioridades, _imprimir_prioridad);
  printf("\n");

  // --------------------------------------------------------------
  // diccionario

  t_dictionary* diccionarioEmpleados;
  diccionarioEmpleados = dictionary_create();
  dictionary_put(diccionarioEmpleados, "Carlos", create_empleado("Carlos", 0));
  dictionary_put(diccionarioEmpleados, "Samuel", create_empleado("Samuel", 0));

  int cantidadEmpleados = dictionary_size(diccionarioEmpleados);
  printf("cantidadEmpleados: %d\n", cantidadEmpleados);

  t_empleado* unEmpleado;
  // creamos un elemento con key="Carlos"
  unEmpleado = dictionary_get(diccionarioEmpleados, "Carlos");
  imprimir_empleado(unEmpleado);

  // actualizamos los datos del que tenga como key="Carlos"
  dictionary_put(diccionarioEmpleados, "Carlos", create_empleado("Carlos", 5000));
  // obtenemos nuevamente su información
  unEmpleado = dictionary_get(diccionarioEmpleados, "Carlos");
  imprimir_empleado(unEmpleado);

  void _asignar_sueldos(int _, t_empleado* empleado){
    dictionary_put(diccionarioEmpleados, empleado->nombre, create_empleado(empleado->nombre, 500));
  }

  void _imprimir_empleado(char* _, t_empleado* empleado){
    imprimir_empleado(empleado);
  }

  printf("\n\nempleados:\n");
  dictionary_iterator(diccionarioEmpleados, (void*) _asignar_sueldos);
  dictionary_iterator(diccionarioEmpleados, (void*) _imprimir_empleado);

  // --------------------------------------------------------------
  // diccionario + arrays

  printf("\n\ndiccionario+arrays:\n");

  char** listaEmpleadosNombres = config_get_array_value(config, "EMPLEADOS");
  char** listaEmpleadosSueldos = config_get_array_value(config, "SUELDOS");

  //printf("sueldo: %s\n", string_array_pop(listaEmpleadosSueldos));
  //printf("sueldo: %s\n", listaEmpleadosSueldos[0]);

  t_dictionary* listaEmpleados = dictionary_create();

  int i=0;
  void _asignar_nombre_empleado(char* nombre){
    int sueldo = atoi(string_array_pop(listaEmpleadosSueldos));
    //printf("%d %s\n", atoi(sueldo), nombre);
    dictionary_put(listaEmpleados, nombre, create_empleado(nombre, sueldo));
  }

  string_iterate_lines(listaEmpleadosNombres, _asignar_nombre_empleado);
  //string_iterate_lines(listaEmpleadosNombre, _asignar_sueldo_empleado);

  void _imprimir_info_empleado(char* _, t_empleado* empleado){
    imprimir_empleado(empleado);
  }

  printf("empleados y sus salarios:\n");
  dictionary_iterator(listaEmpleados, (void*) _imprimir_info_empleado);

  return 0;
}

//------------------------------------------------------------------

void imprimir_empleado(t_empleado* unEmpleado){
  printf("nombre=%s, sueldo=%d\n", unEmpleado->nombre, unEmpleado->sueldo);
}

t_empleado* create_empleado(char* nombre, int sueldo){
  t_empleado* empleado = malloc(sizeof(t_empleado));
  empleado->nombre = strdup(nombre);
  empleado->sueldo = sueldo;

  return empleado;
}

t_config* cargar_config(char* nombre){
  t_config* config = config_create(nombre);

  if(config){
    printf("La config ha sido creada\n");
  }
  else{
    printf("La config NO existe!\n");
  }

  return config;
}
