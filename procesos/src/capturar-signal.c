#include <stdio.h>
#include <signal.h>

// estas macros son solo de referencia
//
#define SIGHUP  1   /* Hangup the process */
#define SIGINT  2   /* Interrupt the process */
#define SIGQUIT 3   /* Quit the process */
#define SIGILL  4   /* Illegal instruction. */
#define SIGTRAP 5   /* Trace trap. */
#define SIGABRT 6   /* Abort. */

void handle_sigint(int sig){
  printf("Capturé la señal %d y.. ahora como saldrás..?:)\n", sig);
}

int main(){
  // capturamos el atajo de teclas CTRL+C que solemos usar
  // para interrumpir un proceso en ejecución
  signal(SIGINT, handle_sigint);

  // loop infinito
  while(1){
    sleep(1);
    // imprimimos cualquier mensaje para molestar
    printf("Tururu..\n");
  }

  return 0;
}
