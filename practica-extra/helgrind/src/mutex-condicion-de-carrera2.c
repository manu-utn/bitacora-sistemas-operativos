/*
 * gcc mutex-condicion-de-carrera2.c -pthread -g -Wall && valgrind --tool=helgrind ./a.out
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/****************************************************************************/
/**** Declaraciones de estructuras && funciones && variables globales ******/
typedef struct{
  int valor;
  pthread_mutex_t mutex;
} contador_t;

static void *rutina_incrementar(void*);
int incrementar_contador(contador_t*);


/*************************************************************/
/**************** Programa Principal  ************************/
int main(){
  contador_t* contador = malloc(sizeof(contador_t));
  contador->valor = 0;
  pthread_t hilos[1];
  int estado_hilo = 0;

  estado_hilo = pthread_create(&hilos[0], NULL, rutina_incrementar, contador);

  if(estado_hilo){
    printf("Oh.. HAY ERROR!! el código de error es %d", estado_hilo);
    exit(-1); // syscall
  }

  int contador_incrementado = incrementar_contador(contador);
  pthread_join(hilos[0], NULL);
  pthread_mutex_destroy(&(contador->mutex));
  free(contador);

  printf("main: incrementé el contador a %d niaka niaka niaka :)\n", contador_incrementado);

  return 0;
}


/*************************************************************/
/********** Definiciones de las funciones ********************/

static void *rutina_incrementar(void* arg){
  contador_t* parametro = (contador_t*)arg;
  printf("hilo: incrementé el contador a %d\n", incrementar_contador(parametro));
  pthread_exit(NULL);
}

int incrementar_contador(contador_t* contador){
  pthread_mutex_t mutex = contador->mutex;
  int valor = 0;

  pthread_mutex_lock(&mutex);
  valor = ++(contador->valor);
  pthread_mutex_unlock(&mutex);

  return valor;
}
