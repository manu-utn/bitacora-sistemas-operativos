#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

// -------------------------------------------------------------------------------
// >
#define THREADS_MAX 4
#define CANT_TAREAS_MAX 50

typedef struct Tarea{
  int duracion;
  char tipo;
} tarea_t;

tarea_t COLA_TAREAS[CANT_TAREAS_MAX];
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
    // la rutina "iniciarTarea" tiene un pthread_cond_wait()
    // que le avisa que se quede esperando hasa que le soliciten ejecutarla
    // con solicitarTarea() que tiene un pthread_cond_signal()
    if(pthread_create(&th[i], NULL, &iniciarTarea, NULL) != 0){
      perror("Error al crear hilo");
    }
  }

  for(int i=0; i < CANT_TAREAS_MAX; i++){
    // manera alternativa de crear una estructura
    tarea_t tarea = {
      .duracion= rand() % 100 + 1, // números entre 1-100
      .tipo= 'A'+rand() % 26 // letras entre A-Z en mayuscula
    };
    // agregará la tarea a la cola de tareas,
    // y le avisará con pthread_cond_signal() a los hilos que se ejecuten
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
  // agregamos a la cola de tareas, y aumentamos la longitud de la cola
  pthread_mutex_lock(&MUTEX_COLA_TAREAS);
    COLA_TAREAS[CONTADOR_TAREAS] = tarea;
    CONTADOR_TAREAS++;
  pthread_mutex_unlock(&MUTEX_COLA_TAREAS);

  // avisamos al hilo que estaba esperando con pthread_cond_wait()
  // que ya hay tareas, y que ejecute instrucciones seguidas del wait()
  pthread_cond_signal(&CONDICION_COLA_TAREAS);
}

void* iniciarTarea(void* args){
  while(1){
    tarea_t tarea;

    // emulamos un tiempo de espera, como si estuviese iniciando la tarea..
    int tiempo_espera = rand() % 2;
    sleep(tiempo_espera);

    printf("iniciarTarea (%d)\n", CONTADOR_TAREAS);

    pthread_mutex_lock(&MUTEX_COLA_TAREAS);
      // mientras no haya tareas creadas, se queda esperando con wait()
      // es un loop infinito ...
      while(CONTADOR_TAREAS == 0){
        // MUTEX_COLA_TAREAS:
        // -> necesita de parámetro al mutex, porque..
        // -> 1. se bloqueó el mutex al ejecutar esta rutina
        // -> 2. el pthread_cond_wait() desbloquea el mutex, para que otro hilo
        // -> pueda acceder al recurso compartido
        //
        // Observación: Al final hay un pthread_mutex_unlock() pero.. al entrar a este
        // while infinito que se ejecuta hasta que no se cumpla la condición,
        // no se podría desbloquear
        //
        // pthread_cond_wait():
        // -> desbloquea el mutex
        // -> hasta que la condición de la rutina asociada al hilo no se cumpla..
        // -> then, no avanza a las siguientes instrucción debajo del wait()
        //
        // pthread_cond_signal():
        // -> es solicitarTarea quien manda la señal para avanzar

        pthread_cond_wait(&CONDICION_COLA_TAREAS, &MUTEX_COLA_TAREAS);
      }

      // agarramos la primera tarea, se la asignaremos al hilo (pero aún no lo va a ejecutar)
      tarea = COLA_TAREAS[0];

      for(int i=0; i < CONTADOR_TAREAS-1; i++){
        // movemos los elementos una posición a la izquierda
        // Ej. [i][1][2][3] -> [0][1][2]
        COLA_TAREAS[i] = COLA_TAREAS[i + 1];
      }

      // como sacamos la tarea de la cola
      CONTADOR_TAREAS--;
    pthread_mutex_unlock(&MUTEX_COLA_TAREAS);

    ejecutarTarea(&tarea);
  }
}

void ejecutarTarea(tarea_t* tarea){
  printf("duracion=%d minutos, tipo=%c (cantidad_tareas=%d)\n",
         tarea->duracion,
         tarea->tipo,
         CONTADOR_TAREAS);
}
