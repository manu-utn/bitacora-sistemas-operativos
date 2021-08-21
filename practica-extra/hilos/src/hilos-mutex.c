#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // sleep()

#define THREADS_MAX 8

pthread_mutex_t MUTEX_ALUMNOS;
pthread_mutex_t MUTEX_MATERIAS;

int ALUMNOS = 0;
int MATERIAS = 0;

void* incrementar(){
  while(1){

    // - Grave ERROR..! ESTO PUEDE PRODUCIR UN "DEADLOCK" ó "INTERBLOQUEO" ó "PUNTO MUERTO"..!
    // - El orden de los lock importante, también puede suceder si en vez del condicional
    // estuviese en otra rutina asociada a otro hilo
    // - Un hilo puede bloquea el recurso que el otro necesita, y viceversa
    //
    // Supongamos que dos hilos ejecutan el mismo recurso "casi" al mismo tiempo
    // la 2da vez que se bloquee el mismo recurso, ocurrirá el famoso "deadlock"
    //
    // hilo_1:
    // -> 1. lock(alumnos)
    // -> 2. lock(materias) <- bloqueó el recurso
    //
    // hilo_2:
    // -> 1. lock(materias) <- se bloqueó el recurso (por 2da vez)
    // -> 2. lock(alumnos)

    if(MATERIAS % 3 == 0){
      printf("a) se bloquea alumnos\n");
      pthread_mutex_lock(&MUTEX_ALUMNOS); // <--- esto puede producir el 'DEADLOCK', debería estar debajo..
      sleep(1);
      printf("a) se bloquea materias\n");
      pthread_mutex_lock(&MUTEX_MATERIAS);
    }else{
      printf("b) se bloquea materias\n");
      pthread_mutex_lock(&MUTEX_MATERIAS);
      sleep(1);
      printf("b) se bloquea alumnos\n");
      pthread_mutex_lock(&MUTEX_ALUMNOS);
    }

    ALUMNOS += 900;
    MATERIAS += 5;

    printf("Hay %d alumnos y %d materias\n", ALUMNOS, MATERIAS);
    printf("se desbloquea alumnos\n");
    pthread_mutex_unlock(&MUTEX_ALUMNOS);
    printf("se desbloquea materias\n");
    pthread_mutex_unlock(&MUTEX_MATERIAS);

  }

  pthread_exit(NULL);
}

int main(int argc, char* argv[]){
  pthread_t th[THREADS_MAX];

  pthread_mutex_init(&MUTEX_ALUMNOS, NULL);
  pthread_mutex_init(&MUTEX_MATERIAS, NULL);

  for(int i=0; i < THREADS_MAX; i++){
    if(pthread_create(&th[i], NULL, incrementar, NULL) != 0){
      perror("Error al crear un hilo");
    }
  }

  for(int i=0; i < THREADS_MAX; i++){
    if(pthread_join(th[i], NULL) != 0){
      perror("Error al crear un hilo");
    }
  }

  pthread_mutex_destroy(&MUTEX_ALUMNOS);
  pthread_mutex_destroy(&MUTEX_MATERIAS);

  return 0;
}
