#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void){
  pid_t pid;

  pid = fork();

  if(pid > 0){
    printf("Proceso padre: Hola, soy un proceso padre :)\n");
    sleep(10);

    printf("Proceso padre: y mi proceso hijo? soy un proceso zombie? T_T\n");
    printf("el proceso padre se aburrió y finalizó...\n");
  }
  else if(pid == 0){
    printf("Proceso hijo: Hola, soy un proceso hijo :)\n\n");
    sleep(5);
    printf("El proceso hijo se cansó y finalizó...\n\n");

    // otra manera sería usando un wrapper a la syscall (exit)
    exit(0);
  }

  return 0;
}
