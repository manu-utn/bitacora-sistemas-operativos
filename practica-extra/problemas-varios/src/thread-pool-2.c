#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include <pthread.h>
#include <semaphore.h>

// -------------------------------------------------------------------------------
// >
#define THREADS_MAX 4
#define CANT_TAREAS_MAX 50
#define CANT_PERSONAS_MAX 5

typedef struct Tarea{
  void (*tareaFuncion)(int, char, char[50]);
  int duracion;
  char prioridad;
  char persona_asignada[50];
} tarea_t;

char PERSONAS_NOMBRES[CANT_PERSONAS_MAX][50] = {"carlos", "pedrito", "samuel", "ramirez", "perez"};

int CONTADOR_TAREAS = 0;

tarea_t COLA_TAREAS[CANT_TAREAS_MAX];
pthread_cond_t CONDICION_COLA_TAREAS;

pthread_mutex_t MUTEX_COLA_TAREAS;
pthread_cond_t CONDICION_COLA_TAREAS;

void solicitarTarea(tarea_t);
void ejecutarTarea(tarea_t*);
void* iniciarTarea(void*);

void estudiar(int, char, char[50]);
void jugar(int, char, char[50]);

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


  for(int i=0; i < CANT_TAREAS_MAX; i++){
    int codigo_persona = rand() % CANT_PERSONAS_MAX;

    tarea_t tarea = {
      .tareaFuncion = (i % 2 == 0) ? &estudiar : &jugar,
      .duracion = rand() % 100 + 1,
      .prioridad = 'A' + rand() % 26
    };

    strcpy(tarea.persona_asignada, PERSONAS_NOMBRES[codigo_persona]);

    solicitarTarea(tarea);
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

void* iniciarTarea(void* args){
  while(1){
    tarea_t tarea;

    pthread_mutex_lock(&MUTEX_COLA_TAREAS);
      while(CONTADOR_TAREAS == 0){
        pthread_cond_wait(&CONDICION_COLA_TAREAS, &MUTEX_COLA_TAREAS);
      }

      tarea = COLA_TAREAS[0];

      for(int i=0; i < CONTADOR_TAREAS-1; i++){
        COLA_TAREAS[i] = COLA_TAREAS[i+1]; // [i][1][2][3] -> [0][1][2]
      }

      CONTADOR_TAREAS--;
    pthread_mutex_unlock(&MUTEX_COLA_TAREAS);

    ejecutarTarea(&tarea);
  }
}

void solicitarTarea(tarea_t tarea){
  pthread_mutex_lock(&MUTEX_COLA_TAREAS);
    COLA_TAREAS[CONTADOR_TAREAS] = tarea;
    CONTADOR_TAREAS++;
  pthread_mutex_unlock(&MUTEX_COLA_TAREAS);

  pthread_cond_signal(&CONDICION_COLA_TAREAS);
}

void ejecutarTarea(tarea_t* tarea){
  tarea->tareaFuncion(tarea->duracion, tarea->prioridad, tarea->persona_asignada);
}

void estudiar(int duracion, char prioridad, char persona[50]){
  printf("%s estudiará por %d minutos, su prioridad es %c\n",
         persona,
         duracion,
         prioridad);
}

void jugar(int duracion, char prioridad, char persona[50]){
  printf("%s jugará por %d minutos, su prioridad es %c\n",
         persona,
         duracion,
         prioridad);
}
