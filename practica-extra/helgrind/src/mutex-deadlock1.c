// gcc mutex-deadlock.c -pthread -g -Wall && valgrind --tool=helgrind ./a.out
#include <stdio.h>
#include <pthread.h>

// - inicializamos dos semáforos mutex
// (de forma estática, en tiempo de compilación)
static pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

static void *rutina_1(void *arg){
  int *valor = (int *)arg;
  /* printf("soy un hilo, y recibo un %d\n", *valor); */

  pthread_mutex_lock(&m1);
  pthread_mutex_lock(&m2);

  pthread_mutex_unlock(&m1);
  pthread_mutex_unlock(&m2);
  pthread_exit(NULL);
  //return 0;
}

static void *rutina_2(void *arg){
   int *valor = (int *)arg;
   /* printf("soy un hilo, y recibo un %d\n", *valor); */

  pthread_mutex_lock(&m2);
  pthread_mutex_lock(&m1);

  pthread_mutex_unlock(&m1);
  pthread_mutex_unlock(&m2);

  pthread_exit(NULL);
  //return 0;
}

int main(int argc, char *argv[]){
  pthread_t hilo_1, hilo_2;
  int param1 = 1, param2= 2;

  pthread_mutex_lock(&m1);
  pthread_mutex_unlock(&m1);

  pthread_create(&hilo_1, NULL, rutina_1, &param1);
  pthread_create(&hilo_2, NULL, rutina_2, &param2);

  pthread_join(hilo_1, NULL);
  pthread_join(hilo_2, NULL);

  return 0;
}
