#include <stdio.h>
#include <pthread.h>

// Vector de hilos (con capacidad de hasta dos como máximo)
// - Haremos que ambos hilos compitan por acceder a la variable global "contador"
// esto se conoce por "Condición de carrera"
// - El programa tendrá comportamientos erráticos/inesperados por lo dicho arriba
// - El orden de ejecución dependerá del "planificador", a menos que usemos "semáforos"
pthread_t tid[2];

// "Contador" es variable global
// - es un recurso compartido por los dos hilos que crearemos
// - se considera una "sección crítica" porque habrá comportamiento errático
// cuando ambos hilos hagan operaciones de escritura/lectura
// (la operación realizada NO siempre será la deseada, ej. 1+1 "puede" resultar en 3)
int contador;

// Puntero a función void
// - El tipo del "Puntero a función" es void, porque es el que pide pthread_create(4)
// cuando la pasemos por tercer parámetro
// - El argumento es un "Puntero a nulo", por lo mismo que arriba, la firma lo pide.
//
// Nota: La ventaja de pasarle un puntero a nulo, es que podemos pasarle cualquier cosa
// un struct, un vector, etc..
void* contar(void *);

int main(int argc, char** argv){
  int i = 0;
  int error;
  int parametros[2] = {1,2};

  while (i < 2){
    error = pthread_create(&tid[i], NULL, contar, &parametros[i]);

    if (error != 0){
      printf("NO pude crear el hilo..! :(\n");
    }

    i++;
  } // end-while

  printf("Eu, esperamos que se termine de ejecutar el hilo (1)\n");
  pthread_join(tid[0], NULL);
  printf("Ey, esperamos que se termine de ejecutar el hilo (2)\n");
  pthread_join(tid[1], NULL);

  return 0;
}

void* contar(void *arg){
  // recibimos el argumento de pthread_create que es un "puntero a nulo", por tanto
  // 1. necesitamos castear el dato a puntero a int
  // 2. obtenemos el valor, con el operador de indirección/desreferencia * (el asterisco)
  int num_hilo = *((int*)arg);

  for(int i=0; i < 3; i++){
    // - cada hilo intentará acceder a este recurso compartido (la variable global contador)
    // - como NO están sincronizados, porque el "planificador" decide el orden de ejecución
    // veremos que se los números no serán 1,2,3,4.. sino en desorden 1,2,2,3,4,4,3,5..
    contador = contador + 1;

    printf("Hilo %d: Mi tarea (%d) ha comenzado :(\n", num_hilo, contador);

    // creamos un loop como delay, similar a un sleep()
    // esto hará que el "planificador" cambie el orden de ejecución de cada hilo
    for (int i = 0; i < 10000*10000; i++)
      ; // esto es válido y se conoce como "Sentencia nula"
    printf("Hilo %d: Mi tarea (%d) ha finalizado :)\n", num_hilo, contador);
  }

  pthread_exit(NULL);
  //return NULL;
}

