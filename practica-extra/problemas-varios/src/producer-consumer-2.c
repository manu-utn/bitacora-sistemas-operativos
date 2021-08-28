#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

// -------------------------------------------------------------------------------
// >
#define THREADS_MAX 5
#define BUFFER_TAMANIO 5
#define RECURSOS_MAX 10 // Máxima cantidad de recursos que un Productor puede producir ó un Consumidor puede consumir

sem_t BUFFER_COMPLETO; // semáforo binario
sem_t BUFFER_VACIO;  // semáforo contador

pthread_mutex_t MUTEX_BUFFER;

int BUFFER[BUFFER_TAMANIO];
int RECURSOS = 0;

void* consumidor(void* args);
void* productor(void* args);

// -------------------------------------------------------------------------------
// >

/*
 * BUFFER_VACIO:
 * -> 1. Es el semáforo contador, inicializado en N
 * -> 2. Ese N es la mayor cantidad de recursos que puede generar el producer
 *
 * -> 3. El Producer hará wait(sem) produciendo un sem--
 * cuando se cumpla (sem == 0) el Producer se queda esperando/durmiendo
 *
 * -> 4. Cuando el Consumer haga un signal(sem) produciendo un sem++ hará
 * que se despierte el semáforo Producer, para que genere nuevos recursos
 *
 * -> 5. El nombre del buffer "vacio" no indica realmente que quedó vacío del todo,
 * eso sólo depende de que tanto consuma el Consumer, o cuanto genere el Producer
 *
 * BUFFER_LLENO:
 * -> 1. Es el semáforo binario, inicializado en 0
 * -> 2. Permite manejar prioridad/orden de ejecución (en este caso hace que el Producer comience, y luego el Consumer)
 * -> 3. Al estar inicializado en 0, permanece esperando/durmiendo hasta que el Producer haga signal(sem)
 */
int main(int argc, char* argv[]){
  srand(time(NULL));

  pthread_t th[THREADS_MAX];

  pthread_mutex_init(&MUTEX_BUFFER, NULL);
  // inicializamos semáforo contador
  // -> 1er parámetro: dirección de memoria donde se creará
  // -> 2do parámetro: si es 0, indicamos que el semáforo es compartido unicamente entre hilos de este proceso
  // -> 3er parámetro: Máx. cant. de recursos disponibles (al ser sem=N, será semáforo contador)
  //
  sem_init(&BUFFER_VACIO, 0, BUFFER_TAMANIO);

  // inicializamos semáforo binario
  // -> 1er y 2do parámetro: idem
  // -> 3er parámetro: al tener valor 0 ó 1, actúa como Semáforo Binario
  sem_init(&BUFFER_COMPLETO, 0, 0);

  for(int i=0; i < THREADS_MAX; i++){
    int *numero = malloc(sizeof(int));
    *numero = i;

    if(i > 0){
      if(pthread_create(&th[i], NULL, &consumidor, numero) != 0){
        perror("error..!");
      }
    }
    else{
      if(pthread_create(&th[i], NULL, &productor, NULL) != 0){
        perror("error..!");
      }
    }
  }

  // retardamos el fin de ejecución de hilo main,
  // al menos hasta que terminen de ejecutar los hilos sus rutinas asociadas
  for(int i=0; i < THREADS_MAX; i++){
    if(pthread_join(th[i], NULL) != 0){
      perror("error");
    }
  }

  return 0;
}

// -------------------------------------------------------------------------------
// >

/*
 * Nota (1):
 * si (sem == 0):
 * -> 1. se quedan esperando/durmiendo los hilos, hasta que se agreguen recursos al buffer
 * -> 2. ocurre cuando los hilos consumidores vaciaron el buffer haciendo wait(sem) que hace sem--
 *
 * si (sem > 0):
 * -> 1. hace sem-- por invocar wait(sem)
 * -> 2. avanza a la sig. instrucción debajo del wait()
 *
 * -> cada hilo asociado a esta rutina "consumidor" consumirá recursos
 *  en este caso se ve representado por el decremento en 1 de "RECURSOS"
 *
 * Nota (2):
 * -> 1. si llegó hasta acá es porque (sem > 0) e hizo sem--
 * -> 2. bloquea el recurso compartido de resto de los hilos, para evitar "condición de carrera"
 *    y hace uso de recurso
 *
 * Nota (3):
 * -> 1. avisa que ya usó un recurso del buffer, para que el "productor" agregue más
 * -> 2. en realidad "NO está vacío" no confundir.. sólo avisa que usó un recurso del buffer
 * -> 3. hace sem++ (hasta llegar a su tope, el valor N con el que se inicializó el semáforo contador)
 */
void* consumidor(void* args){
  int numero = *(int*)args;
  int valor;
  int tiempo_de_procesamiento;

  //while(1){
  for(int i=0; i < RECURSOS_MAX; i++){
    sem_wait(&BUFFER_COMPLETO); // -> (1)

    pthread_mutex_lock(&MUTEX_BUFFER); // -> (2)
      valor = BUFFER[RECURSOS];
      RECURSOS--;
      // tiempo_de_procesamiento = rand() % 2 + 1;
      // sleep(tiempo_de_procesamiento);
      printf("consumidor (%d): valor obtenido %d (RECURSOS=%d)\n", numero, valor, RECURSOS);
    pthread_mutex_unlock(&MUTEX_BUFFER);

    sem_post(&BUFFER_VACIO); // -> (3)
  }

  // liberamos memoria del puntero cuando termine el loop
  free(args);
  // por ser un puntero a NULL
  pthread_exit(NULL);
}

/*
 * Nota (1):
 * si (sem == 0):
 * -> 1. se queda durmiendo/esperando como un while(1) y no ejecuta lo que le siga debajo del wait(sem)
 * -> 2. ocurre si los hilos productor llenaron el buffer,
 * porque se ejecutó muchas veces wait(&VACIO)
 *
 * si (sem > 0):
 * -> 1. harán sem-- cada hilo productor, por hacer el wait(sem)
 * -> 2. avanza a la sig. instrucción debajo del wait(sem)
 *
 * -> cada hilo asociado a esta rutina "productor" generará recursos
 *  en este caso se ve representado por el incremento en 1 de "RECURSOS"
 *
 * Nota (2):
 * -> 1. si llegó hasta acá es porque (sem > 0) e hizo sem--
 * -> 2. bloquea el acceso al recurso al resto de los hilos para evitar "race condition"
 *    y hace uso del recurso compartido
 * -> 3. agrega al buffer un valor al azar, e incrementa en 1 a "RECURSOS" porque agregó un recurso
 *
 * Nota (3):
 * -> 1. avisa que volvió agregó recursos al buffer, para que el "consumidor" lo use
 * -> 2. en realidad "no está completo" no confundir, sólo avisa que agregó recursos
 * -> 3. hace sem++
 *
 */
void* productor(void* args){
  int valor = rand() % 10 + 1;
  int tiempo_de_calculo = rand() % 2 + 1;
  /* sleep(tiempo_de_calculo); */

  //while(1){
  for(int i=0; i < RECURSOS_MAX; i++){
    sem_wait(&BUFFER_VACIO); // -> (1)

    pthread_mutex_lock(&MUTEX_BUFFER); // --> (2)
      BUFFER[RECURSOS] = valor;
      RECURSOS++;
      printf("productor: Generando recursos.. (RECURSOS=%d)\n", RECURSOS);
    pthread_mutex_unlock(&MUTEX_BUFFER);

    sem_post(&BUFFER_COMPLETO); // -> (3)
  }

  // por ser un puntero a NULL
  pthread_exit(NULL);
}
