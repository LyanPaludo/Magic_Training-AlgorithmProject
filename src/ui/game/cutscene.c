//CUTSCENES

#include "cutscene.h"
#include "../../core/config.h"
#include "../../core/game.h"
#include "../../core/fontes.h"

Cutscene cutscene_atual;

void configurar_cutscene(Cutscene *p, const char *principal, const char *secundaria, GameState proximo) {
    p->linha_principal  = principal;
    p->linha_secundaria = secundaria;
    p->proximo_estado   = proximo;
}
    

void desenhar_cutscene(Cutscene *p, int corSec) {
    ALLEGRO_FONT *fonte_titulo  = obter_fonte("assets/fonts/Agbalumo-Regular.ttf", ALTURA_TELA * 0.08);
    ALLEGRO_FONT *fonte_corpo   = obter_fonte("assets/fonts/Tinos-Regular.ttf", ALTURA_TELA * 0.035);
    ALLEGRO_FONT *fonte_enter  = obter_fonte("assets/fonts/Silkscreen-Regular.ttf", ALTURA_TELA * 0.035);

    ALLEGRO_COLOR cor_titulo   = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR cor_normal   = al_map_rgb(200, 200, 200);
    ALLEGRO_COLOR cor_destaque = al_map_rgb(255, 50, 50);

    al_draw_text(
        fonte_titulo, 
        cor_titulo, 
        LARGURA_TELA * 0.5, 
        ALTURA_TELA * 0.3,
        ALLEGRO_ALIGN_CENTRE, 
        p->linha_principal
    );

    al_draw_multiline_text(
        fonte_corpo, 
        corSec == 0 ? cor_normal : cor_destaque,
        LARGURA_TELA * 0.5, 
        ALTURA_TELA * 0.55, 
        LARGURA_TELA * 0.8,
        al_get_font_line_height(fonte_corpo),
        ALLEGRO_ALIGN_CENTRE, 
        p->linha_secundaria
    );

    al_draw_text(
        fonte_enter, 
        cor_normal, 
        LARGURA_TELA * 0.5, 
        ALTURA_TELA * 0.85,
        ALLEGRO_ALIGN_CENTRE, 
        "Press enter to continue..."
    );
}
