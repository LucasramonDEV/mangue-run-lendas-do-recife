#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "player.h"

#define ENEMY_LARGURA 32
#define ENEMY_ALTURA 32

#define HOMEM_SACO_IDLE 0
#define HOMEM_SACO_ATTACK_L 1
#define HOMEM_SACO_ATTACK_R 2

typedef enum {
    INIMIGO_HOMEM_SACO_AREIA
} TipoInimigo;

typedef enum {
    INIMIGO_VIVO,
    INIMIGO_MORRENDO
} EstadoInimigo;

typedef struct Enemy {
    float x, y;
    float velocidade;
    float tempoAtaque;
    float tempoFantasma;
    int frameAtual;
    int direcao;
    TipoInimigo tipo;
    EstadoInimigo estado;
    struct Enemy *prox;
} Enemy;

void carregarInimigos(void);
void atualizarInimigos(Player *player, float dt);
void desenharInimigos(Texture2D sprite);
Enemy *getListaInimigos(void);
void inimigoVirarFantasma(Enemy *e);
void removerInimigo(Enemy *alvo);

#endif