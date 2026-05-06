#include "raylib.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include "enemy.h"
<<<<<<< HEAD
#include "item.h"
#include <string.h>
#include <stdio.h>

#define MAX_NOME 6
#define QTD_PERFIS 4
#define ARQUIVO_PERFIS "perfis.txt"

typedef enum {
    TELA_MENU_INICIAL,
    TELA_LISTA_PERFIS,
    TELA_NOVO_PERFIL,
    TELA_SELECAO_FASES,
    TELA_CONFIGURACOES,
    TELA_JOGO
} TelaJogo;

static void carregarPerfis(char perfis[QTD_PERFIS][MAX_NOME + 1]) {
    for (int i = 0; i < QTD_PERFIS; i++) {
        perfis[i][0] = '\0';
    }

    FILE *arquivo = fopen(ARQUIVO_PERFIS, "r");
    if (arquivo == NULL) return;

    for (int i = 0; i < QTD_PERFIS; i++) {
        fscanf(arquivo, "%6s", perfis[i]);
        perfis[i][MAX_NOME] = '\0';

        if (strcmp(perfis[i], "vazio") == 0) {
            perfis[i][0] = '\0';
        }
    }

    fclose(arquivo);
}

static void salvarPerfis(char perfis[QTD_PERFIS][MAX_NOME + 1]) {
    FILE *arquivo = fopen(ARQUIVO_PERFIS, "w");
    if (arquivo == NULL) return;

    for (int i = 0; i < QTD_PERFIS; i++) {
        if (strlen(perfis[i]) > 0) {
            fprintf(arquivo, "%s\n", perfis[i]);
        } else {
            fprintf(arquivo, "vazio\n");
        }
    }

    fclose(arquivo);
}
=======
#include "projectile.h"
#include "item.h"
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f

static void desenharHUD(Player player, Texture2D heartTexture) {
    Rectangle origem = {0, 0, 32, 32};

    for (int i = 0; i < player.vidas; i++) {
        DrawTextureRec(heartTexture, origem, (Vector2){20 + i * 36, 20}, WHITE);
    }
}

<<<<<<< HEAD
static int botao(Rectangle rec, const char *texto) {
    Vector2 mouse = GetMousePosition();
    int hover = CheckCollisionPointRec(mouse, rec);

    DrawRectangleRec(rec, hover ? DARKGRAY : GRAY);
    DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, WHITE);

    int textoLargura = MeasureText(texto, 24);
    DrawText(texto, rec.x + rec.width / 2 - textoLargura / 2, rec.y + 13, 24, WHITE);

    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static void desenharBackgroundNoMundo(Texture2D bg) {
    Rectangle origem = {0, 0, bg.width, bg.height};

    Rectangle destino = {
        -400,
        -200,
        MAP_COLUNAS * TILE + 800,
        MAP_LINHAS * TILE + 400
    };

    DrawTexturePro(bg, origem, destino, (Vector2){0, 0}, 0, WHITE);
}

static void telaMenuInicial(TelaJogo *tela) {
    DrawText("MANGUE RUN", 260, 70, 50, WHITE);
    DrawText("Lendas do Recife", 285, 125, 25, LIGHTGRAY);

    if (botao((Rectangle){280, 190, 240, 55}, "NOVO JOGO")) {
        *tela = TELA_LISTA_PERFIS;
    }

    if (botao((Rectangle){280, 260, 240, 55}, "CARREGAR PERFIL")) {
        *tela = TELA_LISTA_PERFIS;
    }

    if (botao((Rectangle){280, 330, 240, 55}, "CONFIGURACOES")) {
        *tela = TELA_CONFIGURACOES;
    }

    if (botao((Rectangle){280, 400, 240, 55}, "FECHAR")) {
        CloseWindow();
    }
}

static void telaConfiguracoes(TelaJogo *tela) {
    DrawText("CONFIGURACOES", 260, 80, 40, WHITE);
    DrawText("Opcoes serao adicionadas futuramente.", 210, 170, 24, LIGHTGRAY);

    if (botao((Rectangle){20, 20, 130, 45}, "VOLTAR")) {
        *tela = TELA_MENU_INICIAL;
    }
}

static void telaListaPerfis(TelaJogo *tela, char perfis[QTD_PERFIS][MAX_NOME + 1],
                            int *slotSelecionado, char nomeDigitado[MAX_NOME + 1],
                            char nomePerfilAtual[MAX_NOME + 1]) {
    DrawText("PERFIS", 335, 70, 45, WHITE);
    DrawText("Escolha um perfil ou selecione um slot vazio.", 165, 130, 22, LIGHTGRAY);

    for (int i = 0; i < QTD_PERFIS; i++) {
        char textoBotao[30];

        if (strlen(perfis[i]) > 0) {
            snprintf(textoBotao, sizeof(textoBotao), "%d - %s", i + 1, perfis[i]);
        } else {
            snprintf(textoBotao, sizeof(textoBotao), "%d - vazio", i + 1);
        }

        Rectangle rec = {260, 180 + i * 65, 280, 50};

        if (botao(rec, textoBotao)) {
            *slotSelecionado = i;

            if (strlen(perfis[i]) > 0) {
                strcpy(nomePerfilAtual, perfis[i]);
                *tela = TELA_SELECAO_FASES;
            } else {
                nomeDigitado[0] = '\0';
                *tela = TELA_NOVO_PERFIL;
            }
        }
    }

    if (botao((Rectangle){20, 20, 130, 45}, "VOLTAR")) {
        *tela = TELA_MENU_INICIAL;
    }
}

static void telaNovoPerfil(TelaJogo *tela, char perfis[QTD_PERFIS][MAX_NOME + 1],
                           int slotSelecionado, char nomeDigitado[MAX_NOME + 1],
                           char nomePerfilAtual[MAX_NOME + 1]) {
    DrawText("CRIAR PERFIL", 250, 80, 40, WHITE);
    DrawText("Digite ate 6 caracteres", 250, 155, 24, LIGHTGRAY);
    DrawText("ENTER confirma | BACKSPACE/DELETE apaga", 160, 340, 22, LIGHTGRAY);

    Rectangle caixa = {275, 220, 250, 60};
    DrawRectangleRec(caixa, DARKGRAY);
    DrawRectangleLines(caixa.x, caixa.y, caixa.width, caixa.height, WHITE);

    DrawText(nomeDigitado, caixa.x + 20, caixa.y + 17, 28, WHITE);

    int tecla = GetCharPressed();

    while (tecla > 0) {
        int tamanho = strlen(nomeDigitado);

        if (tamanho < MAX_NOME && tecla >= 32 && tecla <= 126) {
            nomeDigitado[tamanho] = (char)tecla;
            nomeDigitado[tamanho + 1] = '\0';
        }

        tecla = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_DELETE)) {
        int tamanho = strlen(nomeDigitado);

        if (tamanho > 0) {
            nomeDigitado[tamanho - 1] = '\0';
        }
    }

    if (IsKeyPressed(KEY_ENTER) && strlen(nomeDigitado) > 0) {
        strcpy(perfis[slotSelecionado], nomeDigitado);
        strcpy(nomePerfilAtual, nomeDigitado);
        salvarPerfis(perfis);
        *tela = TELA_SELECAO_FASES;
    }

    if (botao((Rectangle){20, 20, 130, 45}, "VOLTAR")) {
        nomeDigitado[0] = '\0';
        *tela = TELA_LISTA_PERFIS;
    }
}

static void telaSelecaoFases(TelaJogo *tela, const char *nomePerfil, Player *player) {
    const char *fases[11] = {
        "1 - Praia de Boa Viagem",
        "2 - Praca de Casa Forte",
        "3 - Parque da Jaqueira",
        "4 - Parque das Gracas",
        "5 - Praca do Derby",
        "6 - Agamenon",
        "7 - Boa Vista",
        "8 - Forte das Cinco Pontas",
        "9 - Rua do Bom Jesus",
        "10 - Marco Zero",
        "11 - Final"
    };

    DrawText("SELECAO DE FASES", 225, 45, 36, WHITE);
    DrawText(TextFormat("Perfil: %s", nomePerfil), 20, 85, 22, LIGHTGRAY);

    for (int i = 0; i < 11; i++) {
        Rectangle rec = {180, 115 + i * 30, 440, 25};

        Vector2 mouse = GetMousePosition();
        int hover = CheckCollisionPointRec(mouse, rec);

        DrawRectangleRec(rec, hover ? DARKGRAY : BLACK);
        DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, GRAY);
        DrawText(fases[i], rec.x + 10, rec.y + 4, 18, WHITE);

        if (i == 0 && hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *player = criarPlayer(0, 320);
            player->vidas = 3;
            *tela = TELA_JOGO;
        }
    }

    DrawText("Somente a fase 1 esta disponivel por enquanto.", 170, 415, 20, YELLOW);

    if (botao((Rectangle){20, 20, 130, 45}, "VOLTAR")) {
        *tela = TELA_MENU_INICIAL;
    }
}

static void rodarJogo(Player *player, Camera2D *camera, Texture2D bg, Texture2D tileChao,
                      Texture2D tileBloco, Texture2D tilePlataforma,
                      Texture2D enemyShooter, Texture2D heartSprite, Texture2D playerSprite,
                      TelaJogo *tela, char perfis[QTD_PERFIS][MAX_NOME + 1],
                      float *tempoGameOver) {
    float dt = GetFrameTime();

    if (IsKeyPressed(KEY_ESCAPE)) {
        salvarPerfis(perfis);
        *player = criarPlayer(0, 320);
        player->vidas = 3;
        *tempoGameOver = 0;
        *tela = TELA_MENU_INICIAL;
        return;
    }

    if (player->estado != PLAYER_GAME_OVER) {
        atualizarPlayer(player, dt);
        atualizarInimigos(player, dt);
        atualizarItens(player);
    }

    int telaLargura = GetScreenWidth();
    int telaAltura = GetScreenHeight();

    camera->offset = (Vector2){telaLargura / 2.0f, telaAltura / 2.0f};
    camera->target = (Vector2){player->x + 16, player->y + 16};

    float metadeTelaMundo = (telaLargura / 2.0f) / camera->zoom;

    if (camera->target.x < metadeTelaMundo) {
        camera->target.x = metadeTelaMundo;
    }

    if (camera->target.x > MAP_COLUNAS * TILE - metadeTelaMundo) {
        camera->target.x = MAP_COLUNAS * TILE - metadeTelaMundo;
    }

    BeginMode2D(*camera);

    desenharBackgroundNoMundo(bg);
    desenharMapa(tileChao, tileBloco, tilePlataforma);
    desenharItens(heartSprite);
    desenharInimigos(enemyShooter);
    desenharPlayer(*player, playerSprite);

    EndMode2D();

    desenharHUD(*player, heartSprite);

    DrawText("ESC - voltar para o menu inicial", 20, GetScreenHeight() - 30, 20, WHITE);

    if (player->estado == PLAYER_GAME_OVER) {
        *tempoGameOver += dt;

        DrawRectangle(0, 0, telaLargura, telaAltura, Fade(BLACK, 0.55f));
        DrawText("GAME OVER", telaLargura / 2 - 150, telaAltura / 2 - 45, 50, RED);
        DrawText("Voltando ao menu...", telaLargura / 2 - 130, telaAltura / 2 + 20, 24, WHITE);

        if (*tempoGameOver >= 5.0f) {
            salvarPerfis(perfis);
            *player = criarPlayer(0, 320);
            player->vidas = 3;
            *tempoGameOver = 0;
            *tela = TELA_MENU_INICIAL;
        }
=======
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
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f
    }
}

void iniciarJogo(void) {
    InitWindow(800, 450, "Mangue Run: Lendas do Recife");
<<<<<<< HEAD
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    TelaJogo tela = TELA_MENU_INICIAL;

    char perfis[QTD_PERFIS][MAX_NOME + 1];
    char nomeDigitado[MAX_NOME + 1] = "";
    char nomePerfilAtual[MAX_NOME + 1] = "";
    int slotSelecionado = 0;
    float tempoGameOver = 0;

    carregarPerfis(perfis);

    Texture2D playerSprite = LoadTexture("assets/player/playerMODEL.png");
    Texture2D bg = LoadTexture("assets/maps/01_boa_viagem/background.png");

    Texture2D tileChao = LoadTexture("assets/maps/01_boa_viagem/tiles/chao.png");
    Texture2D tileBloco = LoadTexture("assets/maps/01_boa_viagem/tiles/bloco.png");
    Texture2D tilePlataforma = LoadTexture("assets/maps/01_boa_viagem/tiles/plataforma.png");

    Texture2D enemyShooter = LoadTexture("assets/maps/01_boa_viagem/enemy_shooter.png");
    Texture2D heartSprite = LoadTexture("assets/items/heart.png");

    Player player = criarPlayer(0, 320);
    player.vidas = 3;
=======
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
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f

    carregarMapa();
    carregarItens();
    carregarInimigos();

    Camera2D camera = {0};
<<<<<<< HEAD
    camera.rotation = 0;
    camera.zoom = 1.20f;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
=======
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
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f
        }

        BeginDrawing();
        ClearBackground(BLACK);

<<<<<<< HEAD
        if (tela == TELA_MENU_INICIAL) {
            telaMenuInicial(&tela);
        } else if (tela == TELA_LISTA_PERFIS) {
            telaListaPerfis(&tela, perfis, &slotSelecionado, nomeDigitado, nomePerfilAtual);
        } else if (tela == TELA_NOVO_PERFIL) {
            telaNovoPerfil(&tela, perfis, slotSelecionado, nomeDigitado, nomePerfilAtual);
        } else if (tela == TELA_SELECAO_FASES) {
            telaSelecaoFases(&tela, nomePerfilAtual, &player);
        } else if (tela == TELA_CONFIGURACOES) {
            telaConfiguracoes(&tela);
        } else if (tela == TELA_JOGO) {
            rodarJogo(&player, &camera, bg, tileChao, tileBloco, tilePlataforma,
                      enemyShooter, heartSprite, playerSprite, &tela, perfis, &tempoGameOver);
=======
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
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f
        }

        EndDrawing();
    }

<<<<<<< HEAD
    salvarPerfis(perfis);

    UnloadTexture(playerSprite);
    UnloadTexture(bg);
    UnloadTexture(tileChao);
    UnloadTexture(tileBloco);
    UnloadTexture(tilePlataforma);
    UnloadTexture(enemyShooter);
=======
    UnloadTexture(playerSprite);
    UnloadTexture(bgMarco);
    UnloadTexture(bgBomJesus);
    UnloadTexture(tilesetSprite);
    UnloadTexture(enemiesSprite);
    UnloadTexture(notaSprite);
    UnloadTexture(tiroInimigoSprite);
>>>>>>> af22300ea9d746e1528ad1e504e19b72c6c9cc9f
    UnloadTexture(heartSprite);

    CloseWindow();
}