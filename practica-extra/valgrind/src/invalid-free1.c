#include <stdio.h>
#include <stdlib.h>

int main(){
  int *a = malloc(sizeof(int));
  *a = 4;

  printf("a vale %d\n", *a);
  free(a);
  //free(a); // Deberia haber Problemas here..!

  return 0;
}
/*
 * ==23431== Invalid free() / delete / delete[] / realloc()
 * ==23431==    at 0x483F9AB: free (vg_replace_malloc.c:538)
 * ==23431==    by 0x1091A9: main (invalid-free1.c:10)
 */
