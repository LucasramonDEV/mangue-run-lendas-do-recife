#include "map.h"

int mapaAtual = 0;
int telaMapaAtual = 0;

static const char *mapa1[TELAS_POR_MAPA][MAP_HEIGHT]={
{
"                                        ",
"                                        ",
"          T   BBBB          M       T   ",
"    B  BBBBB         BBBBBBBBBBBB  BB   ",
"                                   BB   ",
"               M                   BB   ",
" BBBB     T  BBBB                  BB   ",
"       BBBBB                 T     BB   ",
"                     BBBBBBBBBBB   BB   ",
"                                   BB   ",
"                                   BB   ",
"    U                       T    BBBB   ",
"   BB         BB           BBBB    BB   ",
" P BB      M  BB           M     T BB  U",
"=====OO=====OO==OOOOO====================",
"                                        ",
"                                        ",
"                                        "
},
{
"                                        ",
"                                        ",
"                    T              T    ",
"                    BB   BBB      BB    ",
"                                 BBB    ",
"                                  BB    ",
"                                  BB    ",
"                                  BB    ",
"                T         T    BBBBB    ",
"          BB   BB   BB   BB       BB    ",
"      U                           BB    ",
"      BB                          BB    ",
"    BBBB                          BB    ",
" PBBBBBB                          BB U  ",
"========OOOOOOOOOOOOOOOOOOOOOOOOOO======",
"                                        ",
"                                        ",
"                                        "
},
{
"                 B                      ",
"                 B                      ",
"                 B                      ",
"          M      BBB                    ",
"BBT     BBBB      TB                    ",
"BBBBB     BB    BBBB                    ",
"BB        BB     T   B                  ",
"BB       TBBBB   B   B                  ",
"BB      BBBB         TB        B        ",
"BB        BB    BB   B       BB         ",
"BBT       BB     B   T      BBB         ",
"BBBB      BBBB       B     BBBB         ",
"          BB              BBBBB         ",
" P  U    M BB M        T  BBBBBB M   U ",
"========================================",
"                                        ",
"                                        ",
"                                        "
},
{
"                                        ",
"                                        ",
"                                        ",
"                                        ",
"                                        ",
"                                        ",
"                        T               ",
"                      BBBBB        M    ",
"                          B     BBBBB   ",
"                          B         B   ",
"         M     T     M    B         B   ",
"       BBBB   BBB   BBBB  BBBB      B   ",
"       BB             BB         BBBB   ",
" P  M  BB M    H    M BB    M       B U ",
"========================================",
"                                        ",
"                                        ",
"                                        "
},
{
"                                        ",
"                                        ",
"                                        ",
"                            T           ",
"         M   T      M       BB          ",
"    BBBBBBBBBBBBBBBBBBBBB   BB          ",
"  T           T             BB          ",
" BB           BBB           BB          ",
"                           BBB          ",
" T        T               MBBB          ",
"BB       BBB           MBBBB            ",
"  T                 MBBBBB              ",
"  BB              MBBBB                 ",
" P       M       BBB   T             Q  ",
"==============================OOOO======",
"                                        ",
"                                        ",
"                                        "
}
};

