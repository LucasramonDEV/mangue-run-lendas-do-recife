#include "item.h"
#include "player.h"
#include "raylib.h"
#include <stdlib.h>
#include <stddef.h>

static Heart *listaCoracoes = NULL;

static void inserirCoracao(float x, float y) {
    Heart *novo = malloc(sizeof(Heart));

    if (novo == NULL) return;

    novo->x = x;
    novo->y = y;
    novo->ativo = 1;
    novo->prox = listaCoracoes;

    listaCoracoes = novo;
}

void carregarItens(void) {
    inserirCoracao(7 * 32, 9 * 32);
    inserirCoracao(45 * 32, 9 * 32);
}

void atualizarItens(Player *player) {
    Rectangle playerRec = getPlayerRect(*player);

    for (Heart *h = listaCoracoes; h != NULL; h = h->prox) {
        if (!h->ativo) continue;

        Rectangle heartRec = {h->x, h->y, 32, 32};

        if (CheckCollisionRecs(playerRec, heartRec)) {
            if (player->vidas < 3) {
                player->vidas++;
            }

            h->ativo = 0;
        }
    }
}

void desenharItens(Texture2D heartTexture) {
    for (Heart *h = listaCoracoes; h != NULL; h = h->prox) {
        if (!h->ativo) continue;

        Rectangle origem = {0, 0, 32, 32};
        DrawTextureRec(heartTexture, origem, (Vector2){h->x, h->y}, WHITE);
    }
}