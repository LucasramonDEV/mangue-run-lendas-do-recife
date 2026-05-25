#include "enemy.h"
#include "map.h"
#include "projectile.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define ENEMY_SHOOT_INTERVAL 1.6f
#define ENEMY_MELEE_SPEED 45.0f
#define ENEMY_MELEE_RANGE 90.0f
#define ENEMY_SHOOTER_RANGE 260.0f
#define ENEMY_VERTICAL_RANGE 90.0f

Enemy *enemyList = NULL;

static Texture2D shooterTextures[TOTAL_MAPS];
static Texture2D meleeTextures[TOTAL_MAPS];

static const char *pastasMapasInimigos[TOTAL_MAPS] = {
    "01_boa_viagem",
    "02_casa_forte",
    "03_jaqueira",
    "04_gracas",
    "05_bom_jesus",
    "06_marco_zero"
};

static const char *prefixosMapasInimigos[TOTAL_MAPS] = {
    "bv",
    "cf",
    "ja",
    "gc",
    "bm",
    "mz"
};

static Texture2D carregarTexturaComFallback(const char *caminhoPrincipal, const char *caminhoReserva) {
    Texture2D textura = LoadTexture(caminhoPrincipal);

    if (textura.id == 0 && caminhoReserva != NULL) {
        textura = LoadTexture(caminhoReserva);
    }

    return textura;
}

void carregarTexturasInimigos(void) {
    char caminhoPrincipal[180];
    char caminhoReserva[180];

    for (int i = 0; i < TOTAL_MAPS; i++) {
        shooterTextures[i].id = 0;
        meleeTextures[i].id = 0;
    }

    for (int i = 0; i < TOTAL_MAPS; i++) {
        sprintf(
            caminhoPrincipal,
            "assets/maps/%s/%s_enemy_shoot.png",
            pastasMapasInimigos[i],
            prefixosMapasInimigos[i]
        );

        sprintf(
            caminhoReserva,
            "assets/maps/%s/%s_enemy_shooter.png",
            pastasMapasInimigos[i],
            prefixosMapasInimigos[i]
        );

        shooterTextures[i] = carregarTexturaComFallback(caminhoPrincipal, caminhoReserva);

        sprintf(
            caminhoPrincipal,
            "assets/maps/%s/%s_enemy_melle.png",
            pastasMapasInimigos[i],
            prefixosMapasInimigos[i]
        );

        sprintf(
            caminhoReserva,
            "assets/maps/%s/%s_enemy_melee.png",
            pastasMapasInimigos[i],
            prefixosMapasInimigos[i]
        );

        meleeTextures[i] = carregarTexturaComFallback(caminhoPrincipal, caminhoReserva);
    }
}

void descarregarTexturasInimigos(void) {
    for (int i = 0; i < TOTAL_MAPS; i++) {
        if (shooterTextures[i].id > 0) {
            UnloadTexture(shooterTextures[i]);
            shooterTextures[i].id = 0;
        }

        if (meleeTextures[i].id > 0) {
            UnloadTexture(meleeTextures[i]);
            meleeTextures[i].id = 0;
        }
    }
}

Enemy *CreateEnemy(float x, float y, EnemyType type) {
    Enemy *enemy = malloc(sizeof(Enemy));

    if (enemy == NULL) return NULL;

    enemy->rect = (Rectangle){x, y, TILE_SIZE, TILE_SIZE};
    enemy->type = type;
    enemy->range = type == ENEMY_MELEE ? ENEMY_MELEE_RANGE : ENEMY_SHOOTER_RANGE;
    enemy->life = type == ENEMY_MELEE ? 2 : 1;
    enemy->active = 0;
    enemy->direction = -1;
    enemy->shootTimer = 0.0f;
    enemy->moveTimer = 0.0f;
    enemy->next = NULL;

    return enemy;
}

void InsertEnemy(Enemy **list, Enemy *enemy) {
    if (enemy == NULL) return;

    if (*list == NULL) {
        *list = enemy;
        return;
    }

    Enemy *aux = *list;

    while (aux->next != NULL) {
        aux = aux->next;
    }

    aux->next = enemy;
}

void LoadEnemiesFromMap(Enemy **list, int mapIndex) {
    FreeEnemies(list);

    if (mapIndex < 0 || mapIndex >= TOTAL_MAPS) return;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        const char *linha = GetMapLine(mapIndex, telaMapaAtual, y);

        for (int x = 0; x < MAP_WIDTH; x++) {
            char tile = linha[x];
            Rectangle r = GetTileRect(x, y);

            if (tile == TILE_MELEE) {
                InsertEnemy(list, CreateEnemy(r.x, r.y, ENEMY_MELEE));
            }

            if (tile == TILE_SHOOTER) {
                InsertEnemy(list, CreateEnemy(r.x, r.y, ENEMY_SHOOTER));
            }
        }
    }
}

static int inimigoEstaPertoDoPlayer(Enemy *enemy, Rectangle player) {
    float dx = fabsf(player.x - enemy->rect.x);
    float dy = fabsf(player.y - enemy->rect.y);

    return dx <= enemy->range && dy <= ENEMY_VERTICAL_RANGE;
}

static void atualizarDirecaoInimigo(Enemy *enemy, Rectangle player) {
    if (player.x < enemy->rect.x) {
        enemy->direction = -1;
    } else {
        enemy->direction = 1;
    }
}

