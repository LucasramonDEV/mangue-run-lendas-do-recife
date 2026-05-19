#include "game.h"
#include "raylib.h"
#include "map.h"
#include "enemy.h"
#include "item.h"
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define PLAYER_W 32
#define PLAYER_H 32
#define PLAYER_SPEED 4.0f
#define GRAVITY 0.6f
#define JUMP_FORCE -13.0f
#define CAMERA_ZOOM 1.65f
#define PERFIL_MAX 4

typedef enum {
    TELA_MENU_INICIAL,
    TELA_NOVO_PERFIL,
    TELA_CARREGAR_PERFIL,
    TELA_CONFIGURACOES,
    TELA_MENU_FASES,
    TELA_CUTSCENE,
    TELA_JOGO,
    TELA_CONFIRMAR_SAIDA,
    TELA_FASE_COMPLETA,
    TELA_GAME_OVER
} Tela;

typedef struct {
    Rectangle rect;
    float velY;
    int noChao;
    int vidas;
    int direcao;
    int frame;
    float tempoAnimacao;
    int atacando;
    float tempoAtaque;
} PlayerLocal;

static Tela telaAtual = TELA_MENU_INICIAL;
static Tela telaAnterior = TELA_MENU_INICIAL;

static int faseAtual = 0;
static int faseDesbloqueada = TOTAL_MAPS - 1;
static int modoNoturno = 0;
static int perfilSelecionado = 0;

static char perfis[PERFIL_MAX][30] = {
    "Teste",
    "Vazio",
    "Vazio",
    "Vazio"
};

static float tempoCutscene = 0;
static float tempoFase = 0;
static float tempoConclusao = 0;
static float tempoGameOver = 0;
static float tempoFinalizado = 0;
static int vidasFinalizadas = 0;

static PlayerLocal player;
static Rectangle porta;
static Rectangle tunel;
static Camera2D camera;

static Texture2D playerTexture;
static Texture2D background;
static Texture2D enemyShooter;
static Texture2D heartTexture;

static MapTextures mapTextures;

static const char *cutscenes[TOTAL_MAPS][4] = {
    {"Praia de Boa Viagem", "Chico chega na orla apos ouvir um chamado estranho.", "Cada tunel leva para outra parte da praia.", "A ultima parte possui a porta final da missao."},
    {"Praca de Casa Forte", "A energia das lendas segue para Casa Forte.", "Os tuneis conectam pracas e caminhos diferentes.", "Continue avancando ate chegar na porta final."},
    {"Parque da Jaqueira", "Na Jaqueira, o caminho se divide entre baixo e alto.", "Cada tela representa uma parte do parque.", "Use os tuneis para atravessar todo o mapa."},
    {"Parque das Gracas", "As Gracas escondem uma rota tomada por sombras.", "As passagens ligam trechos diferentes da fase.", "A porta aparece somente na ultima tela."},
    {"Praca do Derby", "No Derby, o movimento da cidade vira desafio.", "Avance por partes conectadas por tuneis.", "Chegue vivo ate a porta final."},
    {"Agamenon", "Na Agamenon, o caos da avenida vira fase.", "Cada tunel representa uma travessia da avenida.", "Continue ate a ultima tela."},
    {"Boa Vista", "A Boa Vista guarda historias antigas nas ruas.", "Passe pelos tuneis e avance pelo centro.", "A ultima tela encerra a missao."},
    {"Forte das Cinco Pontas", "No Forte, o Recife antigo ganha um ar de batalha.", "Cada parte da fase fica conectada por tuneis.", "A porta fica no fim do percurso."},
    {"Rua do Bom Jesus", "Na Rua do Bom Jesus, a cidade fica mais misteriosa.", "Os tuneis levam por trechos da rua antiga.", "Atravesse ate encontrar a saida."},
    {"Marco Zero", "No Marco Zero, todas as lendas se encontram.", "As telas finais formam a ultima travessia.", "Chegue ate a porta e encerre a jornada."}
};

static Color corFundo(void) {
    return modoNoturno ? (Color){18, 24, 38, 255} : WHITE;
}

static Color corTexto(void) {
    return modoNoturno ? RAYWHITE : BLACK;
}

static void salvarConfiguracoes(void) {
    FILE *f = fopen("config.txt", "w");
    if (f == NULL) return;
    fprintf(f, "%d\n", modoNoturno);
    fclose(f);
}

