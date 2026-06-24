#include <allegro5/allegro5.h> // Biblioteca principal
#include <allegro5/allegro_font.h> // Biblioteca para escrever na tela
#include <allegro5/allegro_ttf.h> // Outra biblioteca complementar para com a anterior
#include <allegro5/allegro_primitives.h> // Para desenhar os jogadores, formas e tudo o mais
#include <allegro5/allegro_audio.h> // Biblioteca de audio
#include <allegro5/allegro_acodec.h> // Decodificador para a biblioteca de audio
#include "src/core/game.h" // Arquivo do jogo, para quesitos de fases do jogo e tudo o mais
#include "src/ui/menu/menu.h" // Arquivo do menu principal
#include "src/core/config.h" // Arquivo header com as configurações do jogo
#include "src/core/audio.h" // arquivo de audio geral
#include "src/ui/menu/particulas.h" // arquivo de particulas do menu principal
#include "src/ui/game/cutscene.h" // arquivo das cutscenes
#include "src/core/fontes.h" // arquivo de fontes
#include "src/core/dispatcher.h" // arquivo de dispatch para atualizar e desenhar
#include <allegro5/allegro_image.h> // imagem
#include "src/ui/game/fases/tutorial.h"//tutorial
#include "src/gameplay/player.h"//player

int main(void) {
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();
    al_init_image_addon();

    ALLEGRO_DISPLAY     *tela  = al_create_display(LARGURA_TELA, ALTURA_TELA);
    ALLEGRO_TIMER       *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *fila  = al_create_event_queue();

    inicializar_audio();
    inicializar_particulas();
    inicializar_tutorial();
    inicializar_player();

    configurar_cutscene(&cutscene_atual,
        "Você começa sua aventura...",
        "Os inimigos estão chegando! Utilize as sequências corretas de teclas para conjurar uma magia",
        ESTADO_TUTORIAL
    );

    al_register_event_source(fila, al_get_display_event_source(tela));
    al_register_event_source(fila, al_get_timer_event_source(timer));
    al_register_event_source(fila, al_get_keyboard_event_source());

    al_start_timer(timer);
    al_set_window_title(tela, "Magic Training");

    GameState estado          = ESTADO_MENU;
    int       rodando         = 1;
    int       precisa_desenhar = 0;

    trocar_musica_por_estado(estado);

    while (rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = 0;
        }

        if (evento.type == ALLEGRO_EVENT_TIMER) {
            atualizar_estado(estado, 1.0 / FPS);
            precisa_desenhar = 1;
        }

        if (config_aberto) {
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    config_aberto = 0;
                    opcao_config = 0;
                }
                if (!foco_config) {
                    if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN) { if (opcao_config < 4) opcao_config++; }
                    if (evento.keyboard.keycode == ALLEGRO_KEY_UP)   { if (opcao_config > 0) opcao_config--; }
                } else {
                    if (evento.keyboard.keycode == ALLEGRO_KEY_LEFT)  {if (VOLUME_GLOBAL > 0) VOLUME_GLOBAL -= 0.1f; aplicar_volume(VOLUME_GLOBAL); }
                    if (evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) {if (VOLUME_GLOBAL < 1) VOLUME_GLOBAL += 0.1f; aplicar_volume(VOLUME_GLOBAL); }
                }
                if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    if (opcao_config == 0) { foco_config = !foco_config; }
                    else if (opcao_config == 4) { config_aberto = 0; opcao_config = 0; }
                    else {
                        if (opcao_config == 1) { LARGURA_TELA = resolucoes[0][0]; ALTURA_TELA = resolucoes[0][1]; }
                        if (opcao_config == 2) { LARGURA_TELA = resolucoes[1][0]; ALTURA_TELA = resolucoes[1][1]; }
                        if (opcao_config == 3) { LARGURA_TELA = resolucoes[2][0]; ALTURA_TELA = resolucoes[2][1]; }

                        al_unregister_event_source(fila, al_get_display_event_source(tela));
                        al_destroy_display(tela);
                        tela = al_create_display(LARGURA_TELA, ALTURA_TELA);
                        liberar_fontes();
                        inicializar_particulas();
                        al_register_event_source(fila, al_get_display_event_source(tela));
                        al_set_window_title(tela, "Magic Training");
                    }
                }
            }
        } else {
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    if (estado == ESTADO_MENU)    rodando = 0;
                    if (estado == ESTADO_JOGANDO) { estado = ESTADO_PAUSE; trocar_musica_por_estado(estado); }
                    if (estado == ESTADO_PAUSE)   { estado = ESTADO_JOGANDO; trocar_musica_por_estado(estado); }
                }
                if (estado == ESTADO_CUTSCENE && evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    estado = cutscene_atual.proximo_estado;
                    trocar_musica_por_estado(estado);
                }
                if (estado == ESTADO_MENU) {
                    if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN) { if (opcao_menu < 3) opcao_menu++; }
                    if (evento.keyboard.keycode == ALLEGRO_KEY_UP)   { if (opcao_menu > 0) opcao_menu--; }
                    if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                        if (opcao_menu == 0) { estado = ESTADO_CUTSCENE; trocar_musica_por_estado(estado); }
                        if (opcao_menu == 1) config_aberto = 1;
                        if (opcao_menu == 2) estado = ESTADO_CREDITOS;
                        if (opcao_menu == 3) rodando = 0;
                    }
                }
            }

            // delega para o dispatcher qualquer evento que não foi tratado acima
            processar_evento_estado(estado, &evento);
        }

        if (precisa_desenhar && al_is_event_queue_empty(fila)) {
            desenhar_estado(estado);
            if (config_aberto) desenhar_config_popup();
            al_flip_display();
            precisa_desenhar = 0;
        }
    }

    liberar_fontes();
    al_destroy_display(tela);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila);
    return 0;
}