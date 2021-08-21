#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

// -------------------------------------------------------------------------------
// >
#define THREADS_NUM 3

void *rutina(void* args);

// -------------------------------------------------------------------------------
// >
int main(int argc, char* argv[]){
  pthread_t th[THREADS_NUM];

  int i;
  for(int i=0; i < THREADS_NUM; i++){
    if(pthread_create(&th[i], NULL, &rutina, NULL) != 0){
      perror("Error al crear hilo!");
    }

    // con esto separamos los hilos, del hilo principal 'main'
    pthread_detach(th[i]);
  }

  for(int i=0; i < THREADS_NUM; i++){
    // esto va a fallar.. porque
    // al hacer 'detach' ya los separamos del hilo principal main
    // ya NO están asociados, se perdió la relación
    if(pthread_join(th[i], NULL) != 0){
      perror("Error al hacer join de un hilo");
    }
  }

  // - si no hacemos esto, el hilo principal main, puede finalizar antes
  // - esperamos a que terminen los hilos 'detached'
  pthread_exit(0);

  // - si hacemos esto, los hilos 'detached' pueden quizás no ejecutarse a tiempo
  // return 0;
}

// -------------------------------------------------------------------------------
// >

void *rutina(void* args){
  sleep(1);
  printf("Rutina de hilo finalizado\n");

  pthread_exit(NULL);
}
