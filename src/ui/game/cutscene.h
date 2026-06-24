#ifndef CUTSCENE_H
#define CUTSCENE_H

#include "../../core/game.h"

extern Cutscene cutscene_atual;
// Configura a cutscene que vai ser montada
void configurar_cutscene(Cutscene *p, const char *principal, const char *secundaria, GameState proximo);

//Desenha a cutscene
void desenhar_cutscene(Cutscene *p, int corSec);

#endif