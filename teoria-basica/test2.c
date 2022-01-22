#include <stdio.h>
#include <stdbool.h>

//bool es_vector_nulo(int dim, int vector[dim]) {
bool es_vector_nulo(int dim, int* vector) {
    for (int i = 0; i < dim; i++) {
      if (vector[i] != 0) {
        return false;
      }
    }

    return true;
  }

  int main() {
    int dim = 4;
    int vector1[4] = {1, 1, 1, 1};
    int vector2[4] = {0, 0, 0, 0};

    for (int i = 0; i < dim; i++) {
      printf("%d ", vector1[i]);
    }
    printf("\n");
    for (int i = 0; i < dim; i++) {
      printf("%d ", vector2[i]);
    }
    printf("\n");

    if (es_vector_nulo(dim, vector1)) {
      printf("vector 1: si\n");
    } else {
      printf("vector 1: no\n");
    }

    if (es_vector_nulo(dim, vector2)) {
      printf("vector 2: si\n");
    } else {
      printf("vector 2: no\n");
    }

    printf("\n");

    return 0;
  }
