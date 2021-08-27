#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

// -------------------------------------------------------------------------------
// >
#define THREADS_MAX 4

typedef struct Tarea{
  int x, y;
} tarea_t;

tarea_t COLA_TAREAS[100];
int CONTADOR_TAREAS = 0;

pthread_mutex_t MUTEX_COLA_TAREAS;
pthread_cond_t CONDICION_COLA_TAREAS;

void solicitarTarea(tarea_t);
void ejecutarTarea(tarea_t*);
void* iniciarTarea(void*);

// -------------------------------------------------------------------------------
// >
int main(int argc, char* argv[]){
  srand(time(NULL));

  pthread_t th[THREADS_MAX];

  pthread_mutex_init(&MUTEX_COLA_TAREAS, NULL);
  pthread_cond_init(&CONDICION_COLA_TAREAS, NULL);

  for(int i=0; i < THREADS_MAX; i++){
    if(pthread_create(&th[i], NULL, &iniciarTarea, NULL) != 0){
      perror("Error al crear hilo");
    }
  }

  for(int i=0; i < 50; i++){
    // manera alternativa de crear una estructura
    tarea_t tarea = {
      .x= rand() % 100,
      .y= rand() % 100
    };
    solicitarTarea(tarea);

    // manera común de crear una estructura
    // tarea_t tarea_2;
    // tarea_2.x=6;
    // tarea_2.y=9;
  }


  for(int i=0; i < THREADS_MAX; i++){
    if(pthread_join(th[i], NULL) != 0){
      perror("Error al hacer join de hilo");
    }
  }

  return 0;
}

// -------------------------------------------------------------------------------
// >
void solicitarTarea(tarea_t tarea){
  pthread_mutex_lock(&MUTEX_COLA_TAREAS);
  COLA_TAREAS[CONTADOR_TAREAS] = tarea;
  CONTADOR_TAREAS++;
  pthread_mutex_unlock(&MUTEX_COLA_TAREAS);

  pthread_cond_signal(&CONDICION_COLA_TAREAS);
}

void* iniciarTarea(void* args){
  while(1){
    tarea_t tarea;

    sleep(1);
    pthread_mutex_lock(&MUTEX_COLA_TAREAS);
    while(CONTADOR_TAREAS == 0){
      pthread_cond_wait(&CONDICION_COLA_TAREAS, &MUTEX_COLA_TAREAS);
    }

    tarea = COLA_TAREAS[0];
    for(int i=0; i < CONTADOR_TAREAS-1; i++){
      COLA_TAREAS[i] = COLA_TAREAS[i + 1];
    }

    CONTADOR_TAREAS--;
    pthread_mutex_unlock(&MUTEX_COLA_TAREAS);

    ejecutarTarea(&tarea);
  }
}

void ejecutarTarea(tarea_t* tarea){
  int resultado = tarea->x + tarea->y;

  printf("%d + %d = %d\n", tarea->x, tarea->y, resultado);
}
