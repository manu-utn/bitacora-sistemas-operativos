/*
 * gcc mutex-condicion-de-carrera1.c -pthread -g -Wall && valgrind --tool=helgrind ./a.out
 * Obs: En mutex-condicion-carrera2.c utilizamos malloc, typdef
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/****************************************************************************/
/**** Declaraciones de estructuras && funciones && variables globales ******/

// podriamos haber usado typedef, pero lo dejé asi, para saber las estructuras
// en los casteos
struct contador_t{
  int valor;
  pthread_mutex_t mutex;
};

static void *rutina_incrementar(void*);
int incrementar_contador(struct contador_t*);


/*************************************************************/
/**************** Programa Principal  ************************/
int main(){
  struct contador_t contador;
  contador.valor = 0;
  // - inicializamos el semáforo mutex de forma dinaḿica (en tiempo de ejecución)
  // - usamos el operador & porque la función pthread_mutex_init() requiere
  // de un "puntero a un tipo pthread_mutex_t"
  // (es lo mismo que decir que requiere la dirección de memoria de una variable de ese tipo)
  //pthread_mutex_init(&contador.mutex, NULL);

  // - utilizamos un vector estático de hilos
  // - recordemos que un vector estático, se le asigna espacio en memoria en tiempo de compilación
  // y este NO puede cambiar su tamaño en bytes, si se le asignó 4 Bytes tendrá eso, hasta que finalice el programa..
  pthread_t hilos[1];
  int estado_hilo = 0;

  estado_hilo = pthread_create(&hilos[0], NULL, rutina_incrementar, (struct contador_t *)&contador);

  if(estado_hilo){
    printf("Oh.. HAY ERROR!! el código de error es %d", estado_hilo);
    exit(-1); // syscall
  }

  pthread_join(hilos[0], NULL);
  pthread_mutex_destroy(&contador.mutex);
  int contador_incrementado = incrementar_contador(&contador);

  printf("main: incrementé el contador a %d niaka niaka niaka :)\n", contador_incrementado);

  return 0;
}


/*************************************************************/
/********** Definiciones de las funciones ********************/

static void *rutina_incrementar(void* arg){
  // podriamos pasarle a la funcion incrementar_contador() directamente el casteo
  // pero declaro y defino esta variable del tipo puntero a estructura,
  // para que se entienda mejor el printf
  struct contador_t* parametro = (struct contador_t*)arg;
  printf("hilo: incrementé el contador a %d\n", incrementar_contador(parametro));
  pthread_exit(NULL);
}

int incrementar_contador(struct contador_t* contador){
  pthread_mutex_t mutex = contador->mutex; // (*contador).mutex
   int valor = 0;

   pthread_mutex_lock(&mutex);
   valor = ++(contador->valor); // ++((*contador).valor);
   pthread_mutex_unlock(&mutex);
  return valor;
}
