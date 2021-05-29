#include <stdio.h>
#include <string.h> // para usar %s en printf()
#include <stdlib.h> // para usar
#include <pthread.h>

/*******************************************************************/
/*** Declaración de funciones, estructuras y variables globales ***/

void *asignarTareas_profesor1(void *);
void *asignarTareas_profesor2(void *);

// supongamos que cada alumno estudia
// - 4 horas por semana (las filas)
// - 3 días a la semana (las columnas)

struct alumno{
  int codigo;
  char nombre[20];
  int jornadaDeEstudio[4][3]; // matrix rectángular de 4x5 (4 filas, 3 columnas)
};

/*************************************************************/
/******************* Programa Principal  *********************/
int main(){
  // - declaramos los hilos
  // - cuando creemos los hilos con pthread_create() se guardará una referencia de ellos
  // en el primer parámetro de esa función invocada
  pthread_t hilo1, hilo2;

  struct alumno carlitos;

  carlitos.codigo = 1;
  strcpy(carlitos.nombre, "Carlitos");

  // Con memset()
  // inicializamos la matriz de 4x3, eliminando valores basura que llegase a tener
  //
  // - 1er parámetro es la dirección de memoria de la matriz ó vector
  // - 2do parámetro, valor con el que se va a rellenar/completar
  // - 3er parámetro cantidad de bytes que  completará
  memset(&carlitos.jornadaDeEstudio, 0, sizeof(int)*4*3);

  printf("Profesores asignandole tareas al alumnado, muahaha :)\n");
  // - creamos los hilos, y le asignamos la rutinas/funciones a ejecutar
  // - el último argumento es el parámetro que se le pasa a la rutina/función que se le asignó al hilo
  pthread_create(&hilo1, NULL, asignarTareas_profesor1, (void *)&carlitos);
  pthread_create(&hilo2, NULL, asignarTareas_profesor2, (void *)&carlitos);

  // - el pthread_join() es una función bloqueante
  // - el programa principal main() deberá esperar que se termine de ejecutar ambos hilos
  // - el orden de ejecución dependerá del "planificador"
  pthread_join(hilo1, NULL);
  pthread_join(hilo2, NULL);

  int dia = 0;
  for(int hora=0; hora < 4; hora++){
    for(int dia=0; dia < 3; dia++){
      printf("[dia=%d, hora=%d, tareas_asignadas=%d], ", dia+1, hora+1, carlitos.jornadaDeEstudio[hora][dia]);
    }
    printf("\n");
  }

  return 0; // fin del programa principal MAIN
}


/*************************************************************/
/**************** Definición de funciones  ******************/

// este profesor le asigna 3 tareas
void *asignarTareas_profesor1(void* arg){
  // declaramos un puntero a una estructura alumno
  struct alumno *unAlumno;

  // casteamos el parámetro que es puntero a nulo, a un puntero a estructura alumno
  unAlumno = (struct alumno *)arg;

  for(int hora=0; hora < 4; hora++){
    for(int dia=0; dia < 3; dia++){
      // simulemos que le agregamos 3 tareas para hacer por cada hora de estudio
      printf("Profesor 1: Ya le asigné tarea a %s\n", unAlumno->nombre);
      unAlumno->jornadaDeEstudio[hora][dia] += 3;
    }
  }
  printf("Profesor 1: Terminé de asignarle tareas..! :(\n");

  pthread_exit(NULL);
}

// este profesor le asigna 5 tareas más
void *asignarTareas_profesor2(void* arg){
  struct alumno *unAlumno;

  unAlumno = (struct alumno *)arg;

  // int jornadaDeEstudio[4][3]; // matrix rectángular de 4x5 (4 filas, 3 columnas)
  for(int hora=0; hora < 4; hora++){
    for(int dia=0; dia < 3; dia++){
      // simulemos que le agregamos 5 tareas para hacer por cada hora de estudio
      printf("Profesor 1: Ya le asigné tarea a %s\n", unAlumno->nombre);
      unAlumno->jornadaDeEstudio[hora][dia] += 5;
    }
  }
  printf("Profesor 2: Terminé de asignarle tareas..! :(\n");

  pthread_exit(NULL);
}
