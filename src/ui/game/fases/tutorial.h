#ifndef TUTORIAL_H
#define TUTORIAL_H
#include <allegro5/allegro_primitives.h>


//carrega texturas do cenário
void inicializar_tutorial(void);

//atualiza qualquer lógica do tutorial
void atualizar_tutorial(double delta_time);

//desenha o cenário
void desenhar_tutorial(void);

// Libera as texturas
void finalizar_tutorial(void);

#endif