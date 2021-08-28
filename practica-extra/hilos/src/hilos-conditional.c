#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* rutina(void*);
void thr_join();
void thr_exit();

int DONE = 0;

// -> inicializamos ambos de forma estática
// -> se realiza en tiempo de compilación
pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t COND = PTHREAD_COND_INITIALIZER;

// ------------------------------------------------------------

int main(int argc, char* argv[]){
  pthread_t th;
  printf("Hilo (main): Hola!\n");

  // -> 1er parámetro: dirección de memoria en donde se guardará el hilo creado
  // -> 3er parámetro: al hilo le asociamos como rutina la función "rutina"
  // -> 4to parámetro: le mandamos NULL como parámetro a "rutina", es decir nada
  pthread_create(&th, NULL, &rutina, NULL);

  // -> emulamos lo que haría pthread_join(th, NULL)
  thr_join();

  return 0;
}

// ------------------------------------------------------------

// -> Bloqueamos el mutex cuando invocamos a pthread_cond_wait() y pthread_cond_signal()
// -> porque ambas se deben ejecutar de forma atómica, para evitar condiciones de carrera

void thr_join(){
  pthread_mutex_lock(&MUTEX);
  // -> loop infinito que se ejecuta mientras no se cumpla la condición de corte
  // -> si se cumple la condición de corte, el hilo ya no necesita esperar
  //
  // -> si usamos IF(condition)    => no se garantiza que sea tan certero el chequeo de la condición
  // -> si usamos WHILE(condition) => garantizamos que la condición true, porque se vuelve a iterar y chequear
  while(DONE == 0){
    printf("Esperando que finalize el hilo..\n");

    // -> hace que el hilo se quede esperando/durmiendo mientras tanto
    // -> si otra función no le manda un signal/1 asociado con esta condición, el hilo se queda durmiendo/esperando
    pthread_cond_wait(&COND, &MUTEX);
  }

  pthread_mutex_unlock(&MUTEX);
}

void thr_exit(){
  // 1. bloqueamos el recurso compartido con el mutex, evitamos una "race condition"
  pthread_mutex_lock(&MUTEX);
    // 2. usamos el recurso compartido, está protegido
    DONE = 1;
    // 3. avisamos que se terminó de ejecutar la tarea/rutina asociada al hilo
    // -> si no hacemos signal() => el hilo se quedará esperando/durmiendo
    // -> es necesario indicar la condición al signal/1, porque wait/2 la tiene asociada
    pthread_cond_signal(&COND);
    // 4. desbloqueamos el recurso compartido
  pthread_mutex_unlock(&MUTEX);
}

void* rutina(void* args){
  // ponemos un delay previo a ejecutar las instrucciones de abajo
  sleep(3);

  // pasado los 3 segundos, se ejecutan las siguientes sentencias
  printf("Hilo: Hola..!\n");

  // --> emulamos lo que haría pthread_exit(NULL)
  thr_exit();

  // retornamos NULL, porque es un puntero a una función a void
  return NULL;
}
