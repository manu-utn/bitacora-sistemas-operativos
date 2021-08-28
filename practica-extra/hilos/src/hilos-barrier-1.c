#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// -------------------------------------------------------------------------------
// >
#define THREADS_MAX 3

pthread_barrier_t barrier;

void* controlarSalon(void* args);


// -------------------------------------------------------------------------------
// > MAIN
int main(int argc, char* argv[]){

  pthread_t th[THREADS_MAX];
  int i;

  // definimos como condición un número de hilos,
  // actúa como un "semaforo contador"
  pthread_barrier_init(&barrier, NULL, 3);

  for(i =0; i < THREADS_MAX; i++){
    if(pthread_create(&th[i], NULL, &controlarSalon, NULL) != 0){
      perror("Error al crear un hilo");
    }

    sleep(1);
  }

  // Forzamos que el hilo principal 'main' espere
  // hasta que el resto de los hilos terminen
  for(i =0; i < THREADS_MAX; i++){
    if(pthread_join(th[i], NULL) != 0){
      perror("Error al hacer join de un hilo");
    }
  }

  pthread_barrier_destroy(&barrier);

  return 0;
}


// -------------------------------------------------------------------------------
// >
void* controlarSalon(void* args){
  //while(1){
  printf("Esperando a que se junten 3 invitados (hilos)\n");
  sleep(1);
  // "conceptualmente" por cada wait() hará un decremento en el valor del 'barrier'
  // cuando el valor sea 0 lo dejará pasar
  // la rutina de cada hilo, se quedará esperando.. hasta se cumpla el valor inicial del barrier
  pthread_barrier_wait(&barrier);
  int hilo_id = pthread_self();

  printf("Permitiendo que ingrese un invitado (hilo_id=%d)\n", hilo_id);
  sleep(1);
  //}

  pthread_exit(NULL);
}
