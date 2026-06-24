#include "viewport.h"
#include "config.h"
#include <allegro5/allegro.h>
#include <math.h>

#define VIRTUAL_W 800.0f
#define VIRTUAL_H 600.0f

static ALLEGRO_TRANSFORM transform_anterior;

void aplicar_viewport(void) {
    al_copy_transform(&transform_anterior, al_get_current_transform());

    int screen_w = al_get_display_width(al_get_current_display());
    int screen_h = al_get_display_height(al_get_current_display());

    float escala = fminf((float)screen_w / VIRTUAL_W, (float)screen_h / VIRTUAL_H);
    float largura_final = VIRTUAL_W * escala;
    float altura_final  = VIRTUAL_H * escala;
    float offset_x = (screen_w - largura_final) / 2.0f;
    float offset_y = (screen_h - altura_final) / 2.0f;

    ALLEGRO_TRANSFORM nova;
    al_identity_transform(&nova);
    al_translate_transform(&nova, offset_x, offset_y);
    al_scale_transform(&nova, escala, escala);
    al_use_transform(&nova);
}

void restaurar_viewport(void) {
    al_use_transform(&transform_anterior);
}