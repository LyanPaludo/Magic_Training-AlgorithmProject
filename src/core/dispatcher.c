#include "dispatcher.h"
#include "../ui/menu/menu.h"
#include "../ui/menu/particulas.h"
#include "../ui/game/cutscene.h"
#include <allegro5/allegro5.h>
#include "../ui/game/fases/tutorial.h"
#include "../gameplay/player.h"
#include "../entities/enemy/enemy.h"
#include "viewport.h"

void atualizar_estado(GameState estado, double delta_time) {
    switch (estado) {
        case ESTADO_MENU:
            atualizar_particulas();
            break;
        case ESTADO_TUTORIAL:
            atualizar_tutorial(delta_time);
            atualizar_player(delta_time);
            atualizar_inimigos(delta_time);
            break;
        default:
            break; // outros estados ainda não têm update
    }
}

void desenhar_estado(GameState estado) {
    switch (estado) {
        case ESTADO_MENU:
            al_clear_to_color(al_map_rgb(10, 10, 20));
            desenhar_particulas();
            desenhar_menu();
            break;

        case ESTADO_CUTSCENE:
            al_clear_to_color(al_map_rgb(0, 0, 0));
            desenhar_cutscene(&cutscene_atual, 0);
            break;

        case ESTADO_TUTORIAL:
            al_clear_to_color(al_map_rgb(0, 0, 0));
            aplicar_viewport();
            desenhar_tutorial();
            desenhar_inimigos();
            desenhar_player();
            restaurar_viewport();
            break;

        default:
            al_clear_to_color(al_map_rgb(10, 10, 20));
            break;
    }
}

void processar_evento_estado(GameState estado, ALLEGRO_EVENT *evento) {
    switch (estado) {
        case ESTADO_TUTORIAL:
        case ESTADO_JOGANDO:
            if (evento->type == ALLEGRO_EVENT_KEY_DOWN) {
                if (evento->keyboard.keycode == ALLEGRO_KEY_LEFT) player_pressionar_esquerda(1);
                if (evento->keyboard.keycode == ALLEGRO_KEY_RIGHT) player_pressionar_direita(1);
            }
            if (evento->type == ALLEGRO_EVENT_KEY_UP) {
                if (evento->keyboard.keycode == ALLEGRO_KEY_LEFT) player_pressionar_esquerda(0);
                if (evento->keyboard.keycode == ALLEGRO_KEY_RIGHT) player_pressionar_direita(0);
            }
            break;
        default:
            break;
    }
}