static const char *mapa2[TELAS_POR_MAPA][MAP_HEIGHT] = {
    {
        "                                      B ",
        "                                      B ",
        "                      M  H   M        B ",
        "          T       BBBBBBBBBBBBB       B ",
        "          BBBB    B              M    B ",
        "                  B         M   BBBBBBB ",
        "               M  B BBBBBBBBBBBBBBB B   ",
        "             BBBBB                  B   ",
        "               BBBH     M   H       B   ",
        "     M         BBBBBBBBBBBBBBBBBBB  B   ",
        "   BBBBB       BB                   B   ",
        "             BB          M    M     B   ",
        "             BB BBBBBBBBBBBBBBBBBBBBBBBB",
        " P          BB                 M      U ",
        "========================================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "B                                      B",
        "B      T      M         M            TBB",
        "B  BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB   BB  ",
        "B                               B    B  ",
        "B                               B    B  ",
        "B           T               U   B    B  ",
        "BBBBBBBBBBBBBBBBBBBBBOBBBBBBBBBBBB   B  ",
        "                                B    B  ",
        "                                B    B  ",
        "                                B    B  ",
        "                    T           B   BB  ",
        "       B    B       B           B    B  ",
        "      BB    BB                  B    B  ",
        "     BBB    BBB                 BB   B  ",
        "    BBBBOOOOBBBB                B   B   ",
        " P BBBBBOOOOBBBBOB   M              BB  ",
        "========================================",
        "                                        "
    },
    {
        "                  B   U                 ",
        "                  BBBBBBBB              ",
        "                           B            ",
        "                                        ",
        "           M    T   H    M    T         ",
        "      BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB    ",
        "                                        ",
        "  T                                     ",
        " BBBBB                                  ",
        "                                        ",
        "          M    T     H       M          ",
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB    BBBB",
        "                                   BBBBB",
        " P                                BBBBBB",
        "========================================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        " B                           B          ",
        " B                           B          ",
        " B    H   M     T      M     B          ",
        " B   BBBBBBBBBBBBBBBBBBBB    B          ",
        " B   B                  B    B          ",
        " B   B                  B B  B          ",
        " B  H    M    T    M     B   B          ",
        " BBBBBBBBBBBBBBBBBBBBB  B    B          ",
        " B                      B  BBB          ",
        " B                      B    B          ",
        " B        U     M       B    B          ",
        " BBBBBBBBBBBBBBBBBBBBBBBBB   B          ",
        "                             B          ",
        " P                           B          ",
        "========================================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                BBBBBBB                 ",
        "               BB                       ",
        "              BB                        ",
        "             BB                         ",
        "            BB                          ",
        "           BB                           ",
        "          BB                            ",
        "         BB                             ",
        "        BB                              ",
        "       BBB                  Q           ",
        " P    BBBB                  B           ",
        "==========000000000000000000000000000000",
        "                                        ",
        "                                        ",
        "                                        ",
        "                                        "
    }
};

static const char *mapa3[TELAS_POR_MAPA][MAP_HEIGHT] = {
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "        T              M                ",
        "      BBBB           BBBB        T      ",
        "                              BBBB      ",
        "                H                       ",
        "              BBBB                      ",
        "                       T                ",
        "                     BBBB       M       ",
        "       M                      BBBB      ",
        "     BBBB       BB                 U    ",
        "                BB                BB    ",
        " P        M     BB      M         BB    ",
        "======OOO=======BB==OOOO================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                     T                  ",
        "                   BBBB        M        ",
        "       H                      BBBB      ",
        "     BBBB                               ",
        "               T                        ",
        "             BBBB                       ",
        "                         M              ",
        "         BB            BBBB             ",
        "    U    BB                         U   ",
        "   BB    BB       T              BBBB   ",
        " P BB M          BBBB     M       BB    ",
        "=======OOOO====OOO======================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "          T                  H          ",
        "        BBBB               BBBB         ",
        "                   M                    ",
        "                 BBBB        T          ",
        "                           BBBB         ",
        "     M                                  ",
        "   BBBB             BB                  ",
        "             T      BB       M          ",
        "   U       BBBB     BB     BBBB     U   ",
        "  BB                BB             BB   ",
        " PBB    M                 M        BB   ",
        "======OOO============OOOO===============",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                 T                      ",
        "               BBBB          T          ",
        "       M                   BBBB         ",
        "     BBBB                               ",
        "                   H                    ",
        "                 BBBB                   ",
        "                           M            ",
        "         BB              BBBB           ",
        "   U     BB        T                U   ",
        "  BB     BB      BBBB             BBB   ",
        " PBB M             M              BB    ",
        "=======OOO====OOOO======================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                       T                ",
        "                     BBBB               ",
        "       M                         H      ",
        "     BBBB                      BBBB     ",
        "              T                         ",
        "            BBBB         M              ",
        "                       BBBB             ",
        "    M                                   ",
        "  BBBB       BB                         ",
        "             BB          T          Q   ",
        " P     M     BB        BBBB        BB   ",
        "=========OOO=BB===OOOO==================",
        "                                        ",
        "                                        ",
        "                                        "
    }
};

