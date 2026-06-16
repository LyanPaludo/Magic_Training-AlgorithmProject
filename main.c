#include <allegro5/allegro5.h> // Biblioteca principal
#include <allegro5/allegro_font.h> // Biblioteca para escrever na tela
#include <allegro5/allegro_ttf.h> // Outra biblioteca complementar para com a anterior
#include <allegro5/allegro_primitives.h> // Para desenhar os jogadores, formas e tudo o mais
#include <allegro5/allegro_audio.h> // Biblioteca de audio
#include <allegro5/allegro_acodec.h> // Decodificador para a biblioteca de audio
#include "src/core/game.h" // Arquivo do jogo, para quesitos de fases do jogo e tudo o mais
#include "src/ui/menu.h" // Arquivo do menu principal
#include "src/core/config.h" // Arquivo header com as configurações do jogo
#include "src/core/audio.h" // arquivo de audio geral
#include "src/ui/particulas.h" // arquivo de particulas do menu principal

int main(void) { // define função main, que não recebe nenhum argumento (void)
    // Inicialização do Allegro e seus módulos
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();

    // Recursos principais
    ALLEGRO_DISPLAY      *tela  = al_create_display(LARGURA_TELA, ALTURA_TELA); // TELA
    ALLEGRO_TIMER        *timer = al_create_timer(1.0 / FPS); // TIMER
    ALLEGRO_EVENT_QUEUE  *fila  = al_create_event_queue(); // FILA DE EVENTOS
    //Fontes
    ALLEGRO_FONT *fonte_titulo = al_load_ttf_font("assets/fonts/Agbalumo-Regular.ttf", ALTURA_TELA*0.12, 0);
    ALLEGRO_FONT *fonte_opcoes = al_load_ttf_font("assets/fonts/Tinos-Regular.ttf", ALTURA_TELA*0.06, 0);
    ALLEGRO_FONT *fonte_opcoes_menores = al_load_ttf_font("assets/fonts/Silkscreen-Regular.ttf", ALTURA_TELA*0.042, 0);

    //Função definida em menu.c
    inicializar_menu(fonte_titulo, fonte_opcoes, fonte_opcoes_menores);
    inicializar_audio();
    tocar_musica("assets/audio/music/fundo_menu.ogg");
    inicializar_particulas();
    // Registra as ações do usuário para com o jogo, tela, frames e teclado.
    al_register_event_source(fila, al_get_display_event_source(tela));
    al_register_event_source(fila, al_get_timer_event_source(timer));
    al_register_event_source(fila, al_get_keyboard_event_source());

    al_start_timer(timer); // Inicia o timer
    al_set_window_title(tela, "Magic Training"); // Seta o título da janela

    // -Variáveis de controle do loop-
    //Variável que define o estado do jogo, estão definidas em game.h, dentro de src/core
    GameState estado        = ESTADO_MENU;
    int       rodando       = 1;
    int       precisa_desenhar = 0;// Define quando é a hora de desenhar, importante para a mecânica do jogo

    
    // --- Loop principal ---
    while (rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila, &evento); // Isso define que o programa só faz algo quando algo acontecer

        // Verificação para ver se a janela foi fechada
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = 0;
        }

        // Verifica se o evento chegou, caso tenha chegado, atualiza a tela.
        if (evento.type == ALLEGRO_EVENT_TIMER) {
            if (estado == ESTADO_MENU) {
                atualizar_particulas();
            }
            precisa_desenhar = 1;
        }

        // Sistema para seleção de opção
        if (config_aberto){// verifica se o campo das config está aberto.
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {//KEY_DOWN é se está pressionado
                if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) { // verifica se ESC está apertado
                    config_aberto=0;//Fecha configurações
                    opcao_config=0;
                }
                if (!foco_config){
                    if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                        if (opcao_config < 4){opcao_config++;}
                    }
                    if (evento.keyboard.keycode == ALLEGRO_KEY_UP) {
                        if (opcao_config > 0){opcao_config--;}
                    }
                }else{
                    if (evento.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                        if (VOLUME_GLOBAL>0){VOLUME_GLOBAL-=0.1;}
                        aplicar_volume(VOLUME_GLOBAL);
                    }
                    if (evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                        if (VOLUME_GLOBAL<1){VOLUME_GLOBAL+=0.1;}
                        aplicar_volume(VOLUME_GLOBAL);
                    }
                }
                if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    if (opcao_config == 0){if (foco_config){foco_config=0;}else{foco_config=1;}}
                    else if (opcao_config == 4){config_aberto=0;opcao_config=0;}
                    else {
                        if (opcao_config == 1){LARGURA_TELA = resolucoes[0][0]; ALTURA_TELA = resolucoes[0][1];}
                        if (opcao_config == 2){LARGURA_TELA = resolucoes[1][0]; ALTURA_TELA = resolucoes[1][1];}
                        if (opcao_config == 3){LARGURA_TELA = resolucoes[2][0]; ALTURA_TELA = resolucoes[2][1];}
                        al_unregister_event_source(fila, al_get_display_event_source(tela));
                        al_destroy_display(tela); 
                        tela = al_create_display(LARGURA_TELA, ALTURA_TELA);
                        al_destroy_font(fonte_titulo);
                        al_destroy_font(fonte_opcoes);
                        al_destroy_font(fonte_opcoes_menores);
                        fonte_titulo = al_load_ttf_font("assets/fonts/Agbalumo-Regular.ttf", ALTURA_TELA*0.12, 0);
                        fonte_opcoes = al_load_ttf_font("assets/fonts/Tinos-Regular.ttf", ALTURA_TELA*0.06, 0);
                        fonte_opcoes_menores = al_load_ttf_font("assets/fonts/Silkscreen-Regular.ttf", ALTURA_TELA*0.042, 0);
                        inicializar_menu(fonte_titulo, fonte_opcoes, fonte_opcoes_menores);
                        inicializar_particulas();
                        al_register_event_source(fila, al_get_display_event_source(tela));
                        al_set_window_title(tela, "Magic Training");
                    }
                }                    
            }
        }else{
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) { // verifica se ESC está apertado
                    if (estado == ESTADO_MENU) {
                        rodando = 0;  // ESC no menu fecha o jogo
                    }
                    if (estado == ESTADO_JOGANDO) {
                        estado = ESTADO_PAUSE;  // ESC durante jogo apenas pausa
                    }
                    if (estado == ESTADO_PAUSE) {
                        estado = ESTADO_JOGANDO;  // ESC no pause volta ao jogo
                    }
                }
                if (estado == ESTADO_MENU) {
                    if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                        if (opcao_menu < 3){opcao_menu++;}
                    }
                    if (evento.keyboard.keycode == ALLEGRO_KEY_UP) {
                        if (opcao_menu > 0){opcao_menu--;}
                    }
                    if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                        if (opcao_menu == 0) estado = ESTADO_JOGANDO;
                        if (opcao_menu == 1) config_aberto=1;
                        if (opcao_menu == 2) estado = ESTADO_CREDITOS;
                        if (opcao_menu == 3) rodando = 0;
                    }
                }
            }
        }

        // Só desenha quando a fila está vazia (evita flickering)
        if (precisa_desenhar && al_is_event_queue_empty(fila)) {
            al_clear_to_color(al_map_rgb(10, 10, 20)); // fundo da tela

            if (estado == ESTADO_MENU) {
                desenhar_particulas();
                desenhar_menu();
            }

            if (config_aberto){
                desenhar_config_popup();
            }
            // Aqui vão as funções de draw de cada estado
            // Ex: if (estado == ESTADO_MENU) desenhar_menu();

            al_flip_display(); // Desenha o mapa no backend e só ai ele troca com o front
            precisa_desenhar = 0;
        }
    }

    // --- Limpeza de memória ---
    al_destroy_font(fonte_titulo);
    al_destroy_font(fonte_opcoes);
    al_destroy_font(fonte_opcoes_menores);
    al_destroy_display(tela);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila);
    return 0;
}