static void carregarConfiguracoes(void) {
    FILE *f = fopen("config.txt", "r");
    if (f == NULL) return;
    fscanf(f, "%d", &modoNoturno);
    fclose(f);
}

static void desenharBotao(Rectangle r, const char *texto, Color cor) {
    DrawRectangleRec(r, cor);
    DrawRectangleLinesEx(r, 2, BLACK);
    DrawText(texto, r.x + 18, r.y + 14, 22, BLACK);
}

static void atualizarCamera(void) {
    camera.target = (Vector2){player.rect.x + player.rect.width / 2, player.rect.y + player.rect.height / 2};
    camera.offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    camera.rotation = 0;
    camera.zoom = CAMERA_ZOOM;

    float visivelW = SCREEN_WIDTH / camera.zoom;
    float visivelH = SCREEN_HEIGHT / camera.zoom;
    float mundoW = MAP_WIDTH * TILE_SIZE;
    float mundoH = MAP_HEIGHT * TILE_SIZE;

    if (camera.target.x < visivelW / 2) camera.target.x = visivelW / 2;
    if (camera.target.x > mundoW - visivelW / 2) camera.target.x = mundoW - visivelW / 2;
    if (camera.target.y < visivelH / 2) camera.target.y = visivelH / 2;
    if (camera.target.y > mundoH - visivelH / 2) camera.target.y = mundoH - visivelH / 2;
}

static void carregarParteAtual(void) {
    Rectangle playerRect = {0, 0, PLAYER_W, PLAYER_H};

    mapaAtual = faseAtual;

    FindPlayerStart(faseAtual, telaMapaAtual, &playerRect);
    FindDoor(faseAtual, telaMapaAtual, &porta);
    FindTunnel(faseAtual, telaMapaAtual, &tunel);

    player.rect = playerRect;
    player.velY = 0;
    player.noChao = 0;
    player.direcao = 1;
    player.frame = 0;
    player.tempoAnimacao = 0;
    player.atacando = 0;
    player.tempoAtaque = 0;

    LoadEnemiesFromMap(&enemyList, faseAtual);
    carregarItens();

    UnloadMapTextures(&mapTextures);
    LoadMapTextures(&mapTextures, faseAtual);

    atualizarCamera();
}

static void carregarFaseAtual(void) {
    telaMapaAtual = 0;
    player.vidas = 3;
    tempoFase = 0;
    carregarParteAtual();
}

static int checarColisaoMapa(Rectangle r) {
    int esquerda = (int)(r.x / TILE_SIZE);
    int direita = (int)((r.x + r.width - 1) / TILE_SIZE);
    int cima = (int)(r.y / TILE_SIZE);
    int baixo = (int)((r.y + r.height - 1) / TILE_SIZE);

    for (int y = cima; y <= baixo; y++) {
        for (int x = esquerda; x <= direita; x++) {
            if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return 1;

            if (IsSolidTile(GetMapLine(faseAtual, telaMapaAtual, y)[x])) return 1;
        }
    }

    return 0;
}

static int checarMortePorAbismo(Rectangle r) {
    int centroX = (int)((r.x + r.width / 2) / TILE_SIZE);
    int baixo = (int)((r.y + r.height) / TILE_SIZE);

    if (centroX < 0 || centroX >= MAP_WIDTH || baixo < 0 || baixo >= MAP_HEIGHT) return 1;

    return IsDeadlyTile(GetMapLine(faseAtual, telaMapaAtual, baixo)[centroX]);
}

static void reiniciarPosicaoPlayer(void) {
    Rectangle start = {0, 0, PLAYER_W, PLAYER_H};

    FindPlayerStart(faseAtual, telaMapaAtual, &start);

    player.rect = start;
    player.velY = 0;
    player.noChao = 0;

    atualizarCamera();
}

static void perderVida(void) {
    player.vidas--;

    if (player.vidas <= 0) {
        tempoGameOver = 0;
        telaAtual = TELA_GAME_OVER;
    } else {
        reiniciarPosicaoPlayer();
    }
}

