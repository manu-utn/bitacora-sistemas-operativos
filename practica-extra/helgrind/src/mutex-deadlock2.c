/*
 * gcc mutex-deadlock2.c -pthread -g -Wall && valgrind --tool=helgrind ./a.out
 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/****************************************************************************/
/**** Declaraciones de estructuras && funciones && variables globales ******/
pthread_mutex_t mutex1, mutex2;
static void *rutina(void *);


/*************************************************************/
/**************** Programa Principal  ************************/
int main(void){
  pthread_t hilos[1];
  pthread_mutex_init(&mutex1, NULL);
  pthread_mutex_init(&mutex2, NULL);

  int estado_hilo = 0;
  printf("main: creando hilos..\n");
  estado_hilo = pthread_create(&hilos[0], NULL, rutina, NULL);

  if(estado_hilo){
    printf("Error..!\n");
    exit(-1);
  }
  sleep(1);

  pthread_mutex_lock(&mutex1);
  pthread_mutex_lock(&mutex2);
  printf("main: Bloqueando el printf() dentro de main\n");
  pthread_mutex_unlock(&mutex1);
  pthread_mutex_unlock(&mutex2);
  printf("main: Liberando a printf() dentro de main\n");

  pthread_join(hilos[0], NULL);
  pthread_mutex_destroy(&mutex1);
  pthread_mutex_destroy(&mutex2);

  printf("main: Finalizado..\n");
  return 0;
}


/*************************************************************/
/********** Definiciones de las funciones ********************/
static void *rutina(void *arg){
  printf("Hilo: ejecutando...\n");
  sleep(1);

  // IMPORTANTE..!!!
  // Para evitar el "interbloqueo" ó "deadlock" ó "punto muerto"
  // todos los subprocesos deben "bloquear" el acceso al recurso (sección crítica) en el mismo orden
  // en este caso bloquear usando "mutex1" y luego bloquear usando el "mutex2"
  //
  pthread_mutex_lock(&mutex2); // <<<<<<< ESTO produce un posible interbloqueo, debería estar debajo de lock(&mutex1)
  pthread_mutex_lock(&mutex1); //
  printf("Hilo: Bloqueando el printf() dentro de main\n");
  pthread_mutex_unlock(&mutex1);
  pthread_mutex_unlock(&mutex2);
  printf("Hilo: Liberando a printf() dentro de main\n");
  printf("Hilo: Finalizado...\n");
  pthread_exit(NULL);
}
