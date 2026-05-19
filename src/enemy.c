#include "enemy.h"
#include "map.h"
#include <stdlib.h>
#include <math.h>

Enemy *enemyList = NULL;

Enemy *CreateEnemy(float x, float y, EnemyType type) {
    Enemy *enemy = malloc(sizeof(Enemy));
    if (enemy == NULL) return NULL;

    enemy->rect = (Rectangle){x, y, TILE_SIZE, TILE_SIZE};
    enemy->type = type;
    enemy->range = type == ENEMY_MELEE ? 90 : 220;
    enemy->life = type == ENEMY_MELEE ? 2 : 1;
    enemy->active = 0;
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
    while (aux->next != NULL) aux = aux->next;
    aux->next = enemy;
}

void LoadEnemiesFromMap(Enemy **list, int mapIndex) {
    FreeEnemies(list);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        const char *linha = GetMapLine(mapIndex, telaMapaAtual, y);

        for (int x = 0; x < MAP_WIDTH; x++) {
            char tile = linha[x];
            Rectangle r = GetTileRect(x, y);

            if (tile == TILE_MELEE) InsertEnemy(list, CreateEnemy(r.x, r.y, ENEMY_MELEE));
            if (tile == TILE_SHOOTER) InsertEnemy(list, CreateEnemy(r.x, r.y, ENEMY_SHOOTER));
        }
    }
}

void UpdateEnemies(Enemy *list, Rectangle player) {
    while (list != NULL) {
        float dx = fabsf(player.x - list->rect.x);
        float dy = fabsf(player.y - list->rect.y);

        list->active = dx <= list->range && dy <= 90;

        list = list->next;
    }
}

static void desenharShooterBeta(Rectangle r, int active) {
    DrawRectangleRec(r, BLUE);
    DrawRectangleLinesEx(r, 2, BLACK);
    DrawCircle(r.x + 22, r.y + 12, 5, SKYBLUE);
    DrawRectangle(r.x + 6, r.y + 20, 22, 6, DARKBLUE);
    DrawText("T", r.x + 9, r.y + 5, 18, WHITE);

    if (active) DrawCircleLines(r.x + 16, r.y + 16, 20, ORANGE);
}

static void desenharMeleeBeta(Rectangle r, int active) {
    DrawRectangleRec(r, RED);
    DrawRectangleLinesEx(r, 2, BLACK);
    DrawCircle(r.x + 16, r.y + 10, 6, MAROON);
    DrawRectangle(r.x + 8, r.y + 18, 16, 10, DARKPURPLE);
    DrawText("M", r.x + 8, r.y + 5, 18, WHITE);

    if (active) DrawCircleLines(r.x + 16, r.y + 16, 20, ORANGE);
}

void DrawEnemies(Enemy *list, Texture2D shooterTexture) {
    while (list != NULL) {
        if (list->type == ENEMY_SHOOTER) {
            if (mapaAtual == 0 && shooterTexture.id > 0) {
                DrawTexturePro(
                    shooterTexture,
                    (Rectangle){0, 0, 32, 32},
                    list->rect,
                    (Vector2){0, 0},
                    0,
                    WHITE
                );
            } else {
                desenharShooterBeta(list->rect, list->active);
            }
        } else {
            desenharMeleeBeta(list->rect, list->active);
        }

        if (list->active) {
            DrawCircleLines(list->rect.x + 16, list->rect.y + 16, list->range, ORANGE);
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
    (void)dt;
    UpdateEnemies(enemyList, player);
}

void desenharInimigos(Texture2D shooterTexture) {
    DrawEnemies(enemyList, shooterTexture);
}