static const char *mapa4[TELAS_POR_MAPA][MAP_HEIGHT] = {
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "          T              T              ",
        "        BBBB          BBBB              ",
        "               M                        ",
        "             BBBB              H        ",
        "                             BBBB       ",
        "      M                                 ",
        "    BBBB          T                     ",
        "                 BBBB          M        ",
        "   U                         BBBB       ",
        "  BB        BB                       U  ",
        " PBB    M   BB        M           BBB   ",
        "=======OOO==BB===OOOO===================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                   T                    ",
        "                 BBBB              T    ",
        "                              BBBB      ",
        "        H                               ",
        "      BBBB         M                    ",
        "                BBBB                    ",
        "                          T             ",
        "         BB             BBBB            ",
        "   U     BB       M                 U   ",
        "  BB     BB     BBBB             BBBB   ",
        " PBB M              M             BB    ",
        "======OOOO====OOO=======================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "       T                    H           ",
        "     BBBB                 BBBB          ",
        "                 M                      ",
        "               BBBB        T            ",
        "                         BBBB           ",
        "    M                                   ",
        "  BBBB       BB                         ",
        "             BB      M                  ",
        "   U         BB    BBBB             U   ",
        "  BB    T    BB                 BBBB    ",
        " PBB  BBBB        M              BB     ",
        "========OOO====OOOO=====================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "              T                         ",
        "            BBBB             M          ",
        "                          BBBB          ",
        "       H                                ",
        "     BBBB        T                      ",
        "               BBBB                     ",
        "                         M              ",
        "      T                BBBB             ",
        "    BBBB                         U      ",
        "   U         BB                BBBB     ",
        " P BB   M    BB       M         BB      ",
        "======OOO====BB===OOOO==================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                        T               ",
        "                      BBBB              ",
        "       M                       H        ",
        "     BBBB                    BBBB       ",
        "              T                         ",
        "            BBBB       M                ",
        "                     BBBB               ",
        "   M                                    ",
        " BBBB        BB                         ",
        "             BB          T          Q   ",
        " P      M    BB        BBBB       BBB   ",
        "=========OOO=BB==OOOO===================",
        "                                        ",
        "                                        ",
        "                                        "
    }
};

static const char *mapa5[TELAS_POR_MAPA][MAP_HEIGHT] = {
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "        T                  M            ",
        "      BBBB               BBBB      T    ",
        "                               BBBB     ",
        "                H                       ",
        "              BBBB                      ",
        "                        T               ",
        "                      BBBB      M       ",
        "     M                         BBBB     ",
        "   BBBB        BB                 U     ",
        "               BB                BB     ",
        " P       M     BB       M        BB     ",
        "======OOO======BB==OOOO=================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                 T                      ",
        "               BBBB          M          ",
        "                            BBBB        ",
        "       H                                ",
        "     BBBB             T                 ",
        "                    BBBB                ",
        "                         M              ",
        "        BB             BBBB             ",
        "   U    BB                         U    ",
        "  BB    BB       T              BBBB    ",
        " PBB M          BBBB     M       BB     ",
        "=======OOOO====OOO======================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "           T                H           ",
        "         BBBB             BBBB          ",
        "                     M                  ",
        "                   BBBB      T          ",
        "                           BBBB         ",
        "      M                                 ",
        "    BBBB           BB                   ",
        "              T    BB        M          ",
        "   U        BBBB   BB      BBBB     U   ",
        "  BB               BB              BB   ",
        " PBB    M                 M        BB   ",
        "======OOO=============OOOO==============",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                    T                   ",
        "                  BBBB       T          ",
        "       M                    BBBB        ",
        "     BBBB                               ",
        "                  H                     ",
        "                BBBB                    ",
        "                           M            ",
        "         BB              BBBB           ",
        "   U     BB        T                U   ",
        "  BB     BB      BBBB             BBB   ",
        " PBB M             M              BB    ",
        "=======OOO====OOOO======================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                       T                ",
        "                     BBBB               ",
        "      M                         H       ",
        "    BBBB                       BBBB     ",
        "             T                          ",
        "           BBBB          M              ",
        "                       BBBB             ",
        "    M                                   ",
        "  BBBB       BB                         ",
        "             BB          T          Q   ",
        " P     M     BB        BBBB       BBB   ",
        "=========OOO=BB===OOOO==================",
        "                                        ",
        "                                        ",
        "                                        "
    }
};

