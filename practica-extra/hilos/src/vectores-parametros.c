#include <stdio.h> // biblioteca de standard input/output
#include <pthread.h>
#include <stdlib.h> // para la función rand()

void *rutina_para_hilo1(void *);
void *rutina_para_hilo2(void *);
void *rutina_para_hilo3(void *);

int main(int argc, char** argv){
  // declaramos un vector de 3 hilos
  pthread_t h[3];
  // definimos las rutinas/tareas/funciones/trabajos que tendrá hilo
  void* hilos[3] = {rutina_para_hilo1, rutina_para_hilo2, rutina_para_hilo3};

  // usamos una estructura del tipo estática
  // para evitar problemas con los hilos
  // (es decir un arreglo de tamaño fijo, es estático ya que aloca memoria en el stack)
  int parametros[3] = {2,3,5};

  for(int i=0; i<3; i++){
    // creamos cada hilo
    // le asignamos la rutina/trabajo/tarea/función que debe ejecutar cada uno
    // y le pasamos un parámetro a esas rutinas
    pthread_create(&h[i], NULL, hilos[i], &parametros[i]);
  }
  for(int i=0; i<3; i++){
    pthread_join(h[i], NULL);
  }

  printf("Fin del programa..!\n");

  return 0;
}

void *rutina_para_hilo1(void *pCant){
  int cantidad = *((int*) pCant);

  for(int i=0; i < cantidad; i++){
    printf("hilo 1: %d\n", rand());
  }

  pthread_exit(NULL);
}

void *rutina_para_hilo2(void *pCant){
  int cantidad = *((int*) pCant);

  for(int i=0; i < cantidad; i++){
    printf("hilo 2: %d\n", rand());
  }

  pthread_exit(NULL);
}

void *rutina_para_hilo3(void *pCant){
  int cantidad = *((int*) pCant);

  for(int i=0; i < cantidad; i++){
    printf("hilo 3: %d\n", rand());
  }

  pthread_exit(NULL);
}
