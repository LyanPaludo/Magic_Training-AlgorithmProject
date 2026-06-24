#ifndef PLAYER_H
#define PLAYER_H

typedef enum {
    PLAYER_PARADO,
    PLAYER_ANDANDO_ESQUERDA,
    PLAYER_ANDANDO_DIREITA,
    PLAYER_CONJURANDO
} EstadoPlayer;

void inicializar_player(void);
void atualizar_player(double delta_time);
void desenhar_player(void);
void finalizar_player(void);

// Getters (spell, enemy)
float obter_player_x(void);
float obter_player_y(void);
EstadoPlayer obter_estado_player(void);
void definir_conjurando(int ativo);  // spell.c chama isso ao abrir/fechar combo

//Funções de input
void player_pressionar_esquerda(int pressionada);
void player_pressionar_direita(int pressionada);

#endif