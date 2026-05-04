#include "raylib.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include "enemy.h"
#include "projectile.h"
#include "item.h"

static void desenharHUD(Player player, Texture2D heartTexture) {
    Rectangle origem = {0, 0, 32, 32};

    for (int i = 0; i < player.vidas; i++) {
        DrawTextureRec(heartTexture, origem, (Vector2){20 + i * 36, 20}, WHITE);
    }
}

static void verificarTiros(Player *player) {
    Projectile *p = getListaProjeteis();
    Projectile *proxProjetil;

    while (p != NULL) {
        proxProjetil = p->prox;

        Rectangle projRec = {p->x, p->y, 16, 16};

        if (p->tipo == PROJETIL_PERSONAGEM) {
            Enemy *e = getListaInimigos();

            while (e != NULL) {
                if (e->estado == INIMIGO_VIVO) {
                    Rectangle enemyRec = {e->x, e->y, 32, 32};

                    if (CheckCollisionRecs(projRec, enemyRec)) {
                        inimigoVirarFantasma(e);
                        removerProjetil(p);
                        break;
                    }
                }

                e = e->prox;
            }
        } else {
            Rectangle playerRec = getPlayerRect(*player);

            if (CheckCollisionRecs(projRec, playerRec)) {
                playerTomarDano(player);
                removerProjetil(p);
            }
        }

        p = proxProjetil;
    }
}

void iniciarJogo(void) {
    InitWindow(800, 450, "Mangue Run: Lendas do Recife");
    SetTargetFPS(60);

    Texture2D playerSprite = LoadTexture("assets/playerMODEL.png");
    Texture2D bgMarco = LoadTexture("assets/bg_marco.png");
    Texture2D bgBomJesus = LoadTexture("assets/bg_bom_jesus.png");
    Texture2D tilesetSprite = LoadTexture("assets/RecAntigoTiles.png");
    Texture2D enemiesSprite = LoadTexture("assets/RecifeAntigoEnemys.png");
    Texture2D notaSprite = LoadTexture("assets/DisparoPersonagem.png");
    Texture2D tiroInimigoSprite = LoadTexture("assets/DisparoInimigo.png");
    Texture2D heartSprite = LoadTexture("assets/heart.png");

    Player player = criarPlayer(64, 288);

    carregarMapa();
    carregarItens();
    carregarInimigos();

    Camera2D camera = {0};
    camera.target = (Vector2){player.x, player.y};
    camera.offset = (Vector2){400, 225};
    camera.rotation = 0;
    camera.zoom = 1.5f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (player.estado == PLAYER_VIVO) {
            if (IsKeyPressed(KEY_L)) {
                dispararProjetil(player.x + 28, player.y + 15, 1, PROJETIL_PERSONAGEM);
            }

            if (IsKeyPressed(KEY_K)) {
                dispararProjetil(player.x - 10, player.y + 15, -1, PROJETIL_PERSONAGEM);
            }
        }

        atualizarPlayer(&player, dt);
        atualizarProjeteis(dt);
        atualizarInimigos(&player, dt);
        atualizarItens(&player);
        verificarTiros(&player);

        camera.target = (Vector2){player.x + 16, player.y + 16};

        if (camera.target.x < 400 / camera.zoom) camera.target.x = 400 / camera.zoom;
        if (camera.target.x > MAP_COLUNAS * TILE - (400 / camera.zoom)) {
            camera.target.x = MAP_COLUNAS * TILE - (400 / camera.zoom);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (player.x < 40 * TILE) {
            DrawTexture(bgMarco, 0, 0, WHITE);
        } else {
            DrawTexture(bgBomJesus, 0, 0, WHITE);
        }

        BeginMode2D(camera);

        desenharMapa(tilesetSprite);
        desenharItens(heartSprite);
        desenharInimigos(enemiesSprite);
        desenharProjeteis(notaSprite, tiroInimigoSprite);
        desenharPlayer(player, playerSprite);

        EndMode2D();

        desenharHUD(player, heartSprite);

        if (player.estado == PLAYER_GAME_OVER) {
            DrawText("GAME OVER", 280, 180, 50, RED);
        }

        EndDrawing();
    }

    UnloadTexture(playerSprite);
    UnloadTexture(bgMarco);
    UnloadTexture(bgBomJesus);
    UnloadTexture(tilesetSprite);
    UnloadTexture(enemiesSprite);
    UnloadTexture(notaSprite);
    UnloadTexture(tiroInimigoSprite);
    UnloadTexture(heartSprite);

    CloseWindow();
}