static void atualizarPlayerLocal(void) {
    float dx = 0;
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        dx -= PLAYER_SPEED;
        player.direcao = -1;
    }

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        dx += PLAYER_SPEED;
        player.direcao = 1;
    }

    if (IsKeyPressed(KEY_K) || IsKeyPressed(KEY_L)) {
        player.atacando = 1;
        player.tempoAtaque = 0.25f;
    }

    if (player.atacando) {
        player.tempoAtaque -= dt;

        if (player.tempoAtaque <= 0) {
            player.atacando = 0;
        }
    }

    player.tempoAnimacao += dt;

    if (player.atacando) {
        player.frame = player.direcao == 1 ? 12 : 13;
    } else if (dx != 0) {
        if (player.tempoAnimacao >= 0.10f) {
            player.tempoAnimacao = 0;
            player.frame++;

            if (player.frame < 1 || player.frame > 4) {
                player.frame = 1;
            }
        }
    } else {
        if (player.tempoAnimacao >= 0.25f) {
            player.tempoAnimacao = 0;
            player.frame = player.frame == 0 ? 5 : 0;
        }
    }

    Rectangle testeX = player.rect;
    testeX.x += dx;

    if (!checarColisaoMapa(testeX)) {
        player.rect.x = testeX.x;
    }

    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && player.noChao) {
        player.velY = JUMP_FORCE;
        player.noChao = 0;
    }

    player.velY += GRAVITY;

    Rectangle testeY = player.rect;
    testeY.y += player.velY;

    if (!checarColisaoMapa(testeY)) {
        player.rect.y = testeY.y;
        player.noChao = 0;
    } else {
        if (player.velY > 0) player.noChao = 1;
        player.velY = 0;
    }

    if (checarMortePorAbismo(player.rect) || player.rect.y > MAP_HEIGHT * TILE_SIZE) {
        perderVida();
    }

    atualizarCamera();
}

static void desenharPlayerLocal(void) {
    if (playerTexture.id > 0) {
        Rectangle source = {
            player.frame * 32,
            0,
            32 * player.direcao,
            32
        };

        DrawTexturePro(playerTexture, source, player.rect, (Vector2){0, 0}, 0, WHITE);
    } else {
        DrawRectangleRec(player.rect, GREEN);
    }
}

static void desenharHUD(void) {
    char texto[100];

    sprintf(texto, "Perfil: %s", perfis[perfilSelecionado]);
    DrawText(texto, 20, 20, 20, BLACK);

    sprintf(texto, "Fase: %d | Tela: %d/%d", faseAtual + 1, telaMapaAtual + 1, TELAS_POR_MAPA);
    DrawText(texto, 20, 45, 20, BLACK);

    sprintf(texto, "Vidas: %d", player.vidas);
    DrawText(texto, 20, 70, 20, BLACK);

    sprintf(texto, "Tempo: %.1fs", tempoFase);
    DrawText(texto, 20, 95, 20, BLACK);

    DrawText("P = sair | E = tunel/porta", 20, 120, 18, BLACK);
}

static void desenharMenuInicial(void) {
    DrawText("MANGUE RUN: LENDAS DO RECIFE", 360, 85, 36, corTexto());

    Rectangle novo = {490, 190, 300, 55};
    Rectangle carregar = {490, 270, 300, 55};
    Rectangle config = {490, 350, 300, 55};

    desenharBotao(novo, "Novo perfil", GOLD);
    desenharBotao(carregar, "Carregar perfil", GREEN);
    desenharBotao(config, "Configuracoes", SKYBLUE);

    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, novo) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) telaAtual = TELA_NOVO_PERFIL;
    if (CheckCollisionPointRec(mouse, carregar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) telaAtual = TELA_CARREGAR_PERFIL;
    if (CheckCollisionPointRec(mouse, config) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) telaAtual = TELA_CONFIGURACOES;
}

static void desenharSlotsPerfil(int criar) {
    DrawText(criar ? "NOVO PERFIL" : "CARREGAR PERFIL", 500, 70, 34, corTexto());

    for (int i = 0; i < PERFIL_MAX; i++) {
        Rectangle slot = {420, 150 + i * 80, 440, 55};
        Color cor = strcmp(perfis[i], "Vazio") == 0 ? LIGHTGRAY : GREEN;

        DrawRectangleRec(slot, cor);
        DrawRectangleLinesEx(slot, 2, BLACK);

        char texto[80];
        sprintf(texto, "Slot %d: %s", i + 1, perfis[i]);
        DrawText(texto, slot.x + 20, slot.y + 16, 22, BLACK);

        if (CheckCollisionPointRec(GetMousePosition(), slot) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (criar && strcmp(perfis[i], "Vazio") == 0) {
                sprintf(perfis[i], "Jogador %d", i + 1);
            }

            if (strcmp(perfis[i], "Vazio") != 0) {
                perfilSelecionado = i;
                telaAtual = TELA_MENU_FASES;
            }
        }
    }

    DrawText("BACKSPACE = voltar", 520, 500, 22, corTexto());

    if (IsKeyPressed(KEY_BACKSPACE)) {
        telaAtual = TELA_MENU_INICIAL;
    }
}

