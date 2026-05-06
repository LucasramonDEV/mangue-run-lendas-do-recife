#include "player.h"
#include "map.h"

Player criarPlayer(float x, float y) {
    Player p = {0};

    p.x = x;
    p.y = y;
    p.vidas = 3;
    p.direcao = DIR_DIREITA;
    p.estado = PLAYER_VIVO;
    p.frameAtual = PLAYER_IDLE_1;

    return p;
}

Rectangle getPlayerRect(Player p) {
    return (Rectangle){p.x + 5, p.y + 3, 22, 29};
}

static void resolverColisaoVertical(Player *p) {
    Rectangle r = getPlayerRect(*p);

    int tileBaixoEsq = pegarTilePorPixel(r.x, r.y + r.height);
    int tileBaixoDir = pegarTilePorPixel(r.x + r.width, r.y + r.height);

    if (p->velY >= 0 && (tileEhSolido(tileBaixoEsq) || tileEhSolido(tileBaixoDir))) {
        int linha = (int)((r.y + r.height) / TILE);
        p->y = linha * TILE - PLAYER_ALTURA;
        p->velY = 0;
        p->noChao = 1;
    } else {
        p->noChao = 0;
    }
}

static void atualizarAnimacao(Player *p, float dt) {
    p->tempoAnimacao += dt;

    if (p->tomandoDano) {
        p->frameAtual = PLAYER_DANO;
        p->tempoDano += dt;

        if (p->tempoDano >= 0.15f) {
            p->tomandoDano = 0;
            p->tempoDano = 0;
        }

        return;
    }

    if (p->atacando) {
        p->tempoAtaque += dt;
        p->frameAtual = (p->direcao == DIR_DIREITA) ? PLAYER_ATTACK_R : PLAYER_ATTACK_L;

        if (p->tempoAtaque >= 0.20f) {
            p->atacando = 0;
            p->tempoAtaque = 0;
        }

        return;
    }

    if (!p->noChao) {
        p->frameAtual = (p->velY < 0) ? PLAYER_JUMP : PLAYER_FALL;
        return;
    }

    if (p->velX > 0) {
        if (p->tempoAnimacao >= 0.12f) {
            p->animIndex = (p->animIndex + 1) % 4;
            p->tempoAnimacao = 0;
        }

        p->frameAtual = PLAYER_WALK_R_1 + p->animIndex;
        return;
    }

    if (p->velX < 0) {
        if (p->tempoAnimacao >= 0.12f) {
            p->animIndex = (p->animIndex + 1) % 4;
            p->tempoAnimacao = 0;
        }

        p->frameAtual = PLAYER_WALK_L_1 + p->animIndex;
        return;
    }

    if (p->tempoAnimacao >= 0.35f) {
        p->animIndex = (p->animIndex + 1) % 4;
        p->tempoAnimacao = 0;
    }

    p->frameAtual = PLAYER_IDLE_1 + p->animIndex;
}

void atualizarPlayer(Player *p, float dt) {
    if (p->estado == PLAYER_GAME_OVER) return;

    if (p->estado == PLAYER_FANTASMA) {
        p->frameAtual = PLAYER_MORTE;
        p->tempoFantasma += dt;

        if (p->tempoFantasma >= 2.0f) {
            p->estado = PLAYER_GAME_OVER;
        }

        return;
    }

    p->velX = 0;

    if (IsKeyDown(KEY_D)) {
        p->velX = 180;
        p->direcao = DIR_DIREITA;
    }

    if (IsKeyDown(KEY_A)) {
        p->velX = -180;
        p->direcao = DIR_ESQUERDA;
    }

    if (IsKeyPressed(KEY_SPACE) && p->noChao) {
        p->velY = -380;
        p->noChao = 0;
    }

    if (IsKeyPressed(KEY_L)) {
        p->atacando = 1;
        p->tempoAtaque = 0;
        p->direcao = DIR_DIREITA;
    }

    if (IsKeyPressed(KEY_K)) {
        p->atacando = 1;
        p->tempoAtaque = 0;
        p->direcao = DIR_ESQUERDA;
    }

    p->velY += 850 * dt;

    p->x += p->velX * dt;
    p->y += p->velY * dt;

    if (p->x < 0) {
        p->x = 0;
    }

    if (p->x > MAP_COLUNAS * TILE - PLAYER_LARGURA) {
        p->x = MAP_COLUNAS * TILE - PLAYER_LARGURA;
    }

    resolverColisaoVertical(p);

    if (p->y > 600) {
        playerTomarDano(p);
        p->x = 0;
        p->y = 320;
    }

    atualizarAnimacao(p, dt);
}

void playerTomarDano(Player *p) {
    if (p->estado != PLAYER_VIVO) return;

    p->vidas--;
    p->tomandoDano = 1;
    p->tempoDano = 0;

    if (p->vidas <= 0) {
        p->estado = PLAYER_FANTASMA;
        p->tempoFantasma = 0;
        p->frameAtual = PLAYER_MORTE;
    }
}

void desenharPlayer(Player p, Texture2D sprite) {
    Rectangle origem = {
        p.frameAtual * PLAYER_LARGURA,
        0,
        PLAYER_LARGURA,
        PLAYER_ALTURA
    };

    DrawTextureRec(sprite, origem, (Vector2){p.x, p.y}, WHITE);
}