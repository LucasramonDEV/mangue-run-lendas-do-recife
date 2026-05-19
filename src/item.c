#include "item.h"
#include <stdlib.h>

static Heart *listaCoracoes = NULL;

void inserirCoracao(float x, float y) {
    Heart *novo = malloc(sizeof(Heart));
    if (novo == NULL) return;

    novo->rect = (Rectangle){x, y, 32, 32};
    novo->ativo = 1;
    novo->prox = listaCoracoes;
    listaCoracoes = novo;
}

void carregarItens(void) {
    liberarItens();
    inserirCoracao(500, 390);
    inserirCoracao(850, 390);
}

void atualizarItens(Rectangle player, int *vidas) {
    Heart *h = listaCoracoes;

    while (h != NULL) {
        if (h->ativo && CheckCollisionRecs(player, h->rect)) {
            h->ativo = 0;

            if (*vidas < 3) {
                (*vidas)++;
            }
        }

        h = h->prox;
    }
}

void desenharItens(Texture2D heartTexture) {
    Heart *h = listaCoracoes;

    while (h != NULL) {
        if (h->ativo) {
            if (heartTexture.id > 0) {
                DrawTexturePro(
                    heartTexture,
                    (Rectangle){0, 0, heartTexture.width, heartTexture.height},
                    h->rect,
                    (Vector2){0, 0},
                    0,
                    WHITE
                );
            } else {
                DrawCircle(h->rect.x + 16, h->rect.y + 16, 12, RED);
            }
        }

        h = h->prox;
    }
}

void liberarItens(void) {
    Heart *h = listaCoracoes;

    while (h != NULL) {
        Heart *temp = h;
        h = h->prox;
        free(temp);
    }

    listaCoracoes = NULL;
}