static void desenharConfiguracoes(void) {
    DrawText("CONFIGURACOES", 500, 100, 34, corTexto());

    Rectangle claro = {420, 210, 190, 60};
    Rectangle noturno = {660, 210, 220, 60};
    Rectangle voltar = {540, 330, 190, 55};

    desenharBotao(claro, "Modo claro", LIGHTGRAY);
    desenharBotao(noturno, "Modo noturno", DARKBLUE);
    desenharBotao(voltar, "Voltar", GOLD);

    DrawText(modoNoturno ? "Atual: modo noturno" : "Atual: modo claro", 500, 290, 24, corTexto());

    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, claro) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        modoNoturno = 0;
        salvarConfiguracoes();
    }

    if (CheckCollisionPointRec(mouse, noturno) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        modoNoturno = 1;
        salvarConfiguracoes();
    }

    if (CheckCollisionPointRec(mouse, voltar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        telaAtual = TELA_MENU_INICIAL;
    }
}

static void desenharMenuFases(void) {
    DrawText("SELECIONAR FASE", 485, 60, 34, corTexto());
    DrawText("Todas liberadas para teste.", 500, 110, 24, corTexto());

    for (int i = 0; i < TOTAL_MAPS; i++) {
        int x = 290 + (i % 5) * 140;
        int y = 190 + (i / 5) * 100;

        Rectangle botao = {x, y, 95, 55};
        DrawRectangleRec(botao, GREEN);
        DrawRectangleLinesEx(botao, 2, BLACK);

        char texto[20];
        sprintf(texto, "%d", i + 1);
        DrawText(texto, x + 38, y + 14, 26, BLACK);

        if (CheckCollisionPointRec(GetMousePosition(), botao) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            faseAtual = i;
            carregarFaseAtual();
            tempoCutscene = 0;
            telaAtual = TELA_CUTSCENE;
        }
    }

    DrawText("BACKSPACE = voltar", 535, 450, 22, corTexto());

    if (IsKeyPressed(KEY_BACKSPACE)) {
        telaAtual = TELA_MENU_INICIAL;
    }
}

static void desenharCutscene(void) {
    char tempo[50];

    DrawText(cutscenes[faseAtual][0], 470, 80, 34, corTexto());
    DrawText(cutscenes[faseAtual][1], 330, 180, 24, corTexto());
    DrawText(cutscenes[faseAtual][2], 330, 225, 24, corTexto());
    DrawText(cutscenes[faseAtual][3], 330, 270, 24, corTexto());

    sprintf(tempo, "A fase comeca em %.0f segundos", 10.0f - tempoCutscene);
    DrawText(tempo, 470, 420, 24, RED);
    DrawText("ENTER = pular cutscene", 510, 470, 20, corTexto());
}

static void desenharBackgroundFase(void) {
    if (faseAtual == 0 && background.id > 0) {
        DrawTexturePro(
            background,
            (Rectangle){0, 0, background.width, background.height},
            (Rectangle){0, 0, 1280, 576},
            (Vector2){0, 0},
            0,
            WHITE
        );
    } else {
        ClearBackground(WHITE);
    }
}

static void desenharTelaJogo(void) {
    BeginMode2D(camera);
    desenharBackgroundFase();
    DrawMap(faseAtual, telaMapaAtual, mapTextures);
    desenharInimigos(enemyShooter);
    desenharItens(heartTexture);
    desenharPlayerLocal();
    EndMode2D();

    desenharHUD();
}

static void desenharConfirmacaoSaida(void) {
    DrawRectangle(320, 210, 640, 250, RAYWHITE);
    DrawRectangleLines(320, 210, 640, 250, BLACK);
    DrawText("Tem certeza que deseja sair da missao atual?", 385, 250, 24, BLACK);

    Rectangle sim = {440, 340, 150, 60};
    Rectangle nao = {690, 340, 150, 60};

    desenharBotao(sim, "SIM", LIGHTGRAY);
    desenharBotao(nao, "NAO", LIGHTGRAY);

    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, sim) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        telaAtual = TELA_MENU_FASES;
    }

    if (CheckCollisionPointRec(mouse, nao) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        telaAtual = telaAnterior;
    }
}

