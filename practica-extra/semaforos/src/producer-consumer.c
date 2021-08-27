#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

// -------------------------------------------------------------------------------
// >
#define THREADS_MAX 4
#define BUFFER_TAMANIO 10

sem_t BUFFER_COMPLETO; // semáforo binario
sem_t BUFFER_VACIO;  // semáforo contador

pthread_mutex_t MUTEX_BUFFER;

int BUFFER[BUFFER_TAMANIO];
int contador = 0;

void* consumidor(void* args);
void* productor(void* args);

// -------------------------------------------------------------------------------
// >
int main(int argc, char* argv[]){
  srand(time(NULL));

  pthread_t th[THREADS_MAX];

  pthread_mutex_init(&MUTEX_BUFFER, NULL);
  // inicializamos semáforo contador
  // -> 1er parámetro: dirección de memoria donde se creará
  // -> 2do parámetro:
  // -> 3er parámetro: Máx. cant. de recursos que pueden acceder (al ser N, será contador)
  sem_init(&BUFFER_VACIO, 0, BUFFER_TAMANIO);
  // inicializamos semáforo binario
  // -> 1er y 2do parámetro: idem
  // -> 3er parámetro: al tener valor 0 ó 1, actúa como Bemáforo Binario
  sem_init(&BUFFER_COMPLETO, 0, 0);

  for(int i=0; i < THREADS_MAX; i++){
    int *numero = malloc(sizeof(int));
    *numero = i;
    // alternamos la cant. de hilos consumidores-productores
    if(i % 2 == 0){
      if(pthread_create(&th[i], NULL, &consumidor, numero) != 0){
        perror("error..!");
      }
    }
    else{
      if(pthread_create(&th[i], NULL, &productor, NULL) != 0){
        perror("error..!");
      }
    }
  }

  for(int i=0; i < THREADS_MAX; i++){
    if(pthread_join(th[i], NULL) != 0){
      perror("error");
    }
  }

  return 0;
}

// -------------------------------------------------------------------------------
// >
void* consumidor(void* args){
  int numero = *(int*)args;
  int valor;
  int tiempo_de_procesamiento;

  while(1){
    // 1)
    // si (sem <= 0) => se queda esperando que se llene el buffer
    // si (sem > 0) => hace sem-- y avanza a la sig. instrucción debajo del wait()
    sem_wait(&BUFFER_COMPLETO);

    // 2) bloquea el recurso y lo usa
    pthread_mutex_lock(&MUTEX_BUFFER);
      valor = BUFFER[contador];
      contador--;
      tiempo_de_procesamiento = rand() % 2 + 1;
      /* sleep(tiempo_de_procesamiento); */
    pthread_mutex_unlock(&MUTEX_BUFFER);

    // 3. avisa que usó el buffer, y solicita al "productor" lo llene
    sem_post(&BUFFER_VACIO);
    printf("Consumidor -> numero=%d, tid=%d, valor=%d\n", numero, pthread_self(), valor);
  }

  // liberamos memoria del puntero cuando termine el loop
  free(args);
  // por ser un puntero a NULL
  pthread_exit(NULL);
}

void* productor(void* args){
  int valor = rand() % 10 + 1;
  int tiempo_de_calculo = rand() % 2 + 1;
  printf("productor: Generando contenido..\n");
  /* sleep(tiempo_de_calculo); */

  while(1){
    // si (sem <= 0) => se queda esperando que se vacíe el buffer
    // si (sem > 0)  => hace sem-- y avanza a la sig. instrucción debajo del wait()
    sem_wait(&BUFFER_VACIO);

    // 2. bloquea el acceso al recurso y llena el buffer
    // -> si llegó hasta acá es porque (sem > 0)
    pthread_mutex_lock(&MUTEX_BUFFER);
      BUFFER[contador] = valor;
      contador++;
    pthread_mutex_unlock(&MUTEX_BUFFER);

    // 3. avisa que volvió a llenar el buffer, para que el "consumidor" lo use
    // -> hace sem++
    sem_post(&BUFFER_COMPLETO);
  }

  // por ser un puntero a NULL
  pthread_exit(NULL);
}
