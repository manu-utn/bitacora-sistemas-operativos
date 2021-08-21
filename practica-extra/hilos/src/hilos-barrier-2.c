#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS_MAX 10

int numeros[10] = {1,3,4,7,9,10,12,19,25,50};

void* imprimir(void* args){
  int indice = *(int*) args;
  printf("%d ", indice);

  free(args);
}

int main(int argc, char* argv[]){
  pthread_t th[THREADS_MAX];

  for(int i=0; i < THREADS_MAX; i++){
    int* num = malloc(sizeof(int));
    *num = i;

    // - Si a la rutina del hilo le pasamos por parámetro la dirección de memoria
    // de la variable local i es decir &i tendremos problemas de sincronización,
    // varios hilos tratarán de modificar
    //
    // - En cambio si le pasamos por parámetro un puntero a entero, y reservamos
    // memoria para un entero podríamos solucionarlo temporalmente
    //
    // if(pthread_create(&th[i], NULL, &imprimir, &i) != 0){ // <-- problemas
    if(pthread_create(&th[i], NULL, &imprimir, num) != 0){
      perror("Error al crear hilo");
    }
  }

  for(int i=0; i < THREADS_MAX; i++){
    if(pthread_join(th[i], NULL) != 0){
      perror("Error al hacer join de un hilo");
    }
  }

  return 0;
}
