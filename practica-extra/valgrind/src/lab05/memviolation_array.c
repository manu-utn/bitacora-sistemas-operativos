/* memviolation_array.c */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  int i, *a;

  a = calloc(10, sizeof(int));

  for(i=0; i <= 10; i++) {
    a[i] = i;
  }
  for(i=0; i <= 10; i++) {
    printf("%d\n", a[i]);
  }

  return 0;
}
