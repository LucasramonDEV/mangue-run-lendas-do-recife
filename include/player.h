#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

#define PLAYER_LARGURA 32
#define PLAYER_ALTURA 32

#define PLAYER_IDLE_1 0
#define PLAYER_IDLE_2 1
#define PLAYER_IDLE_3 2
#define PLAYER_WALK_R_1 3
#define PLAYER_WALK_R_2 4
#define PLAYER_WALK_R_3 5
#define PLAYER_WALK_L_1 7
#define PLAYER_WALK_L_2 8
#define PLAYER_WALK_L_3 9
#define PLAYER_JUMP 11
#define PLAYER_FALL 12
#define PLAYER_DANO 13
#define PLAYER_FANTASMA_FRAME 14
#define PLAYER_ATTACK_R 15
#define PLAYER_ATTACK_L 16

typedef enum {
    PLAYER_VIVO,
    PLAYER_FANTASMA,
    PLAYER_GAME_OVER
} EstadoPlayer;

typedef enum {
    DIR_ESQUERDA = -1,
    DIR_DIREITA = 1
} Direcao;

typedef struct {
    float x, y;
    float velX, velY;
    int vidas;
    int noChao;
    int frameAtual;
    int animIndex;
    float tempoAnimacao;
    float tempoDano;
    float tempoFantasma;
    float tempoAtaque;
    int tomandoDano;
    int atacando;
    Direcao direcao;
    EstadoPlayer estado;
} Player;

Player criarPlayer(float x, float y);
void atualizarPlayer(Player *p, float dt);
void desenharPlayer(Player p, Texture2D sprite);
void playerTomarDano(Player *p);
Rectangle getPlayerRect(Player p);

#endif