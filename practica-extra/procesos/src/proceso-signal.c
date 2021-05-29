#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// estas macros son solo de referencia
//
#define SIGHUP  1   /* Hangup the process */
#define SIGINT  2   /* Interrupt the process */
#define SIGQUIT 3   /* Quit the process */
#define SIGILL  4   /* Illegal instruction. */
#define SIGTRAP 5   /* Trace trap. */
#define SIGABRT 6   /* Abort. */

void sigint();

int main(void){

    pid_t pid;

    pid = fork();

    if(pid < 0){
      perror("fork");
      return 1;
    }
    else if(pid == 0){ // proceso hijo
      // capturamos la señal, y reescribimos su comportamiento por otro
      signal(SIGINT, sigint);
      printf("Soy el proceso hijo (PID=%d)..!\n", getpid());

      // loop infinito
      // hasta recibir la señal de proceso padre
      for(;;)
        ; // sentencia nula
    }
    else{ // pid > 0
      sleep(3);
      printf("Proceso padre: envía señal SIGINT a PID=%d\n", pid);
      kill(pid, SIGINT);
    }
}

void sigint(){
  signal(SIGINT, sigint); // la reinicia/reestablece
  printf("Proceso hijo: Recibí una señal de SIGINT...");
}
