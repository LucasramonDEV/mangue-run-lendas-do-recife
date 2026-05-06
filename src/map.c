#include "map.h"

static int mapa[MAP_LINHAS][MAP_COLUNAS];

void carregarMapa(void) {
    for (int i = 0; i < MAP_LINHAS; i++) {
        for (int j = 0; j < MAP_COLUNAS; j++) {
            mapa[i][j] = TILE_VAZIO;
        }
    }

    for (int j = 0; j < MAP_COLUNAS; j++) {
        mapa[11][j] = TILE_CHAO;
    }
}

int tileEhSolido(int tile) {
    return tile == TILE_CHAO;
}

int pegarTilePorPixel(float x, float y) {
    int coluna = (int)(x / TILE);
    int linha = (int)(y / TILE);

    if (linha < 0 || linha >= MAP_LINHAS || coluna < 0 || coluna >= MAP_COLUNAS) {
        return TILE_VAZIO;
    }

    return mapa[linha][coluna];
}

void desenharMapa(Texture2D chao, Texture2D bloco, Texture2D plataforma) {
    for (int i = 0; i < MAP_LINHAS; i++) {
        for (int j = 0; j < MAP_COLUNAS; j++) {
            if (mapa[i][j] == TILE_CHAO) {
                DrawTexture(chao, j * TILE, i * TILE, WHITE);
            }
        }
    }
}