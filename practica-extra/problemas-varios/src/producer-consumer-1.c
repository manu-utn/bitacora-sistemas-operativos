#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// -------------------------------------------------------------------------------
#define THREADS_MAX 4
#define RECURSOS_MAX 300

int RECURSOS = 0;

pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t LLENO = PTHREAD_COND_INITIALIZER;
pthread_cond_t VACIO = PTHREAD_COND_INITIALIZER;

void* consumer(void*);
void* producer(void*);

// -------------------------------------------------------------------------------
int main(int argc, char* argv[]){
  pthread_t th[THREADS_MAX];

  for(int i=0; i < THREADS_MAX; i++){
    if(i > 0){
      if(pthread_create(&th[i], NULL, &consumer, NULL) != 0){
        perror("Error al crear hilo");
      }
    }
    else{
      if(pthread_create(&th[i], NULL, &producer, NULL) != 0){
        perror("Error al crear hilo");
      }
    }
  }

  for(int i=0; i < THREADS_MAX; i++){
    if(pthread_join(th[i], NULL) != 0){
      perror("Error al hacer join de un hilo");
    }
  }

  return 0;
}


// -------------------------------------------------------------------------------

/*
 * Nota (1):
 * -> 1. loop infinito, los hilos asociadas a esta rutina
 * -> 2. se estarán ejecutando forever
 *
 * Nota (2):
 * -> 1. mientras no se cumpla la condición de corte, no sale del loop
 * -> 2. va a seguir iterando y preguntando (es más seguro que un IF)
 * -> 3. se va a quedar en un loop infinito, pero sólo cuando llegó al tope
 *    de recursos generados y no puede cargar más
 *
 * Nota (3):
 * -> 1. ponemos a esperar/dormir al hilo asociado a esta rutina "producer"
 * -> 2. por cada wait() debe haber un signal() con la misma condición asociada
 * -> 3. si en otra parte del fuente no se hace un signal() => el hilo se queda esperando,
 *    y... el programa principal main nunca finalizará
 *
 * Nota (4):
 * -> 1. avisamos que agregamos recursos
 * -> 2. despierta/activa a los hilos asociados con wait() a esta condición (&LLENO)
 */
void* producer(void* args){
  while(1){ // -> (1)
    pthread_mutex_lock(&MUTEX);

    while(RECURSOS == RECURSOS_MAX){ // -> (2)
      pthread_cond_wait(&VACIO, &MUTEX); // -> (3)
    }

    RECURSOS++; // -> creamos recursos
    pthread_cond_signal(&LLENO); // -> (4)
    pthread_mutex_unlock(&MUTEX);

    printf("Producer (RECURSOS: %d)\n", RECURSOS);
  }
}

/*
 * Nota (1):
 * -> loop infinito, los hilos asociadas a esta rutina se estarán ejecutando forever
 *
 * Nota (2):
 * -> 1. mientras no se cumpla la condición de corte, no sale de loop
 * -> 2. va a seguir iterando... y chequeando si se cumplió o no la condición
 * -> 3. se va a quedar en el loop infinito, pero sólo cuando NO haya recursos que consumir
 *
 * Nota (3):
 * -> 1. ponemos a dormir/esperar al hilo asociado a la rutina "consumer"
 * -> 2. mismo concepto que en producer
 *
 * Nota (4):
 * -> 1. avisamos que sacamos recursos
 * -> 2. despierta/activa a los hilos asociados con wait() a esta condición (&VACIO)
 * -> 3. va a despertar a los hilos que tengan asociados a "producer"
 */
void* consumer(void* args){
  while(1){ // -> (1)
    sleep(1);
    pthread_mutex_lock(&MUTEX);

    while(RECURSOS == 0){ // -> (2)
      pthread_cond_wait(&LLENO, &MUTEX); // -> (3)
    }

    RECURSOS--; // usamos los recursos creados por "consumer"

    pthread_cond_signal(&VACIO); // -> (4)
    pthread_mutex_unlock(&MUTEX);

    printf("Consumer (RECURSOS: %d)\n", RECURSOS);
  }
}

