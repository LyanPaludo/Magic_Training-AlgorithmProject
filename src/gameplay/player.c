// src/gameplay/player.c
#include "player.h"
#include "../core/config.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define LARGURA_SPRITE 48
#define ALTURA_SPRITE  48

// Posição da faixa central da ponte (800x600)
#define CENTRO_X_PONTE      400.0f  // meio da ponte, horizontalmente
#define METADE_LARGURA_PONTE 130.0f // distância do centro até a borda da ponte
#define Y_BORDA_PONTE       390.0f  // altura Y nas bordas (onde a ponte é mais alta/rasa)
#define PROFUNDIDADE_MAXIMA  35.0f  // quantos pixels ela afunda no centro
#define VELOCIDADE_X        180.0f  // pixels por segundo, movimento lateral
#define VELOCIDADE_LERP_Y     6.0f  // quão rápido o player se ajusta à curva

static ALLEGRO_BITMAP *spr_frontal[2];
static ALLEGRO_BITMAP *spr_esquerda;
static ALLEGRO_BITMAP *spr_direita;

static float x, y;
static float y_alvo;
static EstadoPlayer estado = PLAYER_PARADO;
static int conjurando = 0;

// Controle de animação do frame "frontal" (parado tem 2 frames sutis de respiração)
static float tempo_anim = 0;
static int frame_atual = 0;
static const float TEMPO_POR_FRAME = 0.4f; // segundos por frame quando parado

// Flags de input, definidas pelo main.c via funções abaixo
static int tecla_esquerda_pressionada = 0;
static int tecla_direita_pressionada  = 0;

static float calcular_y_ponte(float pos_x) {
    float t = (pos_x - CENTRO_X_PONTE) / METADE_LARGURA_PONTE;
    if (t < -1.0f) t = -1.0f;
    if (t > 1.0f)  t = 1.0f;

    float curvatura = 1.0f - (t * t);
    return Y_BORDA_PONTE + (curvatura * PROFUNDIDADE_MAXIMA);
}

void inicializar_player(void) {
    spr_frontal[0] = al_load_bitmap("assets/sprites/player/player_frontal_0.png");
    spr_frontal[1] = al_load_bitmap("assets/sprites/player/player_frontal_1.png");
    spr_esquerda   = al_load_bitmap("assets/sprites/player/player_esquerda.png");
    spr_direita    = al_load_bitmap("assets/sprites/player/player_direita.png");

    x = CENTRO_X_PONTE;
    y = calcular_y_ponte(x);   // calcula a altura certa já na posição inicial
    y_alvo = y;
}

void finalizar_player(void) {
    al_destroy_bitmap(spr_frontal[0]);
    al_destroy_bitmap(spr_frontal[1]);
    al_destroy_bitmap(spr_esquerda);
    al_destroy_bitmap(spr_direita);
}

void player_pressionar_esquerda(int pressionada) { tecla_esquerda_pressionada = pressionada; }
void player_pressionar_direita(int pressionada)   { tecla_direita_pressionada  = pressionada; }

void atualizar_player(double delta_time) {
    if (conjurando) {
        estado = PLAYER_CONJURANDO;
    } else if (tecla_esquerda_pressionada) {
        x -= VELOCIDADE_X * (float)delta_time;
        estado = PLAYER_ANDANDO_ESQUERDA;
    } else if (tecla_direita_pressionada) {
        x += VELOCIDADE_X * (float)delta_time;
        estado = PLAYER_ANDANDO_DIREITA;
    } else {
        estado = PLAYER_PARADO;
    }

    if (x < 280) x = 280;
    if (x > 520) x = 520;

    // ↓ ESSA LINHA FALTAVA — recalcula o alvo a cada frame conforme x muda
    y_alvo = calcular_y_ponte(x);

    // Lerp vertical — agora persegue um alvo que muda dinamicamente
    y = y + (y_alvo - y) * VELOCIDADE_LERP_Y * (float)delta_time;

    if (estado == PLAYER_PARADO) {
        tempo_anim += (float)delta_time;
        if (tempo_anim >= TEMPO_POR_FRAME) {
            tempo_anim = 0;
            frame_atual = (frame_atual + 1) % 2;
        }
    }
}

void desenhar_player(void) {
    ALLEGRO_BITMAP *sprite;

    switch (estado) {
        case PLAYER_ANDANDO_ESQUERDA: sprite = spr_esquerda; break;
        case PLAYER_ANDANDO_DIREITA:  sprite = spr_direita;  break;
        case PLAYER_CONJURANDO:       sprite = spr_frontal[0]; break; // troque por sprite de conjurar quando tiver
        default:                      sprite = spr_frontal[frame_atual]; break;
    }

    int largura_original = al_get_bitmap_width(sprite);
    int altura_original   = al_get_bitmap_height(sprite);

    al_draw_scaled_bitmap(
        sprite, 
        0, 
        0, 
        largura_original, 
        altura_original,
        x - LARGURA_SPRITE / 2, 
        y - ALTURA_SPRITE / 2,
        LARGURA_SPRITE, 
        ALTURA_SPRITE, 0);
}

float obter_player_x(void) { return x; }
float obter_player_y(void) { return y; }
EstadoPlayer obter_estado_player(void) { return estado; }
void definir_conjurando(int ativo) { conjurando = ativo; }