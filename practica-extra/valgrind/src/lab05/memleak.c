#include <stdio.h>
#include <stdlib.h>

//set a constant pre-compiler value
#define TEST_SIZE 5

/**
 * Take an array and double its length by duplicating its values.
 * For example, array with values {1,2,3} double-up's to {1,2,3,1,2,3}.
 *
 * @arg a : int * : array to be doubled-up
 * @arg size: int : size of the array a
 * @return int * : pointer to new doubled-up array
 **/
int * doubleup(int * a, int size) {
  int i;
  int * new_a;

  new_a = calloc(size*2, sizeof(int));

  for(i = 0; i < size; i++) {
    new_a[i] = a[i];
    new_a[i+size] = a[i];
  }

  return new_a;
}


int main(int argc, char * argv[]) {
  int i;
  int * a;

  a = calloc(TEST_SIZE, sizeof(int));

  //initialize the array
  for(i=0; i<TEST_SIZE; i++) {
    a[i] = i+1;
  }

  //print the array
  for(i=0; i<TEST_SIZE; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");

  printf("-----------\n");

  //doubleup
  a = doubleup(a, TEST_SIZE);

  //print the array
  for(i=0; i<TEST_SIZE*2; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");

  return 0;
}
