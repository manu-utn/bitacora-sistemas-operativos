#include <stdio.h>
#include <string.h> // para usar memset()
#include <stdlib.h> // para usar rand()
#include <pthread.h>
#include <semaphore.h>

/*******************************************************************/
/*** Declaración de funciones, estructuras y variables globales ***/

// podría haber usado typdef? SI.. pero no quería (?) :)
struct sucursal{
  int codigo;
  int ventas;
};

struct sucursal sucursales[2]; // vector de struct sucursal
int ventaTotal = 0; // recurso compartido
sem_t sem; // declaración del semáforo


int ventas[2][30]; // 2 sucursales (filas), 30 dias (columnas)

void cargarVentas(int *, int);
void *sumarVentas(void *);
void *sumaVentaTotal(void *);


/*************************************************************/
/******************* Programa Principal  *********************/
int main(){
  pthread_t h[2]; // declaramos dos hilos

  // inicializamos el semáforo con valor 1
  // - wait:   al invocar sem_wait(sem) decrementará su valor en 1, quedando en 0 (bloqueando el acceso al recurso compartido)
  // - signal: al invocar sem_post(sem) incrementará su valor en 1, quedando en 0 (desbloqueando el acceso del recurso compartido)
  sem_init(&sem, 0, 1);

  struct sucursal palermo;
  palermo.codigo = 101;
  palermo.ventas = 0;

  struct sucursal belgrano = {105, 0}; // otra forma de definir una estructura

  // atentis.. esto se puede hacer..! :o
  sucursales[0] = palermo;
  sucursales[1] = belgrano;

  // - inicializamos la matriz con valor 0, para evitar que tenga valores basura (Ej. 10123, 0101, 01,...)
  // - alternativa? recorrer con un bucle y usar el operador de asignación =
  memset(&ventas[0][0], 0, sizeof(int)*2*30);

  // - al primer parámetro le pasamos la matriz, con el operador & de dirección
  // para luego en la función cargarVentas() manejarnos como un puntero
  // - al segundo parámetro le indicamos la cant. de elementos que tiene
  cargarVentas(&ventas[0][0], 30);
  cargarVentas(&ventas[1][0], 30);

  printf("Al principio tenemos ventaTotal=$%d\n\n", ventaTotal);

  // estos serán los parámetros que tendrán como argumento la rutina/función
  // asignada a cada hilo
  int numeroSucursales[2] = {0, 1};
  for(int numSucursal=0; numSucursal<2; numSucursal++){
    printf("cargando las ventas en la estructura sucursal (%d)\n", numSucursal);

    // para el 3er parámetro usamos el operador & de dirección, porque la funcion pthread_create()
    // requiere un puntero a nulo, es decir (void *)
    // y.. si recordamos, un puntero es sólo una dirección a memoria, usaremos el & para pasarle
    // la dirección de memoria de cada elemento del arreglo
    pthread_create(&h[numSucursal], NULL, sumarVentas, &numeroSucursales[numSucursal]);
  }

  for(int i=0; i<2; i++){
    printf("sumando las ventas de la sucursal (%d)\n", i);
    pthread_create(&h[i], NULL, sumaVentaTotal, &numeroSucursales[i]);
  }

  for(int i=0; i < 2; i++){
    printf("esperando que termine de ejecutar el hilo (%d)..\n", i);
    // - funcion bloqueante, impide que el programa principal main() finalice, sin que el hilo elegido termine su ejecución
    // - permite que el hilo elegido tenga suficientes ráfagas de cpu para terminar su ejecución
    pthread_join(h[i], NULL);
  }

  printf("\nAl final nos quedó ventaTotal=$%d\n", ventaTotal);

  return 0; // FIN del programa principal main()
}


/*************************************************************/
/**************** Definición de funciones  ******************/

void *sumaVentaTotal(void *arg){
  int numeroSucursal;
  numeroSucursal = *((int *)arg);

  for(int i=0; i<2; i++){
    // decrementamos el valor del semáforo, su valor inicial era 1
    // al quedar en 0, se bloquea el acceso al recurso compartido, para que solo
    // este hilo pueda acceder
    sem_wait(&sem);

    ventaTotal += sucursales[numeroSucursal].ventas;

    // incrementamos el valor de semáforo en 1
    // ahora si otro semáforo necesita el recurso compartido,
    // lo podrá usar.. porque? porque tiene valor 1
    sem_post(&sem);
  }

  pthread_exit(NULL);
}

void *sumarVentas(void *arg){
  // 1. con (int *)arg casteamos el puntero nulo, a un puntero a entero
  // 2. con *() obtenemos el valor al que el puntero a entero está haciendo referencia
  // porque el * actúa como el operador de indirección
  int numeroSucursal = *((int *)arg);

  // acá el * no actúa como operador de indirección, acá si que estamos definiendo un puntero a entero
  // le pasamos la matriz con el operador & para trabajarla con la famosa "aritmética de punteros"
  // y desplazarnos N bytes por el bloque de memoria asignado con la operación ++
  int* puntero = &ventas[0][0];

  for(int i=0; i<30;i++, puntero++){
    sem_wait(&sem);

    sucursales[numeroSucursal].ventas += *puntero;

    sem_post(&sem);
  }

  pthread_exit(NULL);
}

void cargarVentas(int *puntero, int nDias){
  // Ojo...! cuando hacemos puntero++ NO ESTAMOS INCREMENTANDO su valor..
  // estamos diciendo que avance a la siguiente dirección de memoria, de donde está apuntando el puntero
  // porque? porque son contiguas..
  for(int i=0; i < nDias;i++, puntero++){
    // NO confundir en esta parte el * asterísco del identificado puntero,
    // acá NO estamos declarando un puntero, estamos desreferenciando con el operado * de indirección
    // para cambiar el valor al que apunta
    *puntero = rand() % 500; // le asignamos valores aleatorios entre 0 y 500
  }
}
