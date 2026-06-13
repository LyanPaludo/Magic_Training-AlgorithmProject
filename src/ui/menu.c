/*
Esse arquivo vai funcionar como uma forma de separar e organizar melhor o código, basicamente ele vai definir 
variáveis que só ele vai ter conhecimento, não vai passar esse peso para o main e vai definir a função de 
inicializar_menu, que vai receber as fontes de texto que estão dentro da pasta "assets/fonts" da pasta principal 
a partir do main.c e armazenar dentro de variáveis internas desse código, e vai utilizar isso para desenhar o 
título e as opções do menu principal de uma forma mais separada
*/

#include "menu.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "../core/config.h"

// Variáveis internas, oposto do extern, static fica só aqui
static ALLEGRO_FONT *fonte_titulo  = NULL;
static ALLEGRO_FONT *fonte_opcoes  = NULL;

// Definição real da variável declarada como extern no .h
int opcao_menu = 0;

void inicializar_menu(ALLEGRO_FONT *titulo, ALLEGRO_FONT *opcoes) {
    fonte_titulo = titulo;
    fonte_opcoes = opcoes;
}

void desenhar_menu(void) {
    // Cor do título
    ALLEGRO_COLOR cor_titulo  = al_map_rgb(255, 220, 50);  // dourado
    ALLEGRO_COLOR cor_normal  = al_map_rgb(200, 200, 200); // cinza claro
    ALLEGRO_COLOR cor_destaque = al_map_rgb(255, 255, 255); // branco

    // Título do jogo
    al_draw_text(
        fonte_titulo,
        cor_titulo,
        LARGURA_TELA / 2,  // centro horizontal
        150,               // altura na tela
        ALLEGRO_ALIGN_CENTRE,
        "Magic Training"
    );

    // Opção Jogar — branco se selecionada, cinza se não
    al_draw_text(
        fonte_opcoes,
        opcao_menu == 0 ? cor_destaque : cor_normal,
        LARGURA_TELA / 2,
        320,
        ALLEGRO_ALIGN_CENTRE,
        opcao_menu == 0 ? "> Jogar <" : "Jogar"
    );

    // Opção Créditos
    al_draw_text(
        fonte_opcoes,
        opcao_menu == 1 ? cor_destaque : cor_normal,
        LARGURA_TELA / 2,
        360,
        ALLEGRO_ALIGN_CENTRE,
        opcao_menu == 1 ? "> Créditos <" : "Créditos"
    );

    // Opção Sair
    al_draw_text(
        fonte_opcoes,
        opcao_menu == 2 ? cor_destaque : cor_normal,
        LARGURA_TELA / 2,
        400,
        ALLEGRO_ALIGN_CENTRE,
        opcao_menu == 2 ? "> Sair <" : "Sair"
    );
}