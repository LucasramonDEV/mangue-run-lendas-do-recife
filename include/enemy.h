#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

typedef enum {
    ENEMY_MELEE,
    ENEMY_SHOOTER
} EnemyType;

typedef struct Enemy {
    Rectangle rect;
    EnemyType type;
    float range;
    int life;
    int active;
    struct Enemy *next;
} Enemy;

Enemy *CreateEnemy(float x, float y, EnemyType type);
void InsertEnemy(Enemy **list, Enemy *enemy);
void LoadEnemiesFromMap(Enemy **list, int mapIndex);
void UpdateEnemies(Enemy *list, Rectangle player);
void DrawEnemies(Enemy *list, Texture2D shooterTexture);
void RemoveDeadEnemies(Enemy **list);
void FreeEnemies(Enemy **list);
int CountEnemies(Enemy *list);
int CountEnemiesByType(Enemy *list, EnemyType type);

void carregarInimigos(void);
void atualizarInimigos(Rectangle player, float dt);
void desenharInimigos(Texture2D shooterTexture);

extern Enemy *enemyList;

#endif