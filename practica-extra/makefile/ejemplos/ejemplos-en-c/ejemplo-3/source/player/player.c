#include "player.h"

Jugador crearJugador(char* nombre, int puntos){
  Jugador jugador;
  jugador.nombre = malloc(sizeof(char)* 50);
  strcpy(jugador.nombre, nombre);
  jugador.puntos = puntos;

  return jugador;
}

char* getNombre(Jugador jugador){
  return jugador.nombre;
}

int getPuntos(Jugador jugador){
  return jugador.puntos;
}

void liberar(Jugador jugador){
  free(jugador.nombre);
}
