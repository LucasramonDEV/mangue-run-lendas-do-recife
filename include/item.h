#ifndef ITEM_H
#define ITEM_H

#include "raylib.h"

typedef struct Heart {
    Rectangle rect;
    int ativo;
    struct Heart *prox;
} Heart;

void inserirCoracao(float x, float y);
void carregarItens(void);
void atualizarItens(Rectangle player, int *vidas);
void desenharItens(Texture2D heartTexture);
void liberarItens(void);

#endif