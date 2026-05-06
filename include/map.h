#ifndef MAP_H
#define MAP_H

#include "raylib.h"

#define TILE 32
<<<<<<< HEAD
#define MAP_LINHAS 14
#define MAP_COLUNAS 80

#define TILE_VAZIO -1
#define TILE_CHAO 0
#define TILE_BLOCO 1
#define TILE_PLATAFORMA 2

void carregarMapa(void);
void desenharMapa(Texture2D chao, Texture2D bloco, Texture2D plataforma);
=======
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
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f
int tileEhSolido(int tile);
int pegarTilePorPixel(float x, float y);

#endif