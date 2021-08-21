#include <stdio.h>
#include <stdlib.h> // malloc(), rand()
#include <unistd.h> // sleep()
#include <pthread.h>
#include <semaphore.h>

#define THREADS_MAX 4

sem_t sem;

void* imprimir(void* args){
  sem_wait(&sem); // <-- conceptualmente hace un sem--;
  sleep(1);
  printf("Hola..! Soy el hilo (%d)\n", *(int*)args);
  sem_post(&sem); // <-- conceptualmente hace un sem++;

  free(args);

  pthread_exit(NULL);
}

int main(int argc, char* argv[]){
  pthread_t th[THREADS_MAX];

  // - inicializamos el semáforo
  // - 1er parámetro: se creará el semáforo y se guardará en esa dirección de memoria
  // - 2do parámetro: ...?
  // - 3er parámetro: al tener un valor > 1, lo convierte como en un "semáforo contador"
  //
  // Como el 3er parámetro es 2, y como definimos que tendremos 4 hilos max.
  // 1. dos hilos cualquieras ejecutarán la rutina "imprimir"
  // 2. ejecutarán sem_wait(&sem) por tanto decrementarán a sem en 1, similar a sem--
  // 3. harán el printf()
  // 4. ejecutarán sem_post(&sem) por tanto incrementarán a sem en 1, similar a sem++
  //
  // 5. los otros 2 hilos restantes, deben esperar que los dos hilos anteriores ejecuten su sem_post(&sem)
  // para poder ejecutarse.. Porque cuando se cumple que (sem == 0), el semáforo se queda "esperando"
  // hasta que valga >= 1
  //
  // 6. se repite el paso (1),(2),(3),(4) con los hilos mencionados en el paso (5)
  //
  // 7. finaliza el hilo principal main, porque ya no hay más hilos por ejecutar
  sem_init(&sem,0, 2);

  for(int i=0; i < THREADS_MAX; i++){
    int *num = malloc(sizeof(int)); // <--- necesario, porque muchos hilos accederan a este recurso
    *num = i;

    if(pthread_create(&th[i], NULL, &imprimir, num) != 0){
      perror("Falló al crear un hilo");
    }
    printf("Creando el hilo (%d)..\n", i);
  }

  for(int i=0; i < THREADS_MAX; i++){
    if(pthread_join(th[i], NULL) != 0){
      perror("Falló al hacer join de un hilo");
    }
  }

  sem_destroy(&sem);

  return 0;
}
