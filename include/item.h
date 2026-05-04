#ifndef ITEM_H
#define ITEM_H

#include "raylib.h"
#include "player.h"

typedef struct Heart {
    float x, y;
    int ativo;
    struct Heart *prox;
} Heart;

void carregarItens(void);
void atualizarItens(Player *player);
void desenharItens(Texture2D heartTexture);

#endif