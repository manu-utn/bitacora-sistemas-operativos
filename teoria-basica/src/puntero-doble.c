#include <stdio.h>
#include <stdlib.h>

#define MAX_TRIPULANTES 3

typedef struct{
  int tid;
} t_tripulante;

typedef struct{
  t_tripulante *tripulante;
} t_tripulante_con_tarea;

t_tripulante_con_tarea *tripulantes_con_tarea = NULL;
t_tripulante *tripulantes = NULL;

t_tripulante *crearTripulante(int tid){
  t_tripulante *tripulante = malloc(sizeof(t_tripulante));
  tripulante->tid = tid;

  return tripulante;
}

int main(){
  tripulantes = malloc(sizeof(t_tripulante) * MAX_TRIPULANTES);
  tripulantes_con_tarea = malloc(sizeof(t_tripulante_con_tarea) * MAX_TRIPULANTES);

  /* for(int i=0; i < MAX_TRIPULANTES; i++) */
  /*   tripulantes[i].tid = i; */
  /* for(int i=0; i < MAX_TRIPULANTES; i++) */
  /*   printf("tripulante tid=%d\n", tripulantes[i].tid); */
  /* free(tripulantes); */

  for(int i=0; i < MAX_TRIPULANTES; i++){
    tripulantes_con_tarea[i].tripulante = crearTripulante(i);
  }

  for(int i=0; i < MAX_TRIPULANTES; i++){
    printf("tripulante con tarea tid=%d\n", tripulantes_con_tarea[i].tripulante->tid);

    free(tripulantes_con_tarea[i].tripulante);
  }

  free(tripulantes_con_tarea);

  return 0;
}