static const char *mapa6[TELAS_POR_MAPA][MAP_HEIGHT] = {
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "          T            M       T        ",
        "        BBBB         BBBB    BBBB       ",
        "                 H                      ",
        "               BBBB                     ",
        "                         T              ",
        "                       BBBB       M     ",
        "      M                        BBBB     ",
        "    BBBB       BB                       ",
        "   U           BB        T          U   ",
        "  BB           BB      BBBB       BBB   ",
        " PBB     M             M          BB    ",
        "======OOO=======OOOO====================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                  T             T       ",
        "                BBBB          BBBB      ",
        "       H                                ",
        "     BBBB             M                 ",
        "                    BBBB                ",
        "                          T             ",
        "                        BBBB      M     ",
        "        BB                      BBBB    ",
        "   U    BB       T                 U    ",
        "  BB    BB     BBBB             BBBB    ",
        " PBB M               M            BB    ",
        "=======OOOO====OOO======================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "       T                M        H      ",
        "     BBBB             BBBB     BBBB     ",
        "               T                        ",
        "             BBBB                       ",
        "                         T              ",
        "                       BBBB             ",
        "     M            BB                    ",
        "   BBBB           BB       M            ",
        "   U              BB     BBBB       U   ",
        "  BB     T        BB              BBB   ",
        " PBB   BBBB            M          BB    ",
        "========OOO===OOOO======================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                T                       ",
        "              BBBB              T       ",
        "                           BBBB         ",
        "      H                                 ",
        "    BBBB        M                       ",
        "              BBBB        M             ",
        "                       BBBB             ",
        "      T                                U",
        "    BBBB        BB              BBBB    ",
        "   U            BB              BB      ",
        " P BB    M      BB      M       BB      ",
        "======OOO=======BB==OOOO================",
        "                                        ",
        "                                        ",
        "                                        "
    },
    {
        "                                        ",
        "                                        ",
        "                                        ",
        "                         T              ",
        "                       BBBB             ",
        "       M                       H        ",
        "     BBBB                    BBBB       ",
        "              T                         ",
        "            BBBB       M                ",
        "                     BBBB       T       ",
        "   M                            BBBB    ",
        " BBBB        BB                         ",
        "             BB                     Q   ",
        " P      M    BB        M         BBBB   ",
        "=========OOO=BB==OOOO===================",
        "                                        ",
        "                                        ",
        "                                        "
    }

};

const char *GetMapLine(int mapIndex,int telaIndex,int y){
    if(telaIndex<0)telaIndex=0;
    if(telaIndex>=TELAS_POR_MAPA)telaIndex=TELAS_POR_MAPA-1;

    if(mapIndex==0)return mapa1[telaIndex][y];
    if(mapIndex==1)return mapa2[telaIndex][y];
    if(mapIndex==2)return mapa3[telaIndex][y];
    if(mapIndex==3)return mapa4[telaIndex][y];
    if(mapIndex==4)return mapa5[telaIndex][y];
    return mapa6[telaIndex][y];
}

void LoadMapTextures(MapTextures *textures,int mapIndex){
    Image img;
    Color corChao=(Color){224,183,105,255};
    Color corBloco=(Color){194,122,53,255};
    Color corPlataforma=(Color){244,206,120,255};

    if(textures->usarTextura){
        UnloadTexture(textures->chao);
        UnloadTexture(textures->bloco);
        UnloadTexture(textures->plataforma);
    }

    if(mapIndex==0){
        if(telaMapaAtual==0){
            corChao=(Color){224,183,105,255};
            corBloco=(Color){194,122,53,255};
            corPlataforma=(Color){244,206,120,255};
        }else{
            corChao=GRAY;
            corBloco=DARKGRAY;
            corPlataforma=LIGHTGRAY;
        }
    }else if(mapIndex==1){
        if(telaMapaAtual<=2){
            corChao=GREEN;
            corBloco=DARKGREEN;
            corPlataforma=LIME;
        }else{
            corChao=GRAY;
            corBloco=DARKGRAY;
            corPlataforma=LIGHTGRAY;
        }
    }else if(mapIndex==2){
        corChao=GRAY;
        corBloco=DARKGRAY;
        corPlataforma=LIGHTGRAY;
    }else if(mapIndex==3){
        corChao=GRAY;
        corBloco=DARKGRAY;
        corPlataforma=LIGHTGRAY;
    }else if(mapIndex==4){
        corChao=DARKBROWN;
        corBloco=BROWN;
        corPlataforma=MAROON;
    }else{
        corChao=LIGHTGRAY;
        corBloco=GRAY;
        corPlataforma=DARKGRAY;
    }

    img=GenImageColor(TILE_SIZE,TILE_SIZE,corChao);
    textures->chao=LoadTextureFromImage(img);
    UnloadImage(img);

    img=GenImageColor(TILE_SIZE,TILE_SIZE,corBloco);
    textures->bloco=LoadTextureFromImage(img);
    UnloadImage(img);

    img=GenImageColor(TILE_SIZE,TILE_SIZE,corPlataforma);
    textures->plataforma=LoadTextureFromImage(img);
    UnloadImage(img);

    textures->usarTextura=1;
}

void UnloadMapTextures(MapTextures *textures){
    if(textures->usarTextura){
        UnloadTexture(textures->chao);
        UnloadTexture(textures->bloco);
        UnloadTexture(textures->plataforma);
    }
    textures->usarTextura=0;
}

Rectangle GetTileRect(int x,int y){
    return (Rectangle){x*TILE_SIZE,y*TILE_SIZE,TILE_SIZE,TILE_SIZE};
}

