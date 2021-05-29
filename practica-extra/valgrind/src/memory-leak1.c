#include <stdio.h>
#include <stdlib.h>

int main(){
  int* a = malloc(sizeof(int)*4);

  a = malloc(sizeof(int)*10);
  free(a);

  return 0;
}
==26687== HEAP SUMMARY:
  ==26687==     in use at exit: 16 bytes in 1 blocks
  ==26687==   total heap usage: 2 allocs, 1 frees, 56 bytes allocated
  ==26687== 
  ==26687== LEAK SUMMARY:
  ==26687==    definitely lost: 16 bytes in 1 blocks
  ==26687==    indirectly lost: 0 bytes in 0 blocks
  ==26687==      possibly lost: 0 bytes in 0 blocks
  ==26687==    still reachable: 0 bytes in 0 blocks
  ==26687==         suppressed: 0 bytes in 0 blocks

