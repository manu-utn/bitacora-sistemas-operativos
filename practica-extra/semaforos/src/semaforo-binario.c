#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

// -------------------------------------------------------------------------------
// >
#define THREADS_MAX 1

sem_t sem;
pthread_mutex_t mutex;

int *numero;

void* incrementar(void* args);

// -------------------------------------------------------------------------------
// >
int main(int argc, char* argv[]){
  pthread_t th[THREADS_MAX];

  numero = malloc(sizeof(int));
  *numero = 300;

  pthread_mutex_init(&mutex, NULL);
  sem_init(&sem, 0, 0);

  for(int i=0; i < THREADS_MAX; i++){
    if(pthread_create(&th[i], NULL, &incrementar, NULL) != 0){
      perror("Error al crear hilo!");
    }
  }

  sem_wait(&sem);
  printf("2) Liberamos espacio en memoria\n");
  free(numero);

  // forzamos al hilo principal "main", que espere
  // hasta que el resto de los hilos terminen de ejecutar su rutina
  for(int i=0; i < THREADS_MAX; i++){
    if(pthread_join(th[i], NULL) != 0){
      perror("Error al hacer join de hilo!");
    }
  }


  pthread_mutex_destroy(&mutex);
  sem_destroy(&sem);

  return 0;
}

// -------------------------------------------------------------------------------
// >

void* incrementar(void* args){
  //while(1){
  for(int i=0; i < 50; i++){
    pthread_mutex_lock(&mutex);
    *numero += 1;
    printf("1) incrementamos el número a %d\n", *numero);
    pthread_mutex_unlock(&mutex);

    sem_post(&sem);
  }

  pthread_exit(NULL);
}
