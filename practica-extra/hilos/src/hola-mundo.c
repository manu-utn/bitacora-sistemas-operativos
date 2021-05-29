#include <stdio.h>
#include <string.h> // para utilizar el formato %s en el printf()
#include <stdlib.h> // para fflush()
#include <pthread.h>

/*************************************************************/
/**************** Declaración de funciones  ******************/
void *contar(void *arg);
void *saludar(void *arg);

/*************************************************************/
/**************** Programa Principal  ************************/
int main(int argc, char **argv){
  // declaramos los hilos
  // al momento de usar pthread_create() se crearán los hilos, y se guardará una referencia de cada hilo en estas variables
  pthread_t hilo1, hilo2;

  char *nombre = "Carlitos";

  // forzamos con la operación de casteo,
  // que la cadena de caracteres sea del tipo requerido por pthread_create()
  pthread_create(&hilo1, NULL, saludar, (void *)nombre);

  // Otra manera de pasar parámetros... Sería pasarle la dirección de memoria de la variable,
  // usando el operador & de dirección

  int veces = 5;
  // creamos el hilo, y ejecutará la rutina/función asignada (en este caso es contar)
  if(pthread_create(&hilo2, NULL, contar, &veces) != 0){
    // podemos devolver un mensaje de error, en caso de que falle
    return -1;
  }

  // esperamos que se termine de ejecutar la rutina del hilo
  // si no lo hacemos, apenas finaliza main() también lo hará el hilo y no se terminará de ejecutar
  pthread_join(hilo1, NULL);
  pthread_join(hilo2, NULL);

  return 0; // fin del programa principal main()
}


/*************************************************************/
/**************** Definimos las funciones  *******************/

void *saludar(void *arg){
  // declaramos un puntero a caracteres (ó el famoso string, cadena de caracteres)
  char* nombre;
  // casteamos el dato al que hace referencia el parámetro, que es "puntero a nulo"
  // a un puntero a caracteres
  nombre = (char *) arg;

  // el formato %s provisto por stdlib.h indica que es un String
  // e imprime el principio de bloque de memoria asignado de "nombre" hasta encontrar el caracter nulo \0
  printf("Hola..! %s\n", nombre);
  fflush(stdout); // vaciamos el buffer

  pthread_exit(NULL);
}

void *contar(void *arg){
  // casteamos el valor
  // - con (int *) lo casteamos/convertimos en puntero a entero
  // - con *() obtenemos el valor del puntero, con el operador * asterisco de indirección/desreferencia
  int veces = *((int*)arg);

  printf("Sabias que se contar..? Mirá ");
  for(int i=0; i < veces; i++){
    printf("%d, ", i);
  }
  printf("\n");

  pthread_exit(NULL);
}
