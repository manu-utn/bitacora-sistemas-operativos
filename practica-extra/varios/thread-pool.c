#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

// -------------------------------------------------------------------------------
// >
typedef struct Tarea{
  int x, y;
} tarea_t;

tarea_t COLA_TAREAS[100];
int CONTADOR_TAREAS = 0;

void ejecutarTarea(tarea_t*);

// -------------------------------------------------------------------------------
// >
int main(int argc, char* argv[]){
  tarea_t tarea_1 = {a=5, b=10};

  ejecutarTarea(&tarea_1);

  return 0;
}

// -------------------------------------------------------------------------------
// >
void ejecutarTarea(tarea_t* tarea){
  int resultado = tarea->x + tarea->y;

  printf("%d + %d = %d\n", tarea->x, tarea->y, resultado);
}
