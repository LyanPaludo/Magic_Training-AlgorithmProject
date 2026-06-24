#include "fontes.h"
#include <string.h>
#include <allegro5/allegro_ttf.h>

#define MAX_FONTES_CACHE 32

typedef struct {
    char caminho[128];
    int  tamanho;
    ALLEGRO_FONT *fonte;
} EntradaFonte;

static EntradaFonte cache[MAX_FONTES_CACHE];
static int total_cache = 0;

ALLEGRO_FONT *obter_fonte(const char *caminho, int tamanho) {
    // Procura se já existe essa combinação no cache
    for (int i = 0; i < total_cache; i++) {
        if (cache[i].tamanho == tamanho && strcmp(cache[i].caminho, caminho) == 0) {
            return cache[i].fonte;  // já existe, devolve sem recarregar
        }
    }

    // Não existe —> carrega e guarda no cache
    ALLEGRO_FONT *nova = al_load_ttf_font(caminho, tamanho, 0);
    if (nova != NULL && total_cache < MAX_FONTES_CACHE) {
        strcpy(cache[total_cache].caminho, caminho);
        cache[total_cache].tamanho = tamanho;
        cache[total_cache].fonte   = nova;
        total_cache++;
    }
    return nova;
}

void liberar_fontes(void) {
    for (int i = 0; i < total_cache; i++) {
        al_destroy_font(cache[i].fonte);
    }
    total_cache = 0;
}