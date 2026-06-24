#ifndef VIEWPORT_H
#define VIEWPORT_H

// Aplica a escala/centralização do canvas virtual 800x600 na tela real.
void aplicar_viewport(void);

// Restaura a transformação anterior
void restaurar_viewport(void);

#endif