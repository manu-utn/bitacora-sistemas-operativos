#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

/*************************************************************/
/**** Declaraciones de funciones && variables globales ******/
void *rutina_para_hilo1();
void *rutina_para_hilo2();

// Declaramos el semáforo mutex
// - permite la "sincronización" entre hilos, que accedan a un recurso compartido en sincronía
// - permite la "mutua exclusión" en una sección crítica (sólo un acceso por vez)
// - permite un comportamiento determínistico (un resultado esperado)

// A) INICIALIZACION DE FORMA ESTATICA (en tiempo de compilación)
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//
// B) INICIALIZACION DE FORMA DINAMICA (en tiempo de ejecución)
// pthread_mutex_t mutex;
// pthread_mutex_init(&mutex, NULL); // <<< Esto iría dentro de main(){ ... }

// Definimos la sección crítica
// - En este caso una variable
// - Es una sección crítica porque será accedido para lectura/escritura por multiples hilos
// de forma concurrente
int valor = 0;


/*************************************************************/
/**************** Programa Principal  ************************/
int main(int argc, char** argv){
  // declaramos los hilos
  pthread_t hilo1, hilo2;

  if(pthread_create(&hilo1, NULL, rutina_para_hilo1, NULL) != 0){
    perror("NO se pudo crear el hilo");
    exit(1);
  }

  if(pthread_create(&hilo2, NULL, rutina_para_hilo2, NULL) != 0){
    perror("NO se pudo crear el hilo");
    exit(1);
  }

  pthread_join(hilo1, NULL);
  pthread_join(hilo2, NULL);

  printf("\nValor final: %d\n", valor);

  return 0;
}


/*************************************************************/
/********** Definiciones de las funciones ********************/
void *rutina_para_hilo1(){
  printf("\nHilo (1): Modifiqué valor a ", valor);
  for(int i=0; i < 20; i++){
    // rodeamos la sección crítica por el semáforo mutex
    // si descomentamos el mutex, veremos comportamientos NO determinísticos
    pthread_mutex_lock(&mutex);
    valor = valor + 1; // incrementará en 1
    pthread_mutex_unlock(&mutex);

    printf("%d, ", valor);
    fflush(stdout);
  }
  pthread_exit(NULL);
}

void *rutina_para_hilo2(){
  printf("\nHilo (2): Modifiqué valor a ", valor);
  for(int i=0; i < 20; i++){
    // rodeamos la sección crítica por el semáforo mutex
    // si descomentamos el mutex, veremos comportamientos NO determinísticos
    pthread_mutex_lock(&mutex);
    valor = valor - 1; // decrementará en 1
    pthread_mutex_unlock(&mutex);

    printf("%d, ", valor);
    fflush(stdout);
  }
  pthread_exit(NULL);
}
