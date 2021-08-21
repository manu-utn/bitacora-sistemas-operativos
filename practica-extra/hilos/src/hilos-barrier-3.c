#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> // malloc(), rand()
#include <unistd.h> // sleep()
#include <time.h> // time()

// -------------------------------------------------------------------------------
// >
#define THREADS_MAX 8

pthread_barrier_t barrier_invitados;
pthread_barrier_t barrier_ganador;

int invitados[THREADS_MAX];
// se inicializan todos los elementos con cero
int estado[THREADS_MAX] = {0};

void* adivinarNumero(void* args);
void elegirAlGanador();

// -------------------------------------------------------------------------------
// > MAIN
int main(int argc, char* argv[]){
  // inicializamos la semilla
  srand(time(NULL));

  // declaramos un vector de hasta THREADS_MAX hilos
  pthread_t th[THREADS_MAX];

  // - inicializamos los "semáforos contadores"
  // - la condición es que debe haber un número de (THREADS_MAX hilos + hilo principal main)
  //
  // Sumamos el main como valor en la condición para garantizar prioridad de ejecución entre hilos
  //
  // 1. Se avisa a elegirAlGanador() que terminaron de ejecutar las rutinas de los hilos creados
  // con pthread_barrier_wait(&barrier_invitados)
  //
  // 2. Se avisa a la rutina 'adivinarNumero' de cada hilo que ya se eligió el ganador
  // con pthread_barrier_wait(&barrier_ganador)
  pthread_barrier_init(&barrier_invitados, NULL, THREADS_MAX+1);
  pthread_barrier_init(&barrier_ganador, NULL, THREADS_MAX+1);

  for(int i = 0; i < THREADS_MAX; i++){
    // > Creamos un puntero a entero, y reservamos memoria para un entero,
    //   le asignamos el valor del iterador, y se lo pasamos de parámetro
    //   a la rutina de cada hilo
    //
    // > Si en vez de pasarle de parámetro el puntero, le pasamos la dirección de meoria
    // de la variable i osea &i "podríamos" tener problemas de sincronización
    // (esto se ve mejor en el ejemplo hilos-barriers-2.c)
    int* valor = malloc(sizeof(int));
    *valor = i;

    if(pthread_create(&th[i], NULL, &adivinarNumero, valor) != 0){
      perror("Error: Falló al crear un hilo");
    }
  }

  // ---------------------------------------------------- \\

  while(1){
    printf("\n\n");
    // - Notificamos que "todos" los invitados ya eligieron/adivinaron su número
    // - Si lo comentamos, NO se ejecuta el código que imprime los resultados por pantalla
    // (es una de las dos condiciones para que se ejecute ese código)
    printf("> Los invitados están adivinando un número..\n");
    pthread_barrier_wait(&barrier_invitados); // <- por esto se sumaba el main al iniciazar este barrier

    // luego que todos los invitados tienen asignado un número
    // - elegimos al ganador, quien acertó el número (usamos una lógica propia para elegirlos)
    printf("> Se está eligiendo al ganador/es..\n");
    elegirAlGanador();

    // - Notificamos que ya se eligió al ganador
    // - Si lo comentamos, NO se ejecuta el código que imprime los resultados por pantalla
    // (es una de las dos condiciones para que se ejecute ese código)
    printf("> Los resultados son:\n");
    pthread_barrier_wait(&barrier_ganador);  // <- por esto se sumaba el main al iniciazar este barrier

    sleep(2);
  }

  // ---------------------------------------------------- \\

  for(int i = 0; i < THREADS_MAX; i++){
    if(pthread_join(th[i], NULL) != 0){
      perror("Error: Falló al hacer join de un hilo");
    }
  }

  pthread_barrier_destroy(&barrier_ganador);
  pthread_barrier_destroy(&barrier_invitados);

  return 0;
}


// -------------------------------------------------------------------------------
// >

void* adivinarNumero(void* args){
  int numero_invitado = *(int*) args;

  while(1){
    // cada invitado tendrá un número al azar de entre el 1 y el 10
    invitados[numero_invitado] = rand() % 10 + 1;

    // - el hilo no avanza de los wait() hasta que se cumpla la condición, del valor asociado a cada barrier
    // - se duerme al hilo asociado a esta rutina hasta que
    //   1. se alcance el valor con el que se inicializo el 'barrier_invitados'
    //   2. se alcance el valor con el que se inicializo el 'barrier_ganador'
    pthread_barrier_wait(&barrier_invitados); // <- se cumple si todos los invitados participaron
    pthread_barrier_wait(&barrier_ganador);   // <- se cumple si ya se elegió el ganador/es

    // si se alcanzó el valor de ambos barriers, llegó acá
    // ya se le puede decir a cada invitado si ganó o no
    if(estado[numero_invitado] == 1){
      printf("El invitado %d acertó.! :D Su número era %d\n",
             numero_invitado, invitados[numero_invitado]);
    }else{
      printf("El invitado %d NO acertó.! :( Su número era %d\n",
             numero_invitado, invitados[numero_invitado]);
    }
  }

  //free(args);

  pthread_exit(NULL);
}

void elegirAlGanador(){
  int numeroGanador = rand() % 10 + 1; // <-- HACERLO CON rand()

  for(int i =0; i < THREADS_MAX; i++){
    if(invitados[i] == numeroGanador){
      estado[i] = 1; // acertó
    }else{
      estado[i] = 0; // NO acertó
    }
  }
}
