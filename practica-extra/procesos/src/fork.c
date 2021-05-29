#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
  pid_t pid = fork();

  int numero = 1;

  printf("Hola..! ");
  // fork() retorna 0, cuando es un proceso hijo
  if(pid == 0){
    numero++;
    printf("Soy un proceso hijo\n");
    printf("> mi (PID) es %d \n", getpid());
    printf("> el (PID) de mi proceso padre es %d\n", getppid());
  }
  // fork() retorna un n > 0, cuando es el proceso padre
  else{ // pid > 0
    printf("Soy un proceso padre\n");
    printf("> mi (PID) es %d \n", getpid());
  }

  return 0;
}
