#include <stdlib.h>   // rand(), srand()
#include <time.h>     // time() para seed do rand
#include <math.h>     // sin()
#include "particulas.h"
#include "../../core/config.h"
#include <allegro5/allegro_primitives.h>

#define NUM_PARTICULAS 50
//struct para cada particula
typedef struct {
    float x, y;
    float vel_x, vel_y;
    float alpha;         //Opacidade
    float alpha_vel;     //Vel. de mudança da opacidade
    float tamanho;
    float oscilacao;     // fase atual da onda lateral
    float osc_vel;       // velocidade da oscilação
    FormaParticula forma;
    int cor_tipo;        // 0=roxo, 1=azul, 2=amarelo, 3=vermelho
} Particula;

static Particula particulas[NUM_PARTICULAS];


//Função para resetar as partículas
static void resetar_particula(Particula *p) {
    p->x         = (float)(rand() % LARGURA_TELA);
    p->y         = ALTURA_TELA + (float)(rand() % 50);  // nasce abaixo da tela
    p->vel_y     = 0.3f + (float)(rand() % 10) / 10.0f; // entre 0.3 e 1.3
    p->vel_x     = 0.0f;
    p->alpha     = 0.3f + (float)(rand() % 5) / 10.0f;  // entre 0.3 e 0.8
    p->alpha_vel = 0.001f + (float)(rand() % 3) / 1000.0f;
    p->tamanho   = 3.0f + (float)(rand() % 8);          // entre 3 e 11 pixels
    p->oscilacao = (float)(rand() % 628) / 100.0f;      // fase aleatória (0 a 2pi)
    p->osc_vel   = 0.02f + (float)(rand() % 3) / 100.0f;
    p->forma     = (FormaParticula)(rand() % 3);
    p->cor_tipo  = rand() % 4;
}

//Função para inicializar as particulas
void inicializar_particulas(void) {
    srand((unsigned int)time(NULL));
    //gera as NUM_PARTICULAS particulas
    for (int i = 0; i < NUM_PARTICULAS; i++) {
        resetar_particula(&particulas[i]);
        // Espalha pela tela desde o início
        particulas[i].y = (float)(rand() % ALTURA_TELA);
    }
}

//função para atualizar as particulas (fazer elas subir)
void atualizar_particulas(void) {
    for (int i = 0; i < NUM_PARTICULAS; i++) {
        Particula *p = &particulas[i];  // ponteiro para não copiar a struct

        p->y         -= p->vel_y;
        p->oscilacao += p->osc_vel;
        p->x         += sinf(p->oscilacao) * 0.5f;  // balanço suave
        p->alpha     -= p->alpha_vel; //opacidade

        // se a particula chegou ao final, ela reseta
        if (p->alpha <= 0.0f || p->y < -p->tamanho) {
            resetar_particula(p);
        }
    }
}

//Por fim, a função para de fato desenhar as partículas
void desenhar_particulas(void) {
    // Ativa transparência, para o allegro não ignorar o alpha
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    for (int i = 0; i < NUM_PARTICULAS; i++) {
        Particula *p = &particulas[i];
        
        // Define a cor baseada no cor_tipo e no alpha atual da partícula
        ALLEGRO_COLOR cor;
        if (p->cor_tipo == 0)
            cor = al_map_rgba_f(0.235f,0.0f,1.0f, p->alpha);  // roxo
        else if (p->cor_tipo == 1)
            cor = al_map_rgba_f(0.4f, 0.69f, 1.0f, p->alpha);  // azul
        else if (p->cor_tipo == 2)
            cor = al_map_rgba_f(1.0f, 0.99f, 0.53f, p->alpha);  // amarelo
        else
            cor = al_map_rgba_f(1.0f, 0.44, 0.4, p->alpha); // vermelho

        // Desenha a forma
        if (p->forma == FORMA_ORBE) {
            al_draw_filled_circle(p->x, p->y, p->tamanho, cor);

        } else if (p->forma == FORMA_FAISCA) {
            float t = p->tamanho;
            al_draw_filled_triangle(
                p->x,       p->y - t,    // topo
                p->x - t,   p->y + t,    // baixo esquerda
                p->x + t,   p->y + t,    // baixo direita
                cor
            );

        } else if (p->forma == FORMA_CRISTAL) {
            float t = p->tamanho;
            float verts[8] = {
                p->x,     p->y - t,  // cima
                p->x + t, p->y,      // direita
                p->x,     p->y + t,  // baixo
                p->x - t, p->y       // esquerda
            };
            al_draw_filled_polygon(verts, 4, cor);
        }
    }

    // Restaura blending padrão para não afetar o resto do draw
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
}