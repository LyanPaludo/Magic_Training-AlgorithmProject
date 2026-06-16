#ifndef AUDIO_H
#define AUDIO_H

//Função que inicialia sistema de audio
void inicializar_audio(void);

//Função que toca uma música em loop, parando a anterior se houver
void tocar_musica(const char *caminho);

//Função para parar a música atual
void parar_musica(void);

//Aplica o volume global
void aplicar_volume(float volume);

#endif