#ifndef FONTES_H
#define FONTES_H

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


ALLEGRO_FONT *obter_fonte(const char *caminho, int tamanho);

void liberar_fontes(void);

#endif