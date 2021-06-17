#include <stdio.h>
#include "player/player.h"

int main(){
  Jugador fede = crearJugador("fede", 15);
  printf("Hola %s tus puntos son %d\n", getNombre(fede), getPuntos(fede));
  liberar(fede);
  return 0;
}
