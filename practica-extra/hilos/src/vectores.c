#include <stdio.h>
#include <pthread.h> // FUNDAMENTAL..! :)
#include <stdlib.h> // la usamos por la funcion rand()

void *rutina_para_hilo1();
void *rutina_para_hilo2();
void *rutina_para_hilo3();

int main(int argc, char** argv){
  // Creamos un vector de hilos, con capacidad hasta 4 hilos
  // - Si hubiesemos usado vector, tendriamos que haberlos declarado así: pthread_t h1,h2,h3;
  pthread_t h[3];
  // Creamos el vector de las funciones que ejecutará cada hilo
  // - El tipo del vector debe ser void, porque es el tipo que espera la función pthread_create()
  void* hilos[3] = {rutina_para_hilo1, rutina_para_hilo2, rutina_para_hilo3};

  for(int i=0; i < 3; i++){
    printf("Creamos el hilo (%d)\n", i+1);
    // Los argumentos que espera pthread_create(4) son:
    // 1. la referencia al hilo del tipo "pthread_t"
    // 2. los argumentos que usará (si le pasamos NULL, usará los que vienen por defecto)
    // 3. las función/rutina que ejecutará el hilo
    // 4. los argumentos que queramos pasarle a esa función/rutina que ejecutará
    pthread_create(&h[i], NULL, hilos[i], NULL);
  }
  // Recordemos que es el "planificador" quien decidirá el orden de ejecución
  // puede que a veces "parezca" que es h1,h2,h3 pero sería pura casualidad
  for(int i=0; i < 3; i++){
    printf("Esperamos el fin de ejecución del hilo (%d)\n", i+1);
    // El pthread_join(2) permite darle suficientes "ráfagas de CPU" al hilo
    // para que se termine de ejecutar, antes de finalizar el programa principal "main"
    // (si no lo hacemos, es posible que termine main tan rápido, que los hilos no lleguen a ejecutar)
    pthread_join(h[i], NULL);
  }

  printf("Fin del programa..!\n");
  return 0;
}

void *rutina_para_hilo1(){
  for(int i=0; i < 3; i++){
    printf("hilo 1: %d\n", rand());
  }

  pthread_exit(NULL);
}

void *rutina_para_hilo2(){
  for(int i=0; i < 4; i++){
    printf("hilo 2: %d\n", rand());
  }

  pthread_exit(NULL);
}

void *rutina_para_hilo3(){
  for(int i=0; i < 3; i++){
    printf("hilo 3: %d\n", rand());
  }

  pthread_exit(NULL);
}
