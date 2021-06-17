#ifndef PLAYER_H
#define PLAYER_H

#include <string.h>
#include <stdlib.h>

typedef struct{
  char* nombre;
  int puntos;
} Jugador;

Jugador crearJugador(char*, int);
char* getNombre(Jugador);
int getPuntos(Jugador);
void liberar(Jugador);
#endif
