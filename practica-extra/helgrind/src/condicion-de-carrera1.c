/*
 * gcc condicion-de-carrera1.c -pthread -lm -Wall && valgrind --tool=helgrind ./a.out
 *
 * En este ejemplo, habrá condición de carrera entre el hilo main() y el único hilo de la aplicación
*/
#include <stdio.h>
#include <pthread.h>

// - esta variable global será una "sección crítica"
// (cuando varios hilos acceden e realizar una operación de lectura/escritura, en especial por la operación de escritura)
// - al ser "static", su ciclo de vida será hasta el fin de ejecución del programa principal main
// - recordar que el programa principal main() también es un hilo
static int contador = 0;

static void* rutina(void* arg){
  // printf("hilo: voy a acceder y escribir en la variable contador :)\n");
  contador++; // sección crítica, desprotejida

  pthread_exit(NULL);
}

int main(void){
  // declaramos el hilo, al momento de crearse en pthread_create() se guardará la referencia en esta variable
  pthread_t hilo;

  // creamos el hilo, y ejecutamos la rutina/función que tiene asignada
  pthread_create(&hilo, NULL, rutina, NULL);

  // el hilo/proceso principal main() también tratará de acceder y escribir en la variable global "contador"
  // printf("main(): voy a acceder y escribir en la variable contador :)\n");
  contador++; // sección crítica, desprotejida

  // - esta es función bloqueante, que retrasa el fin de ejecución de main()
  // - permite al hilo poder terminar su ejecución antes que main() finalice
  pthread_join(hilo, NULL);

  return 0;
}
