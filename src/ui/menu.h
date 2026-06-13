#ifndef MENU_H
#define MENU_H

#include <allegro5/allegro_font.h>

//Definição da opção destacada no menu principal pelo usuário:
extern int opcao_menu; // Avisa que a variável foi definida em outro local
/*
opções no menu:
-Jogar (0)
-Créditos (1)
-Sair (2)
*/
// Carrega as fontes e prepara o menu
void inicializar_menu(ALLEGRO_FONT *fonte_titulo, ALLEGRO_FONT *fonte_opcoes);
// Desenha o menu
void desenhar_menu(void);
#endif