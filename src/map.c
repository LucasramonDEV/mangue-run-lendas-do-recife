#include "map.h"

int mapaAtual = 0;
int telaMapaAtual = 0;

static const char *mapa1[TELAS_POR_MAPA][MAP_HEIGHT] = {
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "              ?        BB?B?B          ",
        "                                        ",
        "                         ---            ",
        "                                        ",
        "      M        T                        ",
        "                              BB        ",
        " P                         M  BB   U    ",
        "================OOOO====================",
        "================OOOO====================",
        "========================================",
        "========================================"
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                        ?               ",
        "          ||                            ",
        "          ||       ---             ||   ",
        "                   M               ||   ",
        "      ||                           ||   ",
        "      ||        T       M               ",
        " P    ||                       T   U    ",
        "====================OOOO================",
        "====================OOOO================",
        "========================================",
        "========================================"
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "          BB?B                          ",
        "                              BB        ",
        "                              BBB       ",
        "                 ---          BBBB      ",
        "                                        ",
        "      M            T          M         ",
        " P                              U       ",
        "===========OOOO=========================",
        "===========OOOO=========================",
        "========================================",
        "========================================"
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "              BB                        ",
        "              BBB              ?        ",
        "          ---                           ",
        "                                        ",
        "                           ---          ",
        "      M       T                M        ",
        " P                              U       ",
        "======================OOOO==============",
        "======================OOOO==============",
        "========================================",
        "========================================"
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                         B              ",
        "                        BB              ",
        "                       BBB              ",
        "                      BBBB              ",
        "                     BBBBB              ",
        "                    BBBBBB              ",
        " P    M      T     BBBBBBB        Q     ",
        "=================OOOO===================",
        "=================OOOO===================",
        "========================================",
        "========================================"
    }
};

static const char *mapaBase[TELAS_POR_MAPA][MAP_HEIGHT] = {
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "          ?         ---                 ",
        "                                        ",
        "                         ---            ",
        "                                        ",
        "      M         T                       ",
        "                              BB        ",
        " P                         M  BB   U    ",
        "==============OOOO======================",
        "==============OOOO======================",
        "========================================",
        "========================================"
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                  BB?B                  ",
        "                                        ",
        "          ---                 ---       ",
        "                                        ",
        "      T             M                   ",
        "                          BB            ",
        " P      M                   BB     U    ",
        "====================OOOO================",
        "====================OOOO================",
        "========================================",
        "========================================"
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "              ---                       ",
        "                          ?             ",
        "       ---                              ",
        "                                        ",
        "             M        T                 ",
        "                              BB        ",
        " P    M                        BB  U    ",
        "===========OOOO=========================",
        "===========OOOO=========================",
        "========================================",
        "========================================"
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "          BB?B                          ",
        "                              ---       ",
        "                    ---                 ",
        "                                        ",
        "       T           M                    ",
        "                              BB        ",
        " P          M               T  BB  U    ",
        "======================OOOO==============",
        "======================OOOO==============",
        "========================================",
        "========================================"
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        ",
        "                         B              ",
        "                        BB              ",
        "                       BBB              ",
        "                      BBBB              ",
        "                     BBBBB              ",
        "                    BBBBBB              ",
        " P    M      T     BBBBBBB        Q     ",
        "=================OOOO===================",
        "=================OOOO===================",
        "========================================",
        "========================================"
    }
};

const char *GetMapLine(int mapIndex, int telaIndex, int y) {
    if (telaIndex < 0) telaIndex = 0;
    if (telaIndex >= TELAS_POR_MAPA) telaIndex = TELAS_POR_MAPA - 1;

    if (mapIndex == 0) return mapa1[telaIndex][y];

    return mapaBase[telaIndex][y];
}

void LoadMapTextures(MapTextures *textures, int mapIndex) {
    (void)mapIndex;
    textures->usarTextura = 0;
}

void UnloadMapTextures(MapTextures *textures) {
    textures->usarTextura = 0;
}

