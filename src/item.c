<<<<<<< HEAD
#include "item.h"
#include "player.h"
#include "raylib.h"
#include <stdlib.h>
#include <stddef.h>
=======
#include <stdlib.h>
#include "item.h"
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f

static Heart *listaCoracoes = NULL;

static void inserirCoracao(float x, float y) {
    Heart *novo = malloc(sizeof(Heart));
<<<<<<< HEAD

=======
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f
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
<<<<<<< HEAD
            if (player->vidas < 3) {
                player->vidas++;
            }

=======
            if (player->vidas < 3) player->vidas++;
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f
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