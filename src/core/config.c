/*
Assim como o menu.c, esse arquivo serve para deixar o código mais organizado, separando uma função específica em 
um código separado do main.c. Este em específico irá organizar as configurações definidas no config.h
*/

#include "config.h"

int   LARGURA_TELA  = 800;
int   ALTURA_TELA   = 600;
float VOLUME_GLOBAL = 1.0f;

// Array de resoluções disponíveis
int resolucoes[3][2] = {
    {800,  600},
    {1024, 768},
    {1280, 720}
};
int resolucao_atual = 0;  // índice no array acima