static void desenharFaseCompleta(void) {
    char texto[100];

    DrawText("PARABENS!", 530, 160, 42, GREEN);
    DrawText("Voce completou a fase!", 470, 230, 30, corTexto());

    sprintf(texto, "Tempo: %.1f segundos", tempoFinalizado);
    DrawText(texto, 500, 310, 26, corTexto());

    sprintf(texto, "Vidas restantes: %d", vidasFinalizadas);
    DrawText(texto, 500, 350, 26, corTexto());

    DrawText("Voltando para o menu de fases...", 455, 430, 24, BLUE);
}

static void desenharGameOver(void) {
    DrawText("GAME OVER", 510, 220, 54, RED);
    DrawText("Voce perdeu as 3 vidas.", 500, 300, 28, corTexto());
    DrawText("Voltando para a tela de fases...", 455, 360, 26, BLUE);
}

static void concluirFase(void) {
    tempoFinalizado = tempoFase;
    vidasFinalizadas = player.vidas;

    tempoConclusao = 0;
    telaAtual = TELA_FASE_COMPLETA;
}

static void avancarTunel(void) {
    if (telaMapaAtual < TELAS_POR_MAPA - 1) {
        telaMapaAtual++;
        carregarParteAtual();
    }
}

static void atualizarTelaJogo(float dt) {
    tempoFase += dt;

    if (IsKeyPressed(KEY_P)) {
        telaAnterior = TELA_JOGO;
        telaAtual = TELA_CONFIRMAR_SAIDA;
        return;
    }

    atualizarPlayerLocal();

    if (telaAtual != TELA_JOGO) return;

    atualizarInimigos(player.rect, dt);
    atualizarItens(player.rect, &player.vidas);

    if (CheckCollisionRecs(player.rect, tunel) && IsKeyPressed(KEY_E)) {
        avancarTunel();
        return;
    }

    if (CheckCollisionRecs(player.rect, porta) && IsKeyPressed(KEY_E)) {
        concluirFase();
    }
}

void iniciarJogo(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mangue Run: Lendas do Recife");
    SetTargetFPS(60);

    carregarConfiguracoes();

    mapTextures.usarTextura = 0;

    playerTexture = LoadTexture("assets/player/playerMODEL.png");
    background = LoadTexture("assets/maps/01_boa_viagem/background.png");
    enemyShooter = LoadTexture("assets/maps/01_boa_viagem/enemy_shooter.png");
    heartTexture = LoadTexture("assets/items/heart.png");

    carregarFaseAtual();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(corFundo());

        if (telaAtual == TELA_MENU_INICIAL) {
            desenharMenuInicial();
        } else if (telaAtual == TELA_NOVO_PERFIL) {
            desenharSlotsPerfil(1);
        } else if (telaAtual == TELA_CARREGAR_PERFIL) {
            desenharSlotsPerfil(0);
        } else if (telaAtual == TELA_CONFIGURACOES) {
            desenharConfiguracoes();
        } else if (telaAtual == TELA_MENU_FASES) {
            desenharMenuFases();
        } else if (telaAtual == TELA_CUTSCENE) {
            tempoCutscene += dt;
            desenharCutscene();

            if (tempoCutscene >= 10.0f || IsKeyPressed(KEY_ENTER)) {
                carregarFaseAtual();
                telaAtual = TELA_JOGO;
            }
        } else if (telaAtual == TELA_JOGO) {
            atualizarTelaJogo(dt);
            desenharTelaJogo();
        } else if (telaAtual == TELA_CONFIRMAR_SAIDA) {
            desenharTelaJogo();
            desenharConfirmacaoSaida();
        } else if (telaAtual == TELA_FASE_COMPLETA) {
            tempoConclusao += dt;
            desenharFaseCompleta();

            if (tempoConclusao >= 5.0f) {
                telaAtual = TELA_MENU_FASES;
            }
        } else if (telaAtual == TELA_GAME_OVER) {
            tempoGameOver += dt;
            desenharGameOver();

            if (tempoGameOver >= 5.0f) {
                telaAtual = TELA_MENU_FASES;
            }
        }

        EndDrawing();
    }

    salvarConfiguracoes();
    UnloadMapTextures(&mapTextures);
    FreeEnemies(&enemyList);
    liberarItens();

    if (playerTexture.id > 0) UnloadTexture(playerTexture);
    if (background.id > 0) UnloadTexture(background);
    if (enemyShooter.id > 0) UnloadTexture(enemyShooter);
    if (heartTexture.id > 0) UnloadTexture(heartTexture);

    CloseWindow();
}