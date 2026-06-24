#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "game.h"
#include <allegro5/allegro5.h>

void atualizar_estado(GameState estado, double delta_time);
void desenhar_estado(GameState estado);
void processar_evento_estado(GameState estado, ALLEGRO_EVENT *evento);
void atualizar_estado(GameState estado, double delta_time);

#endif