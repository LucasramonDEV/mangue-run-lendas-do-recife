#ifndef MAP_H
#define MAP_H

#include "raylib.h"

#define TILE 32
#define MAP_LINHAS 12
#define MAP_COLUNAS 80

#define TILE_VAZIO 0
#define TILE_CHAO_MARCO 1
#define TILE_CHAO_BOM 2
#define TILE_CAIXA_MARCO 3
#define TILE_CAIXA_BOM 4
#define TILE_PLATAFORMA 5
#define TILE_FINAL 9

void carregarMapa(void);
void desenharMapa(Texture2D tileset);
int tileEhSolido(int tile);
int pegarTilePorPixel(float x, float y);

#endif