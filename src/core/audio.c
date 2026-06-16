/*
Arquivo para rodar a música no menu principal.
*/
#include "audio.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "config.h"

static ALLEGRO_AUDIO_STREAM *musica_atual = NULL;//Variável para guardar stream da música

void inicializar_audio(void) {
    al_reserve_samples(0);
    aplicar_volume(VOLUME_GLOBAL);
}

void tocar_musica(const char *caminho){
    // Para e destroi a música anterior se existir
    if (musica_atual != NULL) {
        al_destroy_audio_stream(musica_atual);
        musica_atual = NULL;
    }

    musica_atual = al_load_audio_stream(caminho, 4, 2048);
    if (musica_atual == NULL) return;
    //seta o loop
    al_set_audio_stream_playmode(musica_atual, ALLEGRO_PLAYMODE_LOOP);
    //taca no mixer
    al_attach_audio_stream_to_mixer(musica_atual, al_get_default_mixer());
}

void parar_musica(void){
    if (musica_atual != NULL) {
        al_destroy_audio_stream(musica_atual);
        musica_atual = NULL;
    }
}

void aplicar_volume(float volume) {
    al_set_mixer_gain(al_get_default_mixer(), volume);
}