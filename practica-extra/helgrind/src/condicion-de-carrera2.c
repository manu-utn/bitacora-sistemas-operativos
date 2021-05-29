/*
 * gcc condicion-de-carrera.c -pthread -g -Wall && valgrind --tool=helgrind ./a.out
 *
 * A diferencia del condicion-de-carrera1.c, en este NO usamos variables globales
 * si no que trabajamos con la dirección de memoria de una variable definida en main()
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// recibimos de parámetro un puntero,
// que es lo mismo que decir una dirección de un bloque de memoria
int incrementar_contador(int *contador){
  // usamos el opeador * de indirección/desreferencia para apuntar al valor
  // al que hace referencia el puntero, e incrementamos su valor en 1
  (*contador)++;

  // retornamos el valor al que hace referencia el puntero
  return *contador;
}

void *rutina_incrementar(void* arg){
  // 1. declaramos un puntero a entero
  // 2. casteamos el parámetro que es "puntero a nulo" por un "puntero a entero"
  int *parametro = (int *)arg;
  // a la función incrementar_valor() le pasamos el puntero,
  // osea la dirección de memoria, no el valor en si
  printf("hilo: incrementé el contador a %d\n", incrementar_contador(parametro));

  pthread_exit(NULL);
}

int main(){
  int contador = 0;
  pthread_t hilo_1;
  int estado_hilo = 0;

  // - el cuarto argumento, es el parámetro que recibirá la rutina que aparece como tercer argumento
  //
  // - en el cuarto argumento usamos la operación de casteo,
  // forzando a que el tipo de la variable "contador" sea del tipo puntero a nulo osea (void *)
  // porque es lo que requiere la función pthread_create()
  //
  // - además usamos el operador & de dirección, para pasarle la dirección de memoria de la variable
  // y trabajarla como un puntero dentro de la rutina/función
  // (porque un puntero es sólo una referencia a una bloque de memoria)
  //
  // Obs: La función rutina_incrementar, necesitará castear ese (void *) a (int *)
  // para utilizarlo como un puntero a entero
  estado_hilo = pthread_create(&hilo_1, NULL, rutina_incrementar, (void *)&contador);

  if(estado_hilo){
    printf("Oh.. HAY ERROR!! el código de error es %d", estado_hilo);
    exit(-1); // syscall
  }

  int contador_incrementado = incrementar_contador(&contador);

  // hace que el proceso principal main() que también es hilo,
  // no finalice hasta que el hilo_1 también lo haga, le da tiempo a que se termine de ejecutar
  pthread_join(hilo_1, NULL);
  printf("main: incrementé el contador a %d niaka niaka niaka :)\n", contador_incrementado);

  return 0;
}
