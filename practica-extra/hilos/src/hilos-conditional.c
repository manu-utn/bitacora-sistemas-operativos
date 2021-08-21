#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

// -------------------------------------------------------------------------------
//
pthread_mutex_t mutex_nota;
pthread_cond_t cond_nota;

int nota = 0;

void *corregirParcial(void *arg);
void *molestarAlProfesor(void *arg);


// -------------------------------------------------------------------------------
// > MAIN
int main(int agc, char* argv[]){
  // declaramos un vector de hasta hilos
  pthread_t th[5];

  // inicializamos el mutex
  pthread_mutex_init(&mutex_nota, NULL);
  // inicializamos la condición
  pthread_cond_init(&cond_nota, NULL);

  if(pthread_create(&th[0], NULL, &corregirParcial, NULL) != 0){
    perror("Error al crear el hilo 1..!");
  }

  for(int i=1; i < 5; i++){
    if(pthread_create(&th[i], NULL, &molestarAlProfesor, NULL) != 0){
      perror("Error al crear el hilo 2..!");
    }
  }

  for(int i=0; i < 2; i++){
    if(pthread_join(th[i], NULL) != 0){
      perror("Error al esperar los hilos ?");
    }
  }

  // no es tan.. importante porque funciona sin esto, pero bueno (?)
  pthread_mutex_destroy(&mutex_nota);
  pthread_cond_destroy(&cond_nota);

  return 0;
}


// -------------------------------------------------------------------------------
//
void *corregirParcial(void *arg){
  for(int i=0; i < 5; i++){
    pthread_mutex_lock(&mutex_nota);
    nota += 2;
    printf("Profesor: Corrigiendo parcial.. (nota=%d)\n", nota);
    pthread_mutex_unlock(&mutex_nota);

    // despierta sólo a un hilo que esperaba esta condición
    // pthread_cond_signal(&cond_nota);

    // despierta a todos los hilos que esperan esta condición
    pthread_cond_broadcast(&cond_nota);

    sleep(1);
  }

  pthread_exit(NULL);
}

void *molestarAlProfesor(void *arg){
  pthread_mutex_lock(&mutex_nota);

  // el hilo se queda bloqueado hasta que se cumpla la condición
  while(nota < 2){
    printf("Alumno: molesta al profesor por tener baja nota :F\n");
    // mientras se cumpla la condición (nota < 2)
    // el cond_wait hará que se duerma el 'hilo' que tiene asociada esta a rutina 'molestarAlProfesor'
    pthread_cond_wait(&cond_nota, &mutex_nota);
  }

  nota -= 1;
  printf("Profesor: corrigiendo parcial muy enojado >:[ (nota=%d, hilo_id=%d)\n", nota, pthread_self());
  pthread_mutex_unlock(&mutex_nota);

  pthread_exit(NULL);
}
