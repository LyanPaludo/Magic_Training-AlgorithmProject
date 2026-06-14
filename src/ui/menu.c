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
#include <math.h>

// Variáveis internas, oposto do extern, static fica só aqui
static ALLEGRO_FONT *fonte_titulo  = NULL;
static ALLEGRO_FONT *fonte_opcoes  = NULL;
static ALLEGRO_FONT *fonte_opcoes_menores  = NULL;

// Definição real da variável declarada como extern no .h
int opcao_menu = 0;
int opcao_config = 0;
int config_aberto = 0;
int foco_config = 0; // Volume ou Resolução


void inicializar_menu(ALLEGRO_FONT *titulo, ALLEGRO_FONT *opcoes, ALLEGRO_FONT *opcoes_menores) {
    fonte_titulo = titulo;
    fonte_opcoes = opcoes;
    fonte_opcoes_menores = opcoes_menores;
}

void desenhar_menu(void) {
    // Cor do título
    ALLEGRO_COLOR cor_titulo  = al_map_rgb(255, 220, 50); 
    ALLEGRO_COLOR cor_normal  = al_map_rgb(200, 200, 200); 
    ALLEGRO_COLOR cor_destaque = al_map_rgb(255, 255, 255); 

    // Título do jogo
    al_draw_text(
        fonte_titulo,
        cor_titulo,
        LARGURA_TELA / 2,  // centro horizontal
        ALTURA_TELA * 0.25,               // altura na tela
        ALLEGRO_ALIGN_CENTRE,
        "Magic Training"
    );

    // Opção Jogar — branco se selecionada, cinza se não
    al_draw_text(
        fonte_opcoes,
        opcao_menu == 0 ? cor_destaque : cor_normal,
        LARGURA_TELA / 2,
        ALTURA_TELA * 0.53,
        ALLEGRO_ALIGN_CENTRE,
        opcao_menu == 0 ? "> Jogar <" : "Jogar"
    );

    // Opção Configurações
    al_draw_text(
        fonte_opcoes,
        opcao_menu == 1 ? cor_destaque : cor_normal,
        LARGURA_TELA / 2,
        ALTURA_TELA * 0.6,
        ALLEGRO_ALIGN_CENTRE,
        opcao_menu == 1 ? "> Configurações <" : "Configurações"
    );

    // Opção Créditos
    al_draw_text(
        fonte_opcoes,
        opcao_menu == 2 ? cor_destaque : cor_normal,
        LARGURA_TELA / 2,
        ALTURA_TELA * 0.67,
        ALLEGRO_ALIGN_CENTRE,
        opcao_menu == 2 ? "> Créditos <" : "Créditos"
    );

    // Opção Sair
    al_draw_text(
        fonte_opcoes,
        opcao_menu == 3 ? cor_destaque : cor_normal,
        LARGURA_TELA / 2,
        ALTURA_TELA * 0.74,
        ALLEGRO_ALIGN_CENTRE,
        opcao_menu == 3 ? "> Sair <" : "Sair"
    );
}

