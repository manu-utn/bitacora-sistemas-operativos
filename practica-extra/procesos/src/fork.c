/*
 * gcc -pthread fork.c && ./a.out
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>

sem_t procesos_disponibles;
sem_t sem_padre;
sem_t sem_hijo;

int main(void){
  // - el semáforo que esté en 1, se ejecutará
  // - el semáforo que esté en 0, NO se ejecutará
  // a menos que le hagan un sem_signal();
  sem_init(&sem_padre, 0, 1); // semáforo binario
  sem_init(&sem_hijo, 0, 0); // semáforo binario
  sem_init(&procesos_disponibles, 0, 10); // semáforo contador

  // 1. El while(1) es necesario porque si el proceso-hijo se ejecuta más rápido
  // osea antes que el proceso-padre, entonces no va a recibir el sem_post(&sem_hijo)
  // para que ejecute su bloque de código
  // 2. El while(1) sería para que el proceso-hijo esté atento por más tiempo
  // para cuando el proceso-padre le avise que se ejecute, sería como un doble chequeo
  while(1){
    sleep(5);
    sem_wait(&procesos_disponibles);
    pid_t pid = fork(); // <- crea un proceso nuevo con todo este código .c

    int numero = 1;

    printf("HOLA! ");
    // fork() retorna 0, cuando es un proceso hijo
    if(pid == 0){
      sem_wait(&sem_hijo); // sem--
      numero++;
      printf("hijo: Soy un proceso hijo\n");
      printf("hijo: mi (PID) es %d \n", getpid());
      printf("hijo: el (PID) de mi proceso padre es %d\n", getppid());
      //sem_post(&sem_padre); // sem--
      sem_post(&procesos_disponibles); // sem++
    }
    // fork() retorna un n > 0, cuando es el proceso padre
    else{ // pid > 0
      sem_wait(&sem_padre); // sem--
      printf("padre: Soy un proceso padre\n");
      printf("padre: mi (PID) es %d \n", getpid());
      sem_post(&sem_hijo); // sem++
      sem_post(&procesos_disponibles); // sem++
    }
  }

  return 0;
}
/*
 1. Al ejecutar el programa varias veces, puede que
 las instrucciones NO se ejecuten en el orden deseado
 2. Por eso se necesita de semáforos binarios si queremos
 mantener un orden de ejecución "padre"->"hijo"

 # Alternativa #1 - en desorden
 Hola..! padre: Soy un proceso padre
 Hola..! hijo: Soy un proceso hijo
 padre: mi (PID) es 6272
 hijo: mi (PID) es 6273
 hijo: el (PID) de mi proceso padre es 6272

 # Alternativa #2 - en orden
 Hola..! padre: Soy un proceso padre
 padre: mi (PID) es 6263
 Hola..! hijo: Soy un proceso hijo
 hijo: mi (PID) es 6264
 hijo: el (PID) de mi proceso padre es 6263

 */

/*
 Por que usar los semáforos..? Para mantener este orden

 HOLA! padre: Soy un proceso padre
 padre: mi (PID) es 14282
 HOLA! hijo: Soy un proceso hijo
 hijo: mi (PID) es 14308 <------------------- este es proceso-hijo, pero será también será un proceso-padre
 hijo: el (PID) de mi proceso padre es 14282

 HOLA! padre: Soy un proceso padre
 padre: mi (PID) es 14308 <--------- éste es un proceso-hijo pero también es proceso-padre de otro
 HOLA! hijo: Soy un proceso hijo
 hijo: mi (PID) es 14312
 hijo: el (PID) de mi proceso padre es 14308

 HOLA! padre: Soy un proceso padre
 padre: mi (PID) es 14312 <-------- éste es proceso-hijo pero también será proceso-padre de otro.. y asi con c/hijo
 HOLA! hijo: Soy un proceso hijo
 hijo: mi (PID) es 14314
 hijo: el (PID) de mi proceso padre es 14312
 */