Rectangle GetTileRect(int x, int y) {
    return (Rectangle){x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
}

int IsSolidTile(char tile) {
    return tile == TILE_GROUND ||
           tile == TILE_WALL ||
           tile == TILE_PLATFORM ||
           tile == TILE_BLOCK ||
           tile == '?';
}

int IsDeadlyTile(char tile) {
    return tile == TILE_VOID;
}

void DrawMap(int mapIndex, int telaIndex, MapTextures textures) {
    (void)textures;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        const char *linha = GetMapLine(mapIndex, telaIndex, y);

        for (int x = 0; x < MAP_WIDTH; x++) {
            char tile = linha[x];
            Rectangle r = GetTileRect(x, y);

            if (tile == TILE_GROUND) {
                DrawRectangleRec(r, (Color){224, 183, 105, 255});
                DrawRectangleLinesEx(r, 1, (Color){166, 116, 48, 255});
            } else if (tile == TILE_WALL) {
                DrawRectangleRec(r, (Color){30, 150, 90, 255});
                DrawRectangleLinesEx(r, 1, DARKGREEN);
            } else if (tile == TILE_BLOCK) {
                DrawRectangleRec(r, (Color){194, 122, 53, 255});
                DrawRectangleLinesEx(r, 1, BROWN);
            } else if (tile == TILE_PLATFORM) {
                DrawRectangleRec(r, (Color){244, 206, 120, 255});
                DrawRectangleLinesEx(r, 1, (Color){166, 116, 48, 255});
            } else if (tile == '?') {
                DrawRectangleRec(r, GOLD);
                DrawRectangleLinesEx(r, 1, BROWN);
                DrawText("?", r.x + 8, r.y + 4, 22, BLACK);
            } else if (tile == TILE_VOID) {
                DrawRectangleRec(r, BLACK);
            } else if (tile == TILE_TUNNEL) {
                DrawRectangleRec(r, (Color){30, 150, 90, 255});
                DrawRectangleLinesEx(r, 2, DARKGREEN);
                DrawRectangle(r.x + 4, r.y + 4, r.width - 8, 6, BLACK);
                DrawText("T", r.x + 8, r.y + 9, 18, WHITE);
            } else if (tile == TILE_DOOR) {
                DrawRectangleRec(r, GOLD);
                DrawRectangleLinesEx(r, 2, BLACK);
                DrawText("Ç", r.x + 5, r.y + 2, 26, BLACK);
            }
        }
    }
}

void FindPlayerStart(int mapIndex, int telaIndex, Rectangle *player) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        const char *linha = GetMapLine(mapIndex, telaIndex, y);

        for (int x = 0; x < MAP_WIDTH; x++) {
            if (linha[x] == TILE_PLAYER) {
                Rectangle r = GetTileRect(x, y);
                player->x = r.x;
                player->y = r.y;
                player->width = TILE_SIZE;
                player->height = TILE_SIZE;
                return;
            }
        }
    }
}

void FindDoor(int mapIndex, int telaIndex, Rectangle *door) {
    door->x = -1000;
    door->y = -1000;
    door->width = TILE_SIZE;
    door->height = TILE_SIZE;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        const char *linha = GetMapLine(mapIndex, telaIndex, y);

        for (int x = 0; x < MAP_WIDTH; x++) {
            if (linha[x] == TILE_DOOR) {
                *door = GetTileRect(x, y);
                return;
            }
        }
    }
}

void FindTunnel(int mapIndex, int telaIndex, Rectangle *tunel) {
    tunel->x = -1000;
    tunel->y = -1000;
    tunel->width = TILE_SIZE;
    tunel->height = TILE_SIZE;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        const char *linha = GetMapLine(mapIndex, telaIndex, y);

        for (int x = 0; x < MAP_WIDTH; x++) {
            if (linha[x] == TILE_TUNNEL) {
                *tunel = GetTileRect(x, y);
                return;
            }
        }
    }
}

int pegarTilePorPixel(float px, float py) {
    int x = (int)(px / TILE_SIZE);
    int y = (int)(py / TILE_SIZE);

    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return TILE_WALL;
    }

    return GetMapLine(mapaAtual, telaMapaAtual, y)[x];
}

int tileEhSolido(int tile) {
    return IsSolidTile((char)tile);
}

void carregarMapa(void) {
}

void desenharMapa(Texture2D chao, Texture2D bloco, Texture2D plataforma) {
    (void)chao;
    (void)bloco;
    (void)plataforma;

    MapTextures textures;
    textures.usarTextura = 0;

    DrawMap(mapaAtual, telaMapaAtual, textures);
}