void desenhar_config_popup(void) {
    ALLEGRO_COLOR cor_normal  = al_map_rgb(200, 200, 200); 
    ALLEGRO_COLOR cor_destaque = al_map_rgb(255, 255, 255); 
    ALLEGRO_COLOR cor_caixa = al_map_rgba(30,30,30,230);
    ALLEGRO_COLOR cor_borda = al_map_rgb(0,0,0);

    al_draw_filled_rounded_rectangle(
        LARGURA_TELA * 0.2, ALTURA_TELA * 0.2,
        LARGURA_TELA * 0.8, ALTURA_TELA * 0.8,
        20, 20,
        cor_caixa
    );
    al_draw_rounded_rectangle(
        LARGURA_TELA * 0.2, ALTURA_TELA * 0.2,
        LARGURA_TELA * 0.8, ALTURA_TELA * 0.8,
        20, 20,
        cor_borda,
        2.0
    );

    al_draw_text(
        fonte_opcoes,
        cor_destaque,
        LARGURA_TELA * 0.5,
        ALTURA_TELA * 0.25,
        ALLEGRO_ALIGN_CENTRE,
        "CONFIGURAÇÕES"
    );

    al_draw_text(
        fonte_opcoes_menores,
        opcao_config == 0 ? cor_destaque : cor_normal,
        LARGURA_TELA * 0.25,
        ALTURA_TELA * 0.35,
        ALLEGRO_ALIGN_LEFT,
        opcao_config == 0 ? "> Volume:" : "Volume:"
    );

    al_draw_filled_rounded_rectangle(
        opcao_config == 0 ? LARGURA_TELA * 0.46 : LARGURA_TELA*0.462, 
        opcao_config == 0 ? ALTURA_TELA * 0.359 : ALTURA_TELA * 0.361, 
        opcao_config == 0 ? LARGURA_TELA * 0.78 : LARGURA_TELA * 0.778, 
        opcao_config == 0 ? ALTURA_TELA * 0.401 : ALTURA_TELA * 0.399,
        10, 10,
        al_map_rgb(120, 120, 120)
    );

    al_draw_rounded_rectangle(
        opcao_config == 0 ? LARGURA_TELA * 0.46 : LARGURA_TELA*0.462, 
        opcao_config == 0 ? ALTURA_TELA * 0.359 : ALTURA_TELA * 0.361, 
        opcao_config == 0 ? LARGURA_TELA * 0.78 : LARGURA_TELA * 0.778, 
        opcao_config == 0 ? ALTURA_TELA * 0.401 : ALTURA_TELA * 0.399,
        10, 10,
        al_map_rgb(255,255,255),
        2.0
    );
    if (VOLUME_GLOBAL>0.0){
        al_draw_filled_rounded_rectangle(//BARRA DE PROGRESSO VOLUME
            opcao_config == 0 ? LARGURA_TELA * 0.46 : LARGURA_TELA*0.462, 
            opcao_config == 0 ? ALTURA_TELA * 0.359 : ALTURA_TELA * 0.361, 
            opcao_config == 0 ? LARGURA_TELA*0.461 + ((LARGURA_TELA * 0.78 - LARGURA_TELA*0.461) * VOLUME_GLOBAL) 
                                : LARGURA_TELA*0.461 + ((LARGURA_TELA * 0.778 - LARGURA_TELA*0.461) * VOLUME_GLOBAL), 
            opcao_config == 0 ? ALTURA_TELA * 0.401 : ALTURA_TELA * 0.399,
            10, 10,
            al_map_rgb(255, 75, 50)
        );
    }
    al_draw_text(
        fonte_opcoes_menores,
        cor_normal,
        LARGURA_TELA * 0.25,
        ALTURA_TELA * 0.45,
        ALLEGRO_ALIGN_LEFT,
        "Resolução:"
    );

    al_draw_text(
        fonte_opcoes_menores,
        opcao_config == 1 ? cor_destaque : cor_normal,
        LARGURA_TELA * 0.32,
        ALTURA_TELA * 0.51,
        ALLEGRO_ALIGN_LEFT,
        opcao_config == 1 ? "> 800x600 <" : "800x600"
    );

    al_draw_text(
        fonte_opcoes_menores,
        opcao_config == 2 ? cor_destaque : cor_normal,
        LARGURA_TELA * 0.32,
        ALTURA_TELA * 0.56,
        ALLEGRO_ALIGN_LEFT,
        opcao_config == 2 ? "> 1024x768 <" : "1024x768"
    );

    al_draw_text(
        fonte_opcoes_menores,
        opcao_config == 3 ? cor_destaque : cor_normal,
        LARGURA_TELA * 0.32,
        ALTURA_TELA * 0.61,
        ALLEGRO_ALIGN_LEFT,
        opcao_config == 3 ? "> 1280x720 <" : "1280x720"
    );

    al_draw_text(
        fonte_opcoes_menores,
        opcao_config == 4 ? cor_destaque : cor_normal,
        LARGURA_TELA * 0.5,
        ALTURA_TELA * 0.7,
        ALLEGRO_ALIGN_CENTRE,
        opcao_config == 4 ? "> SAIR <" : "SAIR"
    );
}