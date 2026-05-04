#include <stdlib.h>
#include "projectile.h"
#include "map.h"

static Projectile *listaProjeteis = NULL;

Projectile *getListaProjeteis(void) {
    return listaProjeteis;
}

void dispararProjetil(float x, float y, int direcao, TipoProjetil tipo) {
    Projectile *novo = malloc(sizeof(Projectile));
    if (novo == NULL) return;

    novo->x = x;
    novo->y = y;
    novo->velX = (tipo == PROJETIL_PERSONAGEM ? 330 : 220) * direcao;
    novo->tipo = tipo;
    novo->prox = listaProjeteis;

    listaProjeteis = novo;
}

void removerProjetil(Projectile *alvo) {
    if (alvo == NULL || listaProjeteis == NULL) return;

    if (listaProjeteis == alvo) {
        listaProjeteis = alvo->prox;
        free(alvo);
        return;
    }

    Projectile *ant = listaProjeteis;

    while (ant->prox != NULL && ant->prox != alvo) {
        ant = ant->prox;
    }

    if (ant->prox == alvo) {
        ant->prox = alvo->prox;
        free(alvo);
    }
}

void atualizarProjeteis(float dt) {
    Projectile *p = listaProjeteis;
    Projectile *prox;

    while (p != NULL) {
        prox = p->prox;
        p->x += p->velX * dt;

        if (p->x < -100 || p->x > MAP_COLUNAS * TILE + 100) {
            removerProjetil(p);
        }

        p = prox;
    }
}

void desenharProjeteis(Texture2D notaPersonagem, Texture2D tiroInimigo) {
    for (Projectile *p = listaProjeteis; p != NULL; p = p->prox) {
        Texture2D sprite = (p->tipo == PROJETIL_PERSONAGEM) ? notaPersonagem : tiroInimigo;
        Rectangle origem = {0, 0, 16, 16};
        DrawTextureRec(sprite, origem, (Vector2){p->x, p->y}, WHITE);
    }
}