#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "player.h"

#define ENEMY_LARGURA 32
#define ENEMY_ALTURA 32

<<<<<<< HEAD
#define HOMEM_SACO_IDLE 0
#define HOMEM_SACO_ATTACK_L 1
#define HOMEM_SACO_ATTACK_R 2

typedef enum {
    INIMIGO_HOMEM_SACO_AREIA
=======
#define BOCA_IDLE 0
#define BOCA_ATTACK_R 1
#define BOCA_ATTACK_L 3
#define BOCA_FANTASMA 4
#define CRIANCA_IDLE 5
#define CRIANCA_ATTACK_L 6
#define CRIANCA_ATTACK_R 7
#define CRIANCA_FANTASMA 8

typedef enum {
    INIMIGO_BOCA_DE_OURO,
    INIMIGO_CRIANCA_FANTASMA
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f
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