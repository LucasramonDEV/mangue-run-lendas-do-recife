#include "map.h"

static int mapa[MAP_LINHAS][MAP_COLUNAS];

void carregarMapa(void) {
    for (int i = 0; i < MAP_LINHAS; i++) {
        for (int j = 0; j < MAP_COLUNAS; j++) {
            mapa[i][j] = TILE_VAZIO;
        }
    }

    for (int j = 0; j < MAP_COLUNAS; j++) {
        mapa[10][j] = (j < 40) ? TILE_CHAO_MARCO : TILE_CHAO_BOM;
        mapa[11][j] = mapa[10][j];
    }

    mapa[9][12] = TILE_CAIXA_MARCO;
    mapa[9][13] = TILE_CAIXA_MARCO;

    mapa[9][55] = TILE_CAIXA_BOM;
    mapa[9][56] = TILE_CAIXA_BOM;

    mapa[8][20] = TILE_PLATAFORMA;
    mapa[8][21] = TILE_PLATAFORMA;
    mapa[8][22] = TILE_PLATAFORMA;

    mapa[7][45] = TILE_PLATAFORMA;
    mapa[7][46] = TILE_PLATAFORMA;
    mapa[7][47] = TILE_PLATAFORMA;

    for (int i = 6; i <= 9; i++) {
        mapa[i][79] = TILE_FINAL;
    }
}

int tileEhSolido(int tile) {
    return tile == TILE_CHAO_MARCO ||
           tile == TILE_CHAO_BOM ||
           tile == TILE_CAIXA_MARCO ||
           tile == TILE_CAIXA_BOM ||
           tile == TILE_PLATAFORMA;
}

int pegarTilePorPixel(float x, float y) {
    int coluna = (int)(x / TILE);
    int linha = (int)(y / TILE);

    if (linha < 0 || linha >= MAP_LINHAS || coluna < 0 || coluna >= MAP_COLUNAS) {
        return TILE_VAZIO;
    }

    return mapa[linha][coluna];
}

void desenharMapa(Texture2D tileset) {
    for (int i = 0; i < MAP_LINHAS; i++) {
        for (int j = 0; j < MAP_COLUNAS; j++) {
            int tile = mapa[i][j];

            if (tile == TILE_VAZIO) continue;

            Rectangle origem = {(tile - 1) * TILE, 0, TILE, TILE};
            Vector2 destino = {j * TILE, i * TILE};

            DrawTextureRec(tileset, origem, destino, WHITE);
        }
    }
}