int IsSolidTile(char tile){
    return tile==TILE_GROUND||
           tile==TILE_WALL||
           tile==TILE_PLATFORM||
           tile==TILE_BLOCK||
           tile=='?';
}

int IsDeadlyTile(char tile){
    return tile==TILE_VOID;
}

void DrawMap(int mapIndex,int telaIndex,MapTextures textures){
    for(int y=0;y<MAP_HEIGHT;y++){
        const char *linha=GetMapLine(mapIndex,telaIndex,y);

        for(int x=0;x<MAP_WIDTH;x++){
            char tile=linha[x];
            Rectangle r=GetTileRect(x,y);

            if(tile==TILE_GROUND){
                DrawTexturePro(textures.chao,(Rectangle){0,0,textures.chao.width,textures.chao.height},r,(Vector2){0,0},0,WHITE);
                DrawRectangleLinesEx(r,1,BLACK);
            }else if(tile==TILE_WALL||tile==TILE_BLOCK){
                DrawTexturePro(textures.bloco,(Rectangle){0,0,textures.bloco.width,textures.bloco.height},r,(Vector2){0,0},0,WHITE);
                DrawRectangleLinesEx(r,1,BLACK);
            }else if(tile==TILE_PLATFORM){
                DrawTexturePro(textures.plataforma,(Rectangle){0,0,textures.plataforma.width,textures.plataforma.height},r,(Vector2){0,0},0,WHITE);
                DrawRectangleLinesEx(r,1,BLACK);
            }else if(tile==TILE_VOID){
                DrawRectangleRec(r,BLACK);
            }else if(tile==TILE_TUNNEL){
                DrawRectangleRec(r,(Color){30,150,90,255});
                DrawRectangleLinesEx(r,2,DARKGREEN);
                DrawRectangle(r.x+4,r.y+4,r.width-8,6,BLACK);
                DrawText("T",r.x+8,r.y+9,18,WHITE);
            }else if(tile==TILE_DOOR){
                DrawRectangleRec(r,GOLD);
                DrawRectangleLinesEx(r,2,BLACK);
                DrawText("Ç",r.x+5,r.y+2,26,BLACK);
            }
        }
    }
}

void FindPlayerStart(int mapIndex,int telaIndex,Rectangle *player){
    for(int y=0;y<MAP_HEIGHT;y++){
        const char *linha=GetMapLine(mapIndex,telaIndex,y);
        for(int x=0;x<MAP_WIDTH;x++){
            if(linha[x]==TILE_PLAYER){
                Rectangle r=GetTileRect(x,y);
                player->x=r.x;
                player->y=r.y;
                player->width=TILE_SIZE;
                player->height=TILE_SIZE;
                return;
            }
        }
    }
}

void FindDoor(int mapIndex,int telaIndex,Rectangle *door){
    door->x=-1000;
    door->y=-1000;
    door->width=TILE_SIZE;
    door->height=TILE_SIZE;

    for(int y=0;y<MAP_HEIGHT;y++){
        const char *linha=GetMapLine(mapIndex,telaIndex,y);
        for(int x=0;x<MAP_WIDTH;x++){
            if(linha[x]==TILE_DOOR){
                *door=GetTileRect(x,y);
                return;
            }
        }
    }
}

void FindTunnel(int mapIndex,int telaIndex,Rectangle *tunel){
    tunel->x=-1000;
    tunel->y=-1000;
    tunel->width=TILE_SIZE;
    tunel->height=TILE_SIZE;

    for(int y=0;y<MAP_HEIGHT;y++){
        const char *linha=GetMapLine(mapIndex,telaIndex,y);
        for(int x=0;x<MAP_WIDTH;x++){
            if(linha[x]==TILE_TUNNEL){
                *tunel=GetTileRect(x,y);
                return;
            }
        }
    }
}

int pegarTilePorPixel(float px,float py){
    int x=(int)(px/TILE_SIZE);
    int y=(int)(py/TILE_SIZE);

    if(x<0||x>=MAP_WIDTH||y<0||y>=MAP_HEIGHT){
        return TILE_WALL;
    }

    return GetMapLine(mapaAtual,telaMapaAtual,y)[x];
}

int tileEhSolido(int tile){
    return IsSolidTile((char)tile);
}

void carregarMapa(void){
}

void desenharMapa(Texture2D chao,Texture2D bloco,Texture2D plataforma){
    (void)chao;
    (void)bloco;
    (void)plataforma;

    MapTextures textures;
    textures.usarTextura=0;

    DrawMap(mapaAtual,telaMapaAtual,textures);
}