#include <stdlib.h>
#include "enemy.h"
#include "projectile.h"

static Enemy *listaInimigos = NULL;

Enemy *getListaInimigos(void) {
    return listaInimigos;
}

static void inserirInimigo(float x, float y, TipoInimigo tipo) {
    Enemy *novo = malloc(sizeof(Enemy));
    if (novo == NULL) return;

    novo->x = x;
    novo->y = y;
    novo->velocidade = 70;
    novo->tempoAtaque = 0;
    novo->tempoFantasma = 0;
    novo->tipo = tipo;
    novo->estado = INIMIGO_VIVO;
    novo->direcao = -1;
    novo->frameAtual = (tipo == INIMIGO_BOCA_DE_OURO) ? BOCA_IDLE : CRIANCA_IDLE;
    novo->prox = listaInimigos;

    listaInimigos = novo;
}

void carregarInimigos(void) {
    inserirInimigo(18 * 32, 9 * 32, INIMIGO_CRIANCA_FANTASMA);
    inserirInimigo(60 * 32, 9 * 32, INIMIGO_BOCA_DE_OURO);
}

void inimigoVirarFantasma(Enemy *e) {
    if (e == NULL) return;

    e->estado = INIMIGO_MORRENDO;
    e->tempoFantasma = 0;
    e->frameAtual = (e->tipo == INIMIGO_BOCA_DE_OURO) ? BOCA_FANTASMA : CRIANCA_FANTASMA;
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
        Rectangle origem = {e->frameAtual * ENEMY_LARGURA, 0, ENEMY_LARGURA, ENEMY_ALTURA};
        DrawTextureRec(sprite, origem, (Vector2){e->x, e->y}, WHITE);
    }
}