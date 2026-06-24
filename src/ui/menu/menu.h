#ifndef MENU_H
#define MENU_H

//Definição da opção destacada no menu principal pelo usuário:
extern int opcao_menu; // Extern avisa que a variável foi definida em outro local
extern int opcao_config;
extern int config_aberto; // config aberto
extern int foco_config;
/*
opções no menu:
-Jogar (0)
-Configurações (1)
-Créditos (2)
-Sair (3)
*/
// Desenha o menu
void desenhar_menu(void);
// Desenha config
void desenhar_config_popup(void);
#endif