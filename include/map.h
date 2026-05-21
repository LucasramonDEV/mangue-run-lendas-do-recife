#ifndef MAP_H
#define MAP_H

#include "raylib.h"

#define MAP_WIDTH 40
#define MAP_HEIGHT 18
#define TOTAL_MAPS 6
#define TELAS_POR_MAPA 5
#define TILE_SIZE 32

#define MAP_COLUNAS MAP_WIDTH
#define MAP_LINHAS MAP_HEIGHT
#define TILE TILE_SIZE

#define TILE_EMPTY ' '
#define TILE_GROUND '='
#define TILE_WALL '|'
#define TILE_PLATFORM '-'
#define TILE_BLOCK 'B'
#define TILE_VOID 'O'
#define TILE_PLAYER 'P'
#define TILE_MELEE 'M'
#define TILE_SHOOTER 'T'
#define TILE_DOOR 'Q'
#define TILE_TUNNEL 'U'
#define TILE_HEART 'H'

extern int mapaAtual;
extern int telaMapaAtual;

const char *GetMapLine(int mapIndex, int telaIndex, int y);

typedef struct {
    Texture2D chao;
    Texture2D bloco;
    Texture2D plataforma;
    int usarTextura;
} MapTextures;

void LoadMapTextures(MapTextures *textures, int mapIndex);
void UnloadMapTextures(MapTextures *textures);

Rectangle GetTileRect(int x, int y);
int IsSolidTile(char tile);
int IsDeadlyTile(char tile);

void DrawMap(int mapIndex, int telaIndex, MapTextures textures);
void FindPlayerStart(int mapIndex, int telaIndex, Rectangle *player);
void FindDoor(int mapIndex, int telaIndex, Rectangle *door);
void FindTunnel(int mapIndex, int telaIndex, Rectangle *tunel);

int pegarTilePorPixel(float px, float py);
int tileEhSolido(int tile);
void carregarMapa(void);
void desenharMapa(Texture2D chao, Texture2D bloco, Texture2D plataforma);

#endif