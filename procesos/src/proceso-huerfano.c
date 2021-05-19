#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void){
  pid_t pid;

  pid = fork();

  if(pid == 0){
    sleep(10);
    printf("Proceso hijo: y mi proceso padre??? soy un proceso huerfano? :(\n");
  }
  else{ // pid > 0
    printf("Proceso padre: Hola, soy un proceso padre, estoy cansado, chau! \n");
    printf("el proceso padre finalizó...\n");
  }

  return 0;
}
