#include "tutorial.h"
#include "../../../core/config.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <math.h>
#include "../../../entities/enemy/enemy.h"
#include <time.h>
#include <stdlib.h>

#define VIRTUAL_W 800.0f
#define VIRTUAL_H 600.0f
static float contador = 0;
static ALLEGRO_BITMAP *cenario = NULL;

void inicializar_tutorial(void) {
    cenario = al_load_bitmap("assets/sprites/cenario/cenario_tutorial.png");
    inicializar_inimigos();
}

void finalizar_tutorial(void) {
    if (cenario) al_destroy_bitmap(cenario);
    cenario = NULL;
}

void atualizar_tutorial(double delta_time) {
    contador += delta_time;
    if (contador >= 2.5){
        TipoInimigo tipo = (TipoInimigo)(rand() % TOTAL_TIPOS_INIMIGO);
        spawnar_inimigo(tipo, (rand() % 201) + 300);
        contador = 0;
    }
}

void desenhar_tutorial(void) {
    al_draw_bitmap(cenario, 0, 0, 0);
}