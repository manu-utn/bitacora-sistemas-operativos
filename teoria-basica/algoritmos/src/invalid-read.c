#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>

void _string_append(char* original, char* to_append){
  original = realloc(original, strlen(original) + strlen(to_append) + 1);
  strcat(original, to_append);
}

char* _string_duplicate(char* str){
  return strdup(str);
}

int main(void){
  /* char* nombre = malloc(sizeof(char)*7); */
  /* strcpy(nombre, "Matias"); */

  char* name = _string_duplicate("Matias");

  /*
   * Perfilamos la memoria con Valgrind
   * - Si descomentamos nuestra implementación, tendrá problemas de "invalid-read" según valgrind
   * - Los problemas de memoria se producen porque pasamos la referencia del puntero (ptr) que se reservó en Heap
   * - El problema se resuelve si pasamos la dirección de memoria del puntero (&ptr) que se reservó en Stack
   *
   * El mensaje será "Invalid read of size 1" tres veces,
   * faltaron reservar 3 Bytes? Valgrind dice que se perdieron 11 Bytes
   *
   * Depuramos con GDB
   * - El "invalid-read" de valgrind, es porque se reservó 8 Bytes "MatiasSo"
   *
   * 1. Agregamos un breakpoint en la segunda sentencia donde invocamos a _string_append()
   * (gdb) b 15
   *
   * 2. Hacemos que se ejecute la sentencia para ver los cambios, avanzando a la siguiente instrucción con "next"
   * > (gdb) n
   *
   * 3. El tamaño de la cadena devolverá 8 Bytes, es decir 8 caracteres
   * > (gdb) print sizeof(name)
   * > $12 = 8
   *
   * 4. La longitud de la cadena devolverá 10
   * > (gdb) print (int)strlen(name)
   * > $13 = 10
   *
   * 5. Imprimimos el contenido del puntero de la forma *cadena@longitud
   * > (gdb) *name@8
   * > $14="MatiasSo"
   *
   * > (gdb) *name@10
   * > $15="MatiasSosa"
   */

  //string_append(&name, "Sosa"); // <- esta versión del commons usa (char**s, char*s)
  _string_append(name, "Sosa"); // <- nuestra versión producirá errores, de forma intencional

  printf("%s\n", name);

  //free(name);

  return 0;
}
