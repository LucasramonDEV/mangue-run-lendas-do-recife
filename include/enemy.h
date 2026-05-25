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
    int direction;
    float shootTimer;
    float moveTimer;
    struct Enemy *next;
} Enemy;

Enemy *CreateEnemy(float x, float y, EnemyType type);
void InsertEnemy(Enemy **list, Enemy *enemy);
void LoadEnemiesFromMap(Enemy **list, int mapIndex);
void UpdateEnemies(Enemy *list, Rectangle player, float dt);
void DrawEnemies(Enemy *list);
void RemoveDeadEnemies(Enemy **list);
void FreeEnemies(Enemy **list);
int CountEnemies(Enemy *list);
int CountEnemiesByType(Enemy *list, EnemyType type);

void carregarInimigos(void);
void atualizarInimigos(Rectangle player, float dt);
void desenharInimigos(void);
void carregarTexturasInimigos(void);
void descarregarTexturasInimigos(void);

extern Enemy *enemyList;

#endif
