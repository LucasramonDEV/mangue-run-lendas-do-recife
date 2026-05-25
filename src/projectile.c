#include "projectile.h"
#include "map.h"
#include <stdlib.h>

#define PROJECTILE_SPEED 420.0f
#define ENEMY_PROJECTILE_SPEED 260.0f
#define PROJECTILE_WIDTH 16.0f
#define PROJECTILE_HEIGHT 8.0f

static Projectile *listaProjeteis = NULL;

static Projectile *criarNoProjetil(float x, float y, int direcao, TipoProjetil tipo) {
    Projectile *novo = malloc(sizeof(Projectile));

    if (novo == NULL) return NULL;

    novo->x = x;
    novo->y = y;
    novo->tipo = tipo;
    novo->velX = (tipo == PROJETIL_PERSONAGEM ? PROJECTILE_SPEED : ENEMY_PROJECTILE_SPEED) * direcao;
    novo->prox = NULL;

    return novo;
}

void dispararProjetil(float x, float y, int direcao, TipoProjetil tipo) {
    Projectile *novo = criarNoProjetil(x, y, direcao, tipo);

    if (novo == NULL) return;

    if (listaProjeteis == NULL) {
        listaProjeteis = novo;
        return;
    }

    Projectile *aux = listaProjeteis;

    while (aux->prox != NULL) {
        aux = aux->prox;
    }

    aux->prox = novo;
}

static int projetilSaiuDaTela(Projectile *p) {
    if (p->x < -100.0f) return 1;
    if (p->x > MAP_WIDTH * TILE_SIZE + 100.0f) return 1;
    return 0;
}

void atualizarProjeteis(float dt) {
    Projectile *atual = listaProjeteis;
    Projectile *anterior = NULL;

    while (atual != NULL) {
        atual->x += atual->velX * dt;

        if (projetilSaiuDaTela(atual)) {
            Projectile *removido = atual;

            if (anterior == NULL) {
                listaProjeteis = atual->prox;
                atual = listaProjeteis;
            } else {
                anterior->prox = atual->prox;
                atual = atual->prox;
            }

            free(removido);
        } else {
            anterior = atual;
            atual = atual->prox;
        }
    }
}

static void desenharProjetilPersonagem(Projectile *p, Texture2D notaPersonagem) {
    Rectangle dst = {p->x, p->y, PROJECTILE_WIDTH, PROJECTILE_HEIGHT};

    if (notaPersonagem.id > 0) {
        Rectangle src = {0, 0, notaPersonagem.width, notaPersonagem.height};
        DrawTexturePro(notaPersonagem, src, dst, (Vector2){0, 0}, 0, WHITE);
    } else {
        DrawRectangleRec(dst, YELLOW);
        DrawRectangleLinesEx(dst, 1, ORANGE);
    }
}

static void desenharProjetilInimigo(Projectile *p, Texture2D tiroInimigo) {
    Rectangle dst = {p->x, p->y, PROJECTILE_WIDTH, PROJECTILE_HEIGHT};

    if (tiroInimigo.id > 0) {
        Rectangle src = {0, 0, tiroInimigo.width, tiroInimigo.height};
        DrawTexturePro(tiroInimigo, src, dst, (Vector2){0, 0}, 0, WHITE);
    } else {
        DrawRectangleRec(dst, RED);
        DrawRectangleLinesEx(dst, 1, MAROON);
    }
}

void desenharProjeteis(Texture2D notaPersonagem, Texture2D tiroInimigo) {
    Projectile *aux = listaProjeteis;

    while (aux != NULL) {
        if (aux->tipo == PROJETIL_PERSONAGEM) {
            desenharProjetilPersonagem(aux, notaPersonagem);
        } else {
            desenharProjetilInimigo(aux, tiroInimigo);
        }

        aux = aux->prox;
    }
}

Projectile *getListaProjeteis(void) {
    return listaProjeteis;
}

void removerProjetil(Projectile *alvo) {
    if (alvo == NULL) return;

    Projectile *atual = listaProjeteis;
    Projectile *anterior = NULL;

    while (atual != NULL) {
        if (atual == alvo) {
            if (anterior == NULL) {
                listaProjeteis = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            free(atual);
            return;
        }

        anterior = atual;
        atual = atual->prox;
    }
}