static void atualizarMelee(Enemy *enemy, float dt) {
    if (!enemy->active) {
        enemy->moveTimer = 0.0f;
        return;
    }

    enemy->moveTimer += dt;

    float oldX = enemy->rect.x;
    enemy->rect.x += enemy->direction * ENEMY_MELEE_SPEED * dt;

    int tileBaixoEsquerda = pegarTilePorPixel(enemy->rect.x, enemy->rect.y + enemy->rect.height + 2);
    int tileBaixoDireita = pegarTilePorPixel(enemy->rect.x + enemy->rect.width - 1, enemy->rect.y + enemy->rect.height + 2);
    int tileFrenteCima = pegarTilePorPixel(enemy->rect.x + (enemy->direction > 0 ? enemy->rect.width : 0), enemy->rect.y + 4);
    int tileFrenteBaixo = pegarTilePorPixel(enemy->rect.x + (enemy->direction > 0 ? enemy->rect.width : 0), enemy->rect.y + enemy->rect.height - 4);

    if ((!tileEhSolido(tileBaixoEsquerda) && !tileEhSolido(tileBaixoDireita)) || tileEhSolido(tileFrenteCima) || tileEhSolido(tileFrenteBaixo)) {
        enemy->rect.x = oldX;
    }
}

static void atualizarShooter(Enemy *enemy, float dt) {
    if (!enemy->active) {
        enemy->shootTimer = 0.0f;
        return;
    }

    enemy->shootTimer += dt;

    if (enemy->shootTimer >= ENEMY_SHOOT_INTERVAL) {
        enemy->shootTimer = 0.0f;

        float tiroX = enemy->direction < 0 ? enemy->rect.x : enemy->rect.x + enemy->rect.width;
        float tiroY = enemy->rect.y + enemy->rect.height / 2.0f;

        dispararProjetil(tiroX, tiroY, enemy->direction, PROJETIL_INIMIGO);
    }
}

void UpdateEnemies(Enemy *list, Rectangle player, float dt) {
    while (list != NULL) {
        list->active = inimigoEstaPertoDoPlayer(list, player);
        atualizarDirecaoInimigo(list, player);

        if (list->type == ENEMY_MELEE) {
            atualizarMelee(list, dt);
        } else if (list->type == ENEMY_SHOOTER) {
            atualizarShooter(list, dt);
        }

        list = list->next;
    }
}

static void desenharShooterBeta(Rectangle r, int active) {
    DrawRectangleRec(r, BLUE);
    DrawRectangleLinesEx(r, 2, BLACK);
    DrawCircle((int)(r.x + 22), (int)(r.y + 12), 5, SKYBLUE);
    DrawRectangle((int)(r.x + 6), (int)(r.y + 20), 22, 6, DARKBLUE);
    DrawText("T", (int)(r.x + 9), (int)(r.y + 5), 18, WHITE);

    if (active) {
        DrawCircleLines((int)(r.x + 16), (int)(r.y + 16), 20, ORANGE);
    }
}

static void desenharMeleeBeta(Rectangle r, int active) {
    DrawRectangleRec(r, RED);
    DrawRectangleLinesEx(r, 2, BLACK);
    DrawCircle((int)(r.x + 16), (int)(r.y + 10), 6, MAROON);
    DrawRectangle((int)(r.x + 8), (int)(r.y + 18), 16, 10, DARKPURPLE);
    DrawText("M", (int)(r.x + 8), (int)(r.y + 5), 18, WHITE);

    if (active) {
        DrawCircleLines((int)(r.x + 16), (int)(r.y + 16), 20, ORANGE);
    }
}

static void desenharSprite(Texture2D texture, Rectangle dst, int direction, int active, EnemyType type) {
    if (texture.id <= 0) {
        if (type == ENEMY_SHOOTER) desenharShooterBeta(dst, active);
        else desenharMeleeBeta(dst, active);
        return;
    }

    int frame = 0;

    if (active) {
        frame = direction < 0 ? 1 : 2;
    }

    int frameWidth = texture.width / 3;
    int frameHeight = texture.height;

    Rectangle src = {frame * frameWidth, 0, frameWidth, frameHeight};
    DrawTexturePro(texture, src, dst, (Vector2){0, 0}, 0, WHITE);
}

void DrawEnemies(Enemy *list) {
    if (mapaAtual < 0 || mapaAtual >= TOTAL_MAPS) return;

    while (list != NULL) {
        if (list->type == ENEMY_SHOOTER) {
            desenharSprite(shooterTextures[mapaAtual], list->rect, list->direction, list->active, ENEMY_SHOOTER);
        } else {
            desenharSprite(meleeTextures[mapaAtual], list->rect, list->direction, list->active, ENEMY_MELEE);
        }

        list = list->next;
    }
}

void RemoveDeadEnemies(Enemy **list) {
    Enemy *current = *list;
    Enemy *previous = NULL;

    while (current != NULL) {
        if (current->life <= 0) {
            Enemy *removed = current;

            if (previous == NULL) {
                *list = current->next;
                current = *list;
            } else {
                previous->next = current->next;
                current = current->next;
            }

            free(removed);
        } else {
            previous = current;
            current = current->next;
        }
    }
}

void FreeEnemies(Enemy **list) {
    Enemy *aux = *list;

    while (aux != NULL) {
        Enemy *temp = aux;
        aux = aux->next;
        free(temp);
    }

    *list = NULL;
}

int CountEnemies(Enemy *list) {
    int count = 0;

    while (list != NULL) {
        count++;
        list = list->next;
    }

    return count;
}

int CountEnemiesByType(Enemy *list, EnemyType type) {
    int count = 0;

    while (list != NULL) {
        if (list->type == type) count++;
        list = list->next;
    }

    return count;
}

void carregarInimigos(void) {
    LoadEnemiesFromMap(&enemyList, mapaAtual);
}

void atualizarInimigos(Rectangle player, float dt) {
    UpdateEnemies(enemyList, player, dt);
}

void desenharInimigos(void) {
    DrawEnemies(enemyList);
}
