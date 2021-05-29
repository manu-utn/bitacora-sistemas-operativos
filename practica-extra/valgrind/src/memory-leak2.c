#include <stdio.h>
#include <stdlib.h>

int main(){
  int *a = malloc(sizeof(int));

  return 0;
}
==27094== HEAP SUMMARY:
  ==27094==     in use at exit: 4 bytes in 1 blocks
  ==27094==   total heap usage: 1 allocs, 0 frees, 4 bytes allocated
  ==27094==
  ==27094== LEAK SUMMARY:
  ==27094==    definitely lost: 4 bytes in 1 blocks
  ==27094==    indirectly lost: 0 bytes in 0 blocks
  ==27094==      possibly lost: 0 bytes in 0 blocks
  ==27094==    still reachable: 0 bytes in 0 blocks
  ==27094==         suppressed: 0 bytes in 0 blocks

