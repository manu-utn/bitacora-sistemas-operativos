#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *rutina_para_hilo1();
void *rutina_para_hilo2();

// la función main() también cuenta como "Hilo" para el "Planificador"
// - El main() es el programa principal, y se lo considera como un "Hilo" también
// - Se lo consida como un "Proceso pesado", porque contiene uno o varios hilos (h1, h2)
// que comparten la misma memoria y recursos
// - Si este termina => el resto de los hilos que contenga, también terminarán
int main(int argc, char **argv){
  pthread_t h1, h2;

  // Creamos hilos h1,h2
  // - son hilos a nivel de usuario
  // - se los considera como "procesos livianos"
  // - se ejecutarán en el orden que el "planificador" decida

  printf("Creamos hilo (1)\n");
  // creamos el hilo 1, y lo ponemos en la cola de Ready
  // - Como 3er parámetro le pasamos solo el identificador de la rutina/función
  // por ser un "Puntero a función"
  // - tampóco no necesitamos el uso de los paréntesis, porque NO queremos invocarla
  // solo queremos pasar la dirección de memoria del comienzo del bloque de memoria reservada
  pthread_create(&h1, NULL, rutina_para_hilo1, NULL); // 1er hilo
  // creamos el hilo 2, y lo ponemos en la cola de Ready
  printf("Creamos hilo (2)\n");
  pthread_create(&h2, NULL, rutina_para_hilo2, NULL); // 2do hilo

  printf("Esperamos que se termine de ejecutar el hilo (1)\n");
  // realizamos una "sincronización" con pthread_join()
  // - tiene el mismo concepto del wait()
  pthread_join(h1, NULL);
  printf("Esperamos que se termine de ejecutar el hilo (2)\n");
  pthread_join(h2, NULL);

  printf("Fin de main\n");
  return 0; // <= acá termina el programa principal main()
}

void *rutina_para_hilo1(){
  for(int i=0; i < 10; i++){
    printf("hilo 1: %d\n", rand());
  }

  pthread_exit(NULL);
}
void *rutina_para_hilo2(){
  for(int i=0; i < 5; i++){
    printf("hilo 2: %d\n", rand());
  }

  pthread_exit(NULL);
}
