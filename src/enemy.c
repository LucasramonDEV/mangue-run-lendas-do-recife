#include <stdlib.h>
#include "enemy.h"
<<<<<<< HEAD
=======
#include "projectile.h"
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f

static Enemy *listaInimigos = NULL;

Enemy *getListaInimigos(void) {
    return listaInimigos;
}

static void inserirInimigo(float x, float y, TipoInimigo tipo) {
    Enemy *novo = malloc(sizeof(Enemy));
    if (novo == NULL) return;

    novo->x = x;
    novo->y = y;
<<<<<<< HEAD
    novo->velocidade = 0;
=======
    novo->velocidade = 70;
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f
    novo->tempoAtaque = 0;
    novo->tempoFantasma = 0;
    novo->tipo = tipo;
    novo->estado = INIMIGO_VIVO;
    novo->direcao = -1;
<<<<<<< HEAD
    novo->frameAtual = HOMEM_SACO_IDLE;
=======
    novo->frameAtual = (tipo == INIMIGO_BOCA_DE_OURO) ? BOCA_IDLE : CRIANCA_IDLE;
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f
    novo->prox = listaInimigos;

    listaInimigos = novo;
}

void carregarInimigos(void) {
<<<<<<< HEAD
    inserirInimigo(35 * 32, 320, INIMIGO_HOMEM_SACO_AREIA);
=======
    inserirInimigo(18 * 32, 9 * 32, INIMIGO_CRIANCA_FANTASMA);
    inserirInimigo(60 * 32, 9 * 32, INIMIGO_BOCA_DE_OURO);
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f
}

void inimigoVirarFantasma(Enemy *e) {
    if (e == NULL) return;

    e->estado = INIMIGO_MORRENDO;
    e->tempoFantasma = 0;
<<<<<<< HEAD
    e->frameAtual = HOMEM_SACO_IDLE;
=======
    e->frameAtual = (e->tipo == INIMIGO_BOCA_DE_OURO) ? BOCA_FANTASMA : CRIANCA_FANTASMA;
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f
}

void removerInimigo(Enemy *alvo) {
    if (alvo == NULL || listaInimigos == NULL) return;

    if (listaInimigos == alvo) {
        listaInimigos = alvo->prox;
        free(alvo);
        return;
    }

    Enemy *ant = listaInimigos;

    while (ant->prox != NULL && ant->prox != alvo) {
        ant = ant->prox;
    }

    if (ant->prox == alvo) {
        ant->prox = alvo->prox;
        free(alvo);
    }
}

void atualizarInimigos(Player *player, float dt) {
    Enemy *e = listaInimigos;
    Enemy *prox;

    while (e != NULL) {
        prox = e->prox;

        if (e->estado == INIMIGO_MORRENDO) {
            e->tempoFantasma += dt;

            if (e->tempoFantasma >= 2.0f) {
                removerInimigo(e);
            }

            e = prox;
            continue;
        }

<<<<<<< HEAD
        if (player->x < e->x) {
            e->direcao = -1;
            e->frameAtual = HOMEM_SACO_ATTACK_L;
        } else {
            e->direcao = 1;
            e->frameAtual = HOMEM_SACO_ATTACK_R;
=======
        if (e->tipo == INIMIGO_CRIANCA_FANTASMA) {
            if (player->x < e->x) {
                e->direcao = -1;
                e->frameAtual = CRIANCA_ATTACK_L;
            } else {
                e->direcao = 1;
                e->frameAtual = CRIANCA_ATTACK_R;
            }

            e->x += e->velocidade * e->direcao * dt;
        }

        if (e->tipo == INIMIGO_BOCA_DE_OURO) {
            e->tempoAtaque += dt;

            if (player->x < e->x) {
                e->direcao = -1;
                e->frameAtual = BOCA_ATTACK_L;
            } else {
                e->direcao = 1;
                e->frameAtual = BOCA_ATTACK_R;
            }

            if (e->tempoAtaque >= 2.0f) {
                dispararProjetil(e->x + 16, e->y + 14, e->direcao, PROJETIL_INIMIGO);
                e->tempoAtaque = 0;
            }
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f
        }

        Rectangle recPlayer = getPlayerRect(*player);
        Rectangle recEnemy = {e->x, e->y, 32, 32};

        if (CheckCollisionRecs(recPlayer, recEnemy)) {
            playerTomarDano(player);
        }

        e = prox;
    }
}

void desenharInimigos(Texture2D sprite) {
    for (Enemy *e = listaInimigos; e != NULL; e = e->prox) {
<<<<<<< HEAD
        Rectangle origem = {
            e->frameAtual * ENEMY_LARGURA,
            0,
            ENEMY_LARGURA,
            ENEMY_ALTURA
        };

=======
        Rectangle origem = {e->frameAtual * ENEMY_LARGURA, 0, ENEMY_LARGURA, ENEMY_ALTURA};
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f
        DrawTextureRec(sprite, origem, (Vector2){e->x, e->y}, WHITE);
    }
}