#include <stdio.h>
#include <stdlib.h> // malloc(), rand()
#include <pthread.h>
#include <time.h> // time()
#include <unistd.h> // sleep()

// -------------------------------------------------------------------------------
//

// creamos un arreglo de hasta 5 strings (cada uno de max. longitud 20)
char invitados[5][20] = {"Carlos", "Pedro", "Ramirez", "Maria", "Samuel"};

void* generarNumeroRandom();
void* elegirInvitado(void *arg);

// -------------------------------------------------------------------------------
// MAIN
int main(int argc, char* argv[]){
  // generamos la semilla
  srand(time(NULL));

  int* numero_ganador;

  pthread_t th_1, th_2;

  printf("Generando número random..\n");

  // - creamos el hilo "th_1" y le asociamos la rutina "generarNumeroRandom"
  if(pthread_create(&th_1, NULL, &generarNumeroRandom, NULL) != 0){
    perror("error al crear hilo (1)");
  }
  // - guardamos en "numero_ganador" el resultado de la rutina asociada al hilo "th_1"
  // pasandole la dirección de memoria del puntero (al usar el operador de dirección &)
  if(pthread_join(th_1, (void**) &numero_ganador) != 0){
    perror("error al hacer join del hilo (1)");
  }

  // - creamos el hilo 'th_2' y le asociamos la rutina 'elegirInvitado'
  // - pasamos a 'numero_ganador' como parámetro a la rutina 'elegirInvitado'
  // que está asociada al hilo 'th_2'
  if(pthread_create(&th_2, NULL, &elegirInvitado, (void*) numero_ganador) != 0){
    perror("error al crear hilo (1)");
  }
  if(pthread_join(th_2, NULL) != 0){
    perror("error al hacer join del hilo (1)");
  }

  printf("El número ganador es %d\n", *numero_ganador);
  // liberamos el espacio en memoria reservada por la rutina asociada al hilo "th"
  free(numero_ganador);

  return 0;
}

// -------------------------------------------------------------------------------
//
void* generarNumeroRandom(){
  sleep(1);

  // - generamos valores entre 0 y 5
  // - hacemos constante el tipo de la variable => sólo será 'int' NO se podrá castear a otro tipo
  int const numero = rand() % 5;
  // - creamos un puntero a int, y le reservamos espacio para un entero
  int* res = malloc(sizeof(int));
  // - desreferenciamos el puntero, y le asignamos el valor generado
  *res = numero;
  // - retornamos el resultado como un puntero a null
  // el que lo use deberá castearlo a (int*) que sería un puntero a int
  return (void*) res;
}

void* elegirInvitado(void *arg){
  int numeroGanador = *(int*) arg;
  /* char* nombreGanador = malloc(sizeof(char)*20); */
  // "creo" que no es necesario reservar memoria, ni tampoco liberar el puntero
  // porque este puntero apunta a una dirección de memoria del Stack (fue allocada de forma estática)
  char *nombreGanador = invitados[numeroGanador];

  printf("El nombre del invitado ganador es %s\n", nombreGanador);

  pthread_exit(NULL);
}
