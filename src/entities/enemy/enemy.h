#ifndef ENEMY_H
#define ENEMY_H

typedef enum {
    TIPO_GOBLIN,
    TIPO_SLIME,
    TIPO_ESQUELETO,
    TOTAL_TIPOS_INIMIGO
} TipoInimigo;

typedef enum {
    INIMIGO_INATIVO,    // não existe, slot livre no array
    INIMIGO_VIVO,
    INIMIGO_MORRENDO  // toca animação de morte antes de desaparecer
} EstadoInimigo;

#define MAX_INIMIGOS 20

void inicializar_inimigos(void);
void finalizar_inimigos(void);
void atualizar_inimigos(double delta_time);
void desenhar_inimigos(void);

// Cria um novo inimigo de um tipo, numa posição X inicial (Y sempre começa no topo)
void spawnar_inimigo(TipoInimigo tipo, float x);

// Aplica dano a TODOS os inimigos dentro de um raio (usado pelo spell depois)
void aplicar_dano_em_area(float x, float y, float raio, int dano);

// Quantos inimigos cruzaram a linha de derrota até agora
int obter_invasores(void);

#endif