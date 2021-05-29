#include <stdio.h>

#define DELAY 128000;

struct cronometro{
  int hora;
  int minuto;
  int segundo;
} t_cronometro;

// las funciones mostar() y actualizar()
// tendran de parámetro un puntero a una estructura
void mostrar(struct cronometro *);
void actualizar(struct cronometro *);
void delay(void);

int main(){

  t_cronometro.hora = 0;
  t_cronometro.minuto = 0;
  t_cronometro.segundo = 0;

  // loop infinito
  for(;;){
    // en ambas funciones le pasamos la direccion de memoria
    // en sus declaraciones vemos el operador de desreferencia *
    // se pueden trabajar como un puntero a una estructura
    mostrar(&t_cronometro);
    actualizar(&t_cronometro);
  }

  return 0;
}

void mostrar(struct cronometro *c){
  printf("%d:%d:%d\n", c->hora, c->minuto, c->segundo);
}
void actualizar(struct cronometro *c){
  c->segundo++;
  if(c->segundo == 60){
    c->minuto++;
    c->segundo = 0;
  }
  if(c->minuto == 60){
    c->hora++;
    c->minuto = 0;
  }
  if(c->hora == 24){
    c->hora = 0;
  }

  delay();
}

void delay(void){
  int x;
  for(x=0; x < 128000 ; ++x) ;
}
