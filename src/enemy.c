#include <stdlib.h>
#include "enemy.h"

static Enemy *listaInimigos = NULL;

Enemy *getListaInimigos(void) {
    return listaInimigos;
}

static void inserirInimigo(float x, float y, TipoInimigo tipo) {
    Enemy *novo = malloc(sizeof(Enemy));
    if (novo == NULL) return;

    novo->x = x;
    novo->y = y;
    novo->velocidade = 0;
    novo->tempoAtaque = 0;
    novo->tempoFantasma = 0;
    novo->tipo = tipo;
    novo->estado = INIMIGO_VIVO;
    novo->direcao = -1;
    novo->frameAtual = HOMEM_SACO_IDLE;
    novo->prox = listaInimigos;

    listaInimigos = novo;
}

void carregarInimigos(void) {
    inserirInimigo(35 * 32, 320, INIMIGO_HOMEM_SACO_AREIA);
}

void inimigoVirarFantasma(Enemy *e) {
    if (e == NULL) return;

    e->estado = INIMIGO_MORRENDO;
    e->tempoFantasma = 0;
    e->frameAtual = HOMEM_SACO_IDLE;
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

        if (player->x < e->x) {
            e->direcao = -1;
            e->frameAtual = HOMEM_SACO_ATTACK_L;
        } else {
            e->direcao = 1;
            e->frameAtual = HOMEM_SACO_ATTACK_R;
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
        Rectangle origem = {
            e->frameAtual * ENEMY_LARGURA,
            0,
            ENEMY_LARGURA,
            ENEMY_ALTURA
        };

        DrawTextureRec(sprite, origem, (Vector2){e->x, e->y}, WHITE);
    }
}