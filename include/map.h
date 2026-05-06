#ifndef MAP_H
#define MAP_H

#include "raylib.h"

#define TILE 32
#define MAP_LINHAS 14
#define MAP_COLUNAS 80

#define TILE_VAZIO -1
#define TILE_CHAO 0
#define TILE_BLOCO 1
#define TILE_PLATAFORMA 2

void carregarMapa(void);
void desenharMapa(Texture2D chao, Texture2D bloco, Texture2D plataforma);
int tileEhSolido(int tile);
int pegarTilePorPixel(float x, float y);

#endif