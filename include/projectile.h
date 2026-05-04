#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "raylib.h"

typedef enum {
    PROJETIL_PERSONAGEM,
    PROJETIL_INIMIGO
} TipoProjetil;

typedef struct Projectile {
    float x, y;
    float velX;
    TipoProjetil tipo;
    struct Projectile *prox;
} Projectile;

void dispararProjetil(float x, float y, int direcao, TipoProjetil tipo);
void atualizarProjeteis(float dt);
void desenharProjeteis(Texture2D notaPersonagem, Texture2D tiroInimigo);
Projectile *getListaProjeteis(void);
void removerProjetil(Projectile *alvo);

#endif