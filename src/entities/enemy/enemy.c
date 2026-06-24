#include "enemy.h"
#include "../../core/config.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <math.h>
#include <string.h>
#include "../../core/fontes.h"


#define LARGURA_SPRITE_INIMIGO 40
#define ALTURA_SPRITE_INIMIGO  40
#define LINHA_DERROTA           370.0f  // Y onde o inimigo "cruza" e invade
#define TEMPO_MORTE              0.5f   // segundos exibindo sprite de morte

typedef struct {
    EstadoInimigo estado;
    TipoInimigo   tipo;
    float x, y;
    float velocidade;
    int   hp, hp_max;
    float hp_bar_timer;   // tempo restante visível da barra de vida
    float tempo_morte;    // contador enquanto INIMIGO_MORRENDO
    int   frame_atual;
    float tempo_anim;
} Enemy;

static Enemy inimigos[MAX_INIMIGOS];
static int invasores = 0;

// Sprites [tipo][frame], frame 0 e 1 = andando, frame 2 = morte
static ALLEGRO_BITMAP *sprites[TOTAL_TIPOS_INIMIGO][3];

static const char *caminhos[TOTAL_TIPOS_INIMIGO][3] = {
    { "assets/sprites/enemies/goblin_0.png",    "assets/sprites/enemies/goblin_1.png",    "assets/sprites/enemies/goblin_death.png" },
    { "assets/sprites/enemies/slime_0.png",     "assets/sprites/enemies/slime_1.png",     "assets/sprites/enemies/slime_death.png" },
    { "assets/sprites/enemies/esqueleto_0.png", "assets/sprites/enemies/esqueleto_1.png", "assets/sprites/enemies/esqueleto_death.png" }
};

static const int hp_base[TOTAL_TIPOS_INIMIGO]    = { 40, 10, 30 };       // goblin, slime, esqueleto
static const float vel_base[TOTAL_TIPOS_INIMIGO] = { 25.0f, 5.0f, 15.0f };

void inicializar_inimigos(void) {
    for (int t = 0; t < TOTAL_TIPOS_INIMIGO; t++) {
        for (int f = 0; f < 3; f++) {
            sprites[t][f] = al_load_bitmap(caminhos[t][f]);
        }
    }
    memset(inimigos, 0, sizeof(inimigos));  // zera tudo, todos INATIVO (=0)
    invasores = 0;
}

void finalizar_inimigos(void) {
    for (int t = 0; t < TOTAL_TIPOS_INIMIGO; t++)
        for (int f = 0; f < 3; f++)
            if (sprites[t][f]) al_destroy_bitmap(sprites[t][f]);
}

void spawnar_inimigo(TipoInimigo tipo, float x) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (inimigos[i].estado == INIMIGO_INATIVO) {
            inimigos[i].estado      = INIMIGO_VIVO;
            inimigos[i].tipo        = tipo;
            inimigos[i].x           = x;
            inimigos[i].y           = 10.0f;  // topo da estrada
            inimigos[i].velocidade  = vel_base[tipo];
            inimigos[i].hp          = hp_base[tipo];
            inimigos[i].hp_max      = hp_base[tipo];
            inimigos[i].hp_bar_timer = 0;
            inimigos[i].frame_atual = 0;
            inimigos[i].tempo_anim  = 0;
            return;
        }
    }
}

void atualizar_inimigos(double delta_time) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        Enemy *e = &inimigos[i];
        if (e->estado == INIMIGO_INATIVO) continue;

        if (e->estado == INIMIGO_MORRENDO) {
            e->tempo_morte += (float)delta_time;
            if (e->tempo_morte >= TEMPO_MORTE) {
                e->estado = INIMIGO_INATIVO;  // some de vez
            }
            continue;  // não anda nem ataca enquanto morre
        }

        // Movimento
        e->y += e->velocidade * (float)delta_time;

        // Cruzou a linha de derrota
        if (e->y >= LINHA_DERROTA) {
            invasores++;
            e->estado = INIMIGO_INATIVO;  // desaparece ao invadir
            continue;
        }

        // Timer da barra de vida (some gradualmente se não tomar dano)
        if (e->hp_bar_timer > 0) {
            e->hp_bar_timer -= (float)delta_time;
        }

        // Animação de andar
        e->tempo_anim += (float)delta_time;
        if (e->tempo_anim >= 0.3f) {
            e->tempo_anim = 0;
            e->frame_atual = (e->frame_atual + 1) % 2;
        }
    }
}

void desenhar_inimigos(void) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        Enemy *e = &inimigos[i];
        if (e->estado == INIMIGO_INATIVO) continue;
        
        int frame_sprite = (e->estado == INIMIGO_MORRENDO) ? 2 : e->frame_atual;
        ALLEGRO_BITMAP *sprite = sprites[e->tipo][frame_sprite];

        int lw = al_get_bitmap_width(sprite);
        int lh = al_get_bitmap_height(sprite);

        al_draw_scaled_bitmap(
            sprite, 0, 0, lw, lh,
            e->x - LARGURA_SPRITE_INIMIGO / 2, 
            e->y - ALTURA_SPRITE_INIMIGO / 2,
            LARGURA_SPRITE_INIMIGO, 
            ALTURA_SPRITE_INIMIGO, 
            0
        );

        // Barra de vida
        if (e->hp_bar_timer > 0 && e->estado == INIMIGO_VIVO) {
            float alpha = (e->hp_bar_timer < 0.5f) ? (e->hp_bar_timer / 0.5f) : 1.0f;
            float largura_barra = LARGURA_SPRITE_INIMIGO;
            float proporcao = (float)e->hp / (float)e->hp_max;

            float bx = e->x - largura_barra / 2;
            float by = e->y - ALTURA_SPRITE_INIMIGO / 2 - 8;

            al_draw_filled_rectangle(
                bx, by, bx + largura_barra, by + 4,
                al_map_rgba_f(0.3f, 0, 0, alpha)
            );
            al_draw_filled_rectangle(
                bx, by, bx + largura_barra * proporcao, by + 4,
                al_map_rgba_f(0.9f, 0.1f, 0.1f, alpha)
            );
        }
    }
}

void aplicar_dano_em_area(float x, float y, float raio, int dano) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        Enemy *e = &inimigos[i];
        if (e->estado != INIMIGO_VIVO) continue;

        float dx = e->x - x;
        float dy = e->y - y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist <= raio) {
            e->hp -= dano;
            e->hp_bar_timer = 3.0f;  // mostra a barra por 3 segundos

            if (e->hp <= 0) {
                e->estado = INIMIGO_MORRENDO;
                e->tempo_morte = 0;
            }
        }
    }
}

int obter_invasores(void) { return invasores; }