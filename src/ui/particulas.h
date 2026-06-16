#ifndef PARTICULAS_H
#define PARTICULAS_H

//Para as partículas, como o jogo envolve diferentes magias, criei um struct com diferentes formas geométricas.
typedef enum {
    FORMA_ORBE,      // círculo
    FORMA_FAISCA,    // triângulo
    FORMA_CRISTAL    // losango
} FormaParticula;

void inicializar_particulas(void);
void atualizar_particulas(void);
void desenhar_particulas(void);

#endif