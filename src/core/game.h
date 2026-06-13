#ifndef GAME_H
#define GAME_H

// Definição dos estados do jogo
/* 
    ESTADO_MENU = MENU INICIAL;
    ESTADO_CREDITOS = CREDITOS DO JOGO;
    ESTADO_JOGANDO = TELA DE JOGO;
    ESTADO_PAUSE = PAUSE NO JOGO;
    ESTADO_TRANSICAO_FASE = TELA DE TRANSIÇÃO ENTRE UMA FASE E OUTRA;
    ESTADO_FIM = TELA DE SCORE FINAL;
*/
typedef enum {
    ESTADO_MENU,
    ESTADO_CREDITOS,
    ESTADO_JOGANDO,
    ESTADO_PAUSE,
    ESTADO_TRANSICAO_FASE,
    ESTADO_FIM
} GameState;

// Fases do jogo -> usadas para definir a dificuldade do jogo e a pontuação;
typedef enum {
    FASE_1,   // 0:00 - 1:30
    FASE_2,   // 1:30 - 3:00
    FASE_3    // 3:00 - 5:00
} FaseJogo;

#endif