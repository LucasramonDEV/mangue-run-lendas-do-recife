#include "game.h"
#include "raylib.h"
#include "map.h"
#include "enemy.h"
#include "projectile.h"
#include "item.h"
#include "aed.h"
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
#define NOME_PERFIL_MAX 6

#define FRAME_IDLE_1 0
#define FRAME_IDLE_2 1
#define FRAME_IDLE_3 2
#define FRAME_IDLE_4 3
#define FRAME_WALK_RIGHT_1 4
#define FRAME_WALK_RIGHT_4 7
#define FRAME_WALK_LEFT_1 8
#define FRAME_WALK_LEFT_4 11
#define FRAME_JUMP 12
#define FRAME_FALL 13
#define FRAME_DAMAGE 14
#define FRAME_DEAD 15
#define FRAME_ATTACK_RIGHT 16
#define FRAME_ATTACK_LEFT 17

typedef enum {
    TELA_MENU_INICIAL,
    TELA_NOVO_PERFIL,
    TELA_CARREGAR_PERFIL,
    TELA_DIGITAR_PERFIL,
    TELA_CONFIGURACOES,
    TELA_MENU_FASES,
    TELA_CUTSCENE,
    TELA_JOGO,
    TELA_CONFIRMAR_SAIDA,
    TELA_FASE_COMPLETA,
    TELA_GAME_OVER,
    TELA_RANK
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

typedef struct {
    int existe;
    float melhorTempo;
    int melhorPontuacao;
} RankingPerfil;

static Tela telaAtual = TELA_MENU_INICIAL;
static Tela telaAnterior = TELA_MENU_INICIAL;

static int faseAtual = 0;
static int modoNoturno = 0;
static int perfilSelecionado = 0;
static int faseRankAtual = 0;

static char perfis[PERFIL_MAX][NOME_PERFIL_MAX] = {"Teste", "Vazio", "Vazio", "Vazio"};
static int perfilFaseDesbloqueada[PERFIL_MAX] = {0, 0, 0, 0};
static char novoPerfilNome[NOME_PERFIL_MAX] = "";
static int novoPerfilTamanho = 0;
static char mensagemPerfil[80] = "";

static RankingPerfil rankingFases[TOTAL_MAPS][PERFIL_MAX];

static float tempoCutscene = 0;
static float tempoFase = 0;
static float tempoConclusao = 0;
static float tempoGameOver = 0;
static float tempoFinalizado = 0;
static int vidasFinalizadas = 0;
static float tempoDanoPlayer = 0.0f;

static PlayerLocal player;
static Rectangle porta;
static Rectangle tunel;
static Camera2D camera;

static Texture2D playerTexture;
static Texture2D backgrounds[TOTAL_MAPS][TELAS_POR_MAPA];
static Texture2D heartTexture;

static MapTextures mapTextures;

static FaseAVL *arvoreFases = NULL;
static RankingAVL *ranking = NULL;
static FilaEventos filaEventos;
static PilhaTelas pilhaCheckpoints;
static TelaCircular *listaTelas = NULL;
static TelaCircular *telaAtualCircular = NULL;
static int checkpointTelaAtual = 0;

static const char *nomesFases[TOTAL_MAPS] = {
    "Boa Viagem",
    "Casa Forte",
    "Jaqueira",
    "Gracas",
    "Bom Jesus",
    "Marco Zero"
};

static const char *cutscenes[TOTAL_MAPS][4] = {
    {"Praia de Boa Viagem", "Chico chega na orla apos ouvir um chamado estranho.", "Cada tunel leva para outra parte da praia.", "A ultima parte possui a porta final da missao."},
    {"Praca de Casa Forte", "A energia das lendas segue para Casa Forte.", "Os tuneis conectam pracas e caminhos diferentes.", "Continue avancando ate chegar na porta final."},
    {"Parque da Jaqueira", "Na Jaqueira, o caminho se divide entre baixo e alto.", "Cada tela representa uma parte do parque.", "Use os tuneis para atravessar todo o mapa."},
    {"Parque das Gracas", "As Gracas escondem uma rota tomada por sombras.", "As passagens ligam trechos diferentes da fase.", "A porta aparece somente na ultima tela."},
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
    if (!f) return;
    fprintf(f, "%d\n", modoNoturno);
    fclose(f);
}

static void carregarConfiguracoes(void) {
    FILE *f = fopen("config.txt", "r");
    if (!f) return;
    fscanf(f, "%d", &modoNoturno);
    fclose(f);
}

static void limparRankingPerfil(int perfil) {
    for (int fase = 0; fase < TOTAL_MAPS; fase++) {
        rankingFases[fase][perfil].existe = 0;
        rankingFases[fase][perfil].melhorTempo = 0;
        rankingFases[fase][perfil].melhorPontuacao = 0;
    }
}

static void carregarRankingArquivo(void) {
    FILE *f = fopen("ranking.txt", "r");

    for (int i = 0; i < TOTAL_MAPS; i++) {
        for (int j = 0; j < PERFIL_MAX; j++) {
            rankingFases[i][j].existe = 0;
            rankingFases[i][j].melhorTempo = 0;
            rankingFases[i][j].melhorPontuacao = 0;
        }
    }

    if (!f) return;

    for (int fase = 0; fase < TOTAL_MAPS; fase++) {
        for (int perfil = 0; perfil < PERFIL_MAX; perfil++) {
            fscanf(
                f,
                "%d %f %d",
                &rankingFases[fase][perfil].existe,
                &rankingFases[fase][perfil].melhorTempo,
                &rankingFases[fase][perfil].melhorPontuacao
            );
        }
    }

    fclose(f);
}

static void salvarRankingArquivo(void) {
    FILE *f = fopen("ranking.txt", "w");
    if (!f) return;

    for (int fase = 0; fase < TOTAL_MAPS; fase++) {
        for (int perfil = 0; perfil < PERFIL_MAX; perfil++) {
            fprintf(
                f,
                "%d %.2f %d\n",
                rankingFases[fase][perfil].existe,
                rankingFases[fase][perfil].melhorTempo,
                rankingFases[fase][perfil].melhorPontuacao
            );
        }
    }

    fclose(f);
}

static void salvarPerfis(void) {
    FILE *f = fopen("perfis.txt", "w");
    if (!f) return;

    for (int i = 0; i < PERFIL_MAX; i++) {
        fprintf(f, "%s %d\n", perfis[i], perfilFaseDesbloqueada[i]);
    }

    fclose(f);
}

static void carregarPerfisDoArquivo(void) {
    FILE *f = fopen("perfis.txt", "r");

    strcpy(perfis[0], "Teste");
    strcpy(perfis[1], "Vazio");
    strcpy(perfis[2], "Vazio");
    strcpy(perfis[3], "Vazio");

    perfilFaseDesbloqueada[0] = TOTAL_MAPS - 1;
    perfilFaseDesbloqueada[1] = 0;
    perfilFaseDesbloqueada[2] = 0;
    perfilFaseDesbloqueada[3] = 0;

    if (!f) {
        salvarPerfis();
    } else {
        for (int i = 0; i < PERFIL_MAX; i++) {
            char nomeTemp[NOME_PERFIL_MAX];
            int desbloqueadaTemp;

            if (fscanf(f, "%5s %d", nomeTemp, &desbloqueadaTemp) == 2) {
                strcpy(perfis[i], nomeTemp);
                perfilFaseDesbloqueada[i] = desbloqueadaTemp;

                if (perfilFaseDesbloqueada[i] < 0) perfilFaseDesbloqueada[i] = 0;
                if (perfilFaseDesbloqueada[i] >= TOTAL_MAPS) perfilFaseDesbloqueada[i] = TOTAL_MAPS - 1;
            }
        }

        fclose(f);
    }

    if (strcmp(perfis[0], "Vazio") == 0) {
        strcpy(perfis[0], "Teste");
        perfilFaseDesbloqueada[0] = 0;
    }
    perfilFaseDesbloqueada[0] = TOTAL_MAPS - 1;
    AED_InserirPerfil(perfis[0], TOTAL_MAPS - 1);
    
    for (int i = 0; i < PERFIL_MAX; i++) {
        if (strcmp(perfis[i], "Vazio") != 0) {
            AED_InserirPerfil(perfis[i], perfilFaseDesbloqueada[i]);
        }
    }
}

static int perfilExiste(char *nome) {
    for (int i = 0; i < PERFIL_MAX; i++) {
        if (strcmp(perfis[i], nome) == 0) return 1;
    }

    return AED_BuscarPerfil(nome) != NULL;
}

static int primeiroSlotVazio(void) {
    for (int i = 0; i < PERFIL_MAX; i++) {
        if (strcmp(perfis[i], "Vazio") == 0) return i;
    }

    return -1;
}

static void inicializarAEDJogo(void) {
    AED_Inicializar();
    AED_InicializarFila(&filaEventos);
    AED_InicializarPilha(&pilhaCheckpoints);

    for (int i = 0; i < TOTAL_MAPS; i++) {
        arvoreFases = AED_InserirFase(arvoreFases, i, 1);
    }

    listaTelas = AED_CriarListaCircularTelas(TELAS_POR_MAPA);
    telaAtualCircular = listaTelas;
}

static const char *pastasMapas[TOTAL_MAPS] = {
    "01_boa_viagem",
    "02_casa_forte",
    "03_jaqueira",
    "04_gracas",
    "05_bom_jesus",
    "06_marco_zero"
};

static const char *prefixosMapas[TOTAL_MAPS] = {
    "bv",
    "cf",
    "ja",
    "gc",
    "bm",
    "mz"
};

static void carregarBackgrounds(void) {
    char caminho[180];

    for (int f = 0; f < TOTAL_MAPS; f++) {
        for (int t = 0; t < TELAS_POR_MAPA; t++) {
            backgrounds[f][t].id = 0;
        }
    }

    for (int f = 0; f < TOTAL_MAPS; f++) {
        for (int t = 0; t < TELAS_POR_MAPA; t++) {
            sprintf(
                caminho,
                "assets/maps/%s/%s_background_%02d.png",
                pastasMapas[f],
                prefixosMapas[f],
                t + 1
            );

            backgrounds[f][t] = LoadTexture(caminho);
        }
    }
}

static void liberarBackgrounds(void) {
    for (int f = 0; f < TOTAL_MAPS; f++) {
        for (int t = 0; t < TELAS_POR_MAPA; t++) {
            if (backgrounds[f][t].id > 0) {
                UnloadTexture(backgrounds[f][t]);
            }
        }
    }
}

static void desenharBotao(Rectangle r, const char *texto, Color cor) {
    DrawRectangleRec(r, cor);
    DrawRectangleLinesEx(r, 2, BLACK);
    DrawText(texto, r.x + 18, r.y + 14, 22, BLACK);
}

static void atualizarCamera(void) {
    camera.target = (Vector2){
        player.rect.x + player.rect.width / 2,
        player.rect.y + player.rect.height / 2
    };

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
    telaMapaAtual = AED_ValorTelaAtual(telaAtualCircular);
    checkpointTelaAtual = telaMapaAtual;

    FindPlayerStart(faseAtual, telaMapaAtual, &playerRect);
    FindDoor(faseAtual, telaMapaAtual, &porta);
    FindTunnel(faseAtual, telaMapaAtual, &tunel);

    player.rect = playerRect;
    player.velY = 0;
    player.noChao = 0;
    player.direcao = 1;
    player.frame = FRAME_IDLE_1;
    player.tempoAnimacao = 0;
    player.atacando = 0;
    player.tempoAtaque = 0;

    AED_Empilhar(&pilhaCheckpoints, telaMapaAtual);

    LoadEnemiesFromMap(&enemyList, faseAtual);
    carregarItens();

    UnloadMapTextures(&mapTextures);
    LoadMapTextures(&mapTextures, faseAtual);

    atualizarCamera();
}

static void carregarFaseAtual(void) {
    if (listaTelas != NULL) telaAtualCircular = listaTelas;

    telaMapaAtual = 0;
    checkpointTelaAtual = 0;
    player.vidas = 3;
    tempoFase = 0;

    AED_InicializarPilha(&pilhaCheckpoints);
    AED_InicializarFila(&filaEventos);

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

    /*
        CORRECAO DO CHECKPOINT:

        Antes essa funcao usava AED_Desempilhar(&pilhaCheckpoints, &telaCheckpoint).
        Isso fazia a primeira morte voltar certo, mas removia o checkpoint da pilha.
        Na segunda morte, o topo da pilha passava a ser a tela anterior.

        Agora a morte nao desempilha checkpoint.
        O checkpoint atual fica salvo em checkpointTelaAtual.
        A pilha continua existindo como estrutura de AED do projeto,
        mas nao e consumida toda vez que o jogador morre.
    */

    telaMapaAtual = checkpointTelaAtual;

    FindPlayerStart(faseAtual, telaMapaAtual, &start);

    player.rect = start;
    player.velY = 0;
    player.noChao = 0;
    player.frame = FRAME_DAMAGE;
    player.atacando = 0;
    player.tempoAtaque = 0;
    tempoDanoPlayer = 1.0f;

    atualizarCamera();
}

static void perderVida(void) {
    AED_Enfileirar(&filaEventos, AED_EVENTO_DANO, 1);
}

static void aplicarDanoPlayer(void) {
    if (tempoDanoPlayer > 0.0f) return;

    tempoDanoPlayer = 1.0f;
    player.frame = FRAME_DAMAGE;
    perderVida();
}

static Rectangle getProjetilRect(Projectile *p) {
    return (Rectangle){p->x, p->y, 16, 8};
}

static void checarColisaoMeleeComPlayer(void) {
    Enemy *e = enemyList;

    while (e != NULL) {
        if (e->type == ENEMY_MELEE && CheckCollisionRecs(player.rect, e->rect)) {
            aplicarDanoPlayer();
            return;
        }

        e = e->next;
    }
}

static void checarColisaoProjeteis(void) {
    Projectile *p = getListaProjeteis();

    while (p != NULL) {
        Projectile *proximo = p->prox;
        Rectangle projetilRect = getProjetilRect(p);

        if (p->tipo == PROJETIL_PERSONAGEM) {
            Enemy *e = enemyList;

            while (e != NULL) {
                if (CheckCollisionRecs(projetilRect, e->rect)) {
                    e->life = 0;
                    removerProjetil(p);
                    break;
                }

                e = e->next;
            }
        } else if (p->tipo == PROJETIL_INIMIGO) {
            if (CheckCollisionRecs(projetilRect, player.rect)) {
                removerProjetil(p);
                aplicarDanoPlayer();
            }
        }

        p = proximo;
    }

    RemoveDeadEnemies(&enemyList);
}

static void atualizarCombate(float dt) {
    if (tempoDanoPlayer > 0.0f) {
        tempoDanoPlayer -= dt;
        if (tempoDanoPlayer < 0.0f) tempoDanoPlayer = 0.0f;
    }

    checarColisaoMeleeComPlayer();
    checarColisaoProjeteis();
}

static void atualizarAnimacaoPlayer(float dx, float dt) {
    player.tempoAnimacao += dt;

    if (player.atacando) {
        player.frame = player.direcao == 1 ? FRAME_ATTACK_RIGHT : FRAME_ATTACK_LEFT;
        return;
    }

    if (!player.noChao && player.velY < 0) {
        player.frame = FRAME_JUMP;
        return;
    }

    if (!player.noChao && player.velY > 0) {
        player.frame = FRAME_FALL;
        return;
    }

    if (dx > 0) {
        if (player.tempoAnimacao >= 0.10f) {
            player.tempoAnimacao = 0;
            player.frame++;
            if (player.frame < FRAME_WALK_RIGHT_1 || player.frame > FRAME_WALK_RIGHT_4) {
                player.frame = FRAME_WALK_RIGHT_1;
            }
        }
        return;
    }

    if (dx < 0) {
        if (player.tempoAnimacao >= 0.10f) {
            player.tempoAnimacao = 0;
            player.frame++;
            if (player.frame < FRAME_WALK_LEFT_1 || player.frame > FRAME_WALK_LEFT_4) {
                player.frame = FRAME_WALK_LEFT_1;
            }
        }
        return;
    }

    if (player.tempoAnimacao >= 0.22f) {
        player.tempoAnimacao = 0;
        player.frame++;
        if (player.frame < FRAME_IDLE_1 || player.frame > FRAME_IDLE_4) {
            player.frame = FRAME_IDLE_1;
        }
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

    if (IsKeyPressed(KEY_K)) {
        player.atacando = 1;
        player.tempoAtaque = 0.25f;
        player.direcao = -1;
        dispararProjetil(player.rect.x, player.rect.y + player.rect.height / 2.0f, -1, PROJETIL_PERSONAGEM);
    }

    if (IsKeyPressed(KEY_L)) {
        player.atacando = 1;
        player.tempoAtaque = 0.25f;
        player.direcao = 1;
        dispararProjetil(player.rect.x + player.rect.width, player.rect.y + player.rect.height / 2.0f, 1, PROJETIL_PERSONAGEM);
    }

    if (player.atacando) {
        player.tempoAtaque -= dt;
        if (player.tempoAtaque <= 0) player.atacando = 0;
    }

    Rectangle testeX = player.rect;
    testeX.x += dx;

    if (!checarColisaoMapa(testeX)) player.rect.x = testeX.x;

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

    atualizarAnimacaoPlayer(dx, dt);

    if (checarMortePorAbismo(player.rect) || player.rect.y > MAP_HEIGHT * TILE_SIZE) {
        player.frame = FRAME_DAMAGE;
        perderVida();
    }

    atualizarCamera();
}

static void desenharPlayerLocal(void) {
    if (playerTexture.id > 0) {
        Rectangle source = {player.frame * 32, 0, 32, 32};
        DrawTexturePro(playerTexture, source, player.rect, (Vector2){0, 0}, 0, WHITE);
    } else {
        DrawRectangleRec(player.rect, GREEN);
    }
}

static void desenharHUD(void) {
    char texto[100];

    sprintf(texto, "Perfil: %s", perfis[perfilSelecionado]);
    DrawText(texto, 20, 20, 20, BLACK);

    sprintf(texto, "Fase: %d/%d - %s | Tela: %d/%d", faseAtual + 1, TOTAL_MAPS, nomesFases[faseAtual], telaMapaAtual + 1, TELAS_POR_MAPA);
    DrawText(texto, 20, 45, 20, BLACK);

    sprintf(texto, "Vidas: %d", player.vidas);
    DrawText(texto, 20, 70, 20, BLACK);

    sprintf(texto, "Tempo: %.1fs", tempoFase);
    DrawText(texto, 20, 95, 20, BLACK);

    DrawText("P = sair | E = tunel/porta | K/L = ataque", 20, 120, 18, BLACK);
}

static void desenharMenuInicial(void) {
    DrawText("MANGUE RUN: LENDAS DO RECIFE", 360, 70, 36, corTexto());

    Rectangle novo = {490, 160, 300, 55};
    Rectangle carregar = {490, 235, 300, 55};
    Rectangle config = {490, 310, 300, 55};
    Rectangle rank = {490, 385, 300, 55};

    desenharBotao(novo, "Novo perfil", GOLD);
    desenharBotao(carregar, "Carregar perfil", GREEN);
    desenharBotao(config, "Configuracoes", SKYBLUE);
    desenharBotao(rank, "Rank", ORANGE);

    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, novo) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        novoPerfilNome[0] = '\0';
        novoPerfilTamanho = 0;
        mensagemPerfil[0] = '\0';
        telaAtual = TELA_DIGITAR_PERFIL;
    }

    if (CheckCollisionPointRec(mouse, carregar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) telaAtual = TELA_CARREGAR_PERFIL;
    if (CheckCollisionPointRec(mouse, config) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) telaAtual = TELA_CONFIGURACOES;
    if (CheckCollisionPointRec(mouse, rank) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) telaAtual = TELA_RANK;
}

static void desenharTelaDigitarPerfil(void) {
    DrawText("CRIAR NOVO PERFIL", 450, 90, 34, corTexto());
    DrawText("Digite ate 5 caracteres e pressione ENTER", 390, 160, 24, corTexto());

    Rectangle campo = {470, 230, 340, 65};
    DrawRectangleRec(campo, RAYWHITE);
    DrawRectangleLinesEx(campo, 2, BLACK);
    DrawText(novoPerfilNome, campo.x + 20, campo.y + 18, 28, BLACK);

    DrawText(mensagemPerfil, 405, 320, 22, RED);
    DrawText("BACKSPACE apaga | ESC volta", 455, 390, 22, corTexto());

    int tecla = GetCharPressed();

    while (tecla > 0) {
        if (tecla >= 33 && tecla <= 126 && novoPerfilTamanho < 5) {
            novoPerfilNome[novoPerfilTamanho] = (char)tecla;
            novoPerfilTamanho++;
            novoPerfilNome[novoPerfilTamanho] = '\0';
        }

        tecla = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && novoPerfilTamanho > 0) {
        novoPerfilTamanho--;
        novoPerfilNome[novoPerfilTamanho] = '\0';
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        telaAtual = TELA_MENU_INICIAL;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        int slot = primeiroSlotVazio();

        if (novoPerfilTamanho == 0) {
            strcpy(mensagemPerfil, "Nome vazio nao permitido.");
        } else if (strcmp(novoPerfilNome, "Vazio") == 0) {
            strcpy(mensagemPerfil, "Esse nome e reservado.");
        } else if (perfilExiste(novoPerfilNome)) {
            strcpy(mensagemPerfil, "Esse nick ja existe.");
        } else if (slot == -1) {
            strcpy(mensagemPerfil, "Limite de 4 perfis atingido.");
        } else {
            strcpy(perfis[slot], novoPerfilNome);
            perfilFaseDesbloqueada[slot] = 0;
            AED_InserirPerfil(perfis[slot], 0);
            salvarPerfis();

            perfilSelecionado = slot;
            telaAtual = TELA_MENU_FASES;
        }
    }
}

static void desenharSlotsPerfil(int criar) {
    DrawText(criar ? "NOVO PERFIL" : "CARREGAR PERFIL", 500, 70, 34, corTexto());

    for (int i = 0; i < PERFIL_MAX; i++) {
        Rectangle slot = {390, 150 + i * 80, 420, 55};
        Rectangle botaoExcluir = {830, 150 + i * 80, 55, 55};

        Color cor = strcmp(perfis[i], "Vazio") == 0 ? LIGHTGRAY : GREEN;

        DrawRectangleRec(slot, cor);
        DrawRectangleLinesEx(slot, 2, BLACK);

        char texto[100];
        sprintf(texto, "Slot %d: %s", i + 1, perfis[i]);
        DrawText(texto, slot.x + 20, slot.y + 9, 20, BLACK);

        if (strcmp(perfis[i], "Vazio") != 0) {
            sprintf(texto, "Liberada: fase %d", perfilFaseDesbloqueada[i] + 1);
            DrawText(texto, slot.x + 20, slot.y + 31, 17, BLACK);
        }

        if (!criar && strcmp(perfis[i], "Vazio") != 0) {
            DrawRectangleRec(botaoExcluir, RED);
            DrawRectangleLinesEx(botaoExcluir, 2, BLACK);
            DrawText("X", botaoExcluir.x + 18, botaoExcluir.y + 13, 26, WHITE);
        }

        if (CheckCollisionPointRec(GetMousePosition(), slot) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (criar) {
                telaAtual = TELA_DIGITAR_PERFIL;
                return;
            }

            if (strcmp(perfis[i], "Vazio") != 0) {
                PerfilHash *p = AED_BuscarPerfil(perfis[i]);

                if (p != NULL) {
                    perfilSelecionado = i;
                    perfilFaseDesbloqueada[i] = p->faseDesbloqueada;
                    telaAtual = TELA_MENU_FASES;
                }
            }
        }

        if (!criar &&
            strcmp(perfis[i], "Vazio") != 0 &&
            CheckCollisionPointRec(GetMousePosition(), botaoExcluir) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            AED_RemoverPerfil(perfis[i]);
            strcpy(perfis[i], "Vazio");
            perfilFaseDesbloqueada[i] = 0;
            limparRankingPerfil(i);
            salvarPerfis();
            salvarRankingArquivo();

            if (perfilSelecionado == i) perfilSelecionado = 0;
        }
    }

    DrawText("BACKSPACE = voltar", 520, 500, 22, corTexto());
    if (IsKeyPressed(KEY_BACKSPACE)) telaAtual = TELA_MENU_INICIAL;
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
    DrawText("SELECIONAR MISSAO", 465, 60, 34, corTexto());

    for (int i = 0; i < TOTAL_MAPS; i++) {
        int x = 250 + (i % 3) * 260;
        int y = 170 + (i / 3) * 120;

        FaseAVL *fase = AED_BuscarFase(arvoreFases, i);
        int liberada = fase != NULL && fase->desbloqueada && i <= perfilFaseDesbloqueada[perfilSelecionado];

        Rectangle botao = {x, y, 210, 70};
        DrawRectangleRec(botao, liberada ? GREEN : GRAY);
        DrawRectangleLinesEx(botao, 2, BLACK);

        char texto[80];
        sprintf(texto, "%02d - %s", i + 1, nomesFases[i]);
        DrawText(texto, x + 15, y + 22, 20, BLACK);

        if (liberada && CheckCollisionPointRec(GetMousePosition(), botao) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            faseAtual = i;
            carregarFaseAtual();
            tempoCutscene = 0;
            telaAtual = TELA_CUTSCENE;
        }
    }

    DrawText("Cada perfil desbloqueia missoes individualmente", 395, 430, 22, corTexto());
    DrawText("BACKSPACE = voltar", 535, 465, 22, corTexto());

    if (IsKeyPressed(KEY_BACKSPACE)) telaAtual = TELA_MENU_INICIAL;
}

static void desenharTelaRank(void) {
    char texto[160];

    int pontuacoesOrdenadas[PERFIL_MAX];
    int usado[PERFIL_MAX];

    for (int i = 0; i < PERFIL_MAX; i++) {
        usado[i] = 0;

        if (strcmp(perfis[i], "Vazio") != 0 && rankingFases[faseRankAtual][i].existe) {
            pontuacoesOrdenadas[i] = rankingFases[faseRankAtual][i].melhorPontuacao;
        } else {
            pontuacoesOrdenadas[i] = -1;
        }
    }

    AED_QuickSort(pontuacoesOrdenadas, 0, PERFIL_MAX - 1);

    DrawText("RANKING POR MISSAO", 425, 50, 36, corTexto());

    Rectangle anterior = {250, 115, 180, 50};
    Rectangle proxima = {850, 115, 180, 50};
    Rectangle voltar = {530, 600, 220, 55};

    desenharBotao(anterior, "< Missao", LIGHTGRAY);
    desenharBotao(proxima, "Missao >", LIGHTGRAY);
    desenharBotao(voltar, "Voltar", GOLD);

    sprintf(texto, "%02d - %s", faseRankAtual + 1, nomesFases[faseRankAtual]);
    DrawText(texto, 510, 125, 25, corTexto());

    DrawText("Pos.", 230, 205, 24, corTexto());
    DrawText("Perfil", 320, 205, 24, corTexto());
    DrawText("Melhor tempo", 560, 205, 24, corTexto());
    DrawText("Pontuacao", 830, 205, 24, corTexto());

    int linha = 0;

    for (int p = PERFIL_MAX - 1; p >= 0; p--) {
        int pontuacaoAtual = pontuacoesOrdenadas[p];

        if (pontuacaoAtual < 0) {
            continue;
        }

        int perfilEncontrado = -1;

        for (int i = 0; i < PERFIL_MAX; i++) {
            if (!usado[i] &&
                strcmp(perfis[i], "Vazio") != 0 &&
                rankingFases[faseRankAtual][i].existe &&
                rankingFases[faseRankAtual][i].melhorPontuacao == pontuacaoAtual) {
                perfilEncontrado = i;
                usado[i] = 1;
                break;
            }
        }

        if (perfilEncontrado != -1) {
            int y = 260 + linha * 65;

            sprintf(texto, "%d", linha + 1);
            DrawText(texto, 240, y, 24, corTexto());

            DrawText(perfis[perfilEncontrado], 320, y, 24, corTexto());

            sprintf(texto, "%.2fs", rankingFases[faseRankAtual][perfilEncontrado].melhorTempo);
            DrawText(texto, 590, y, 24, corTexto());

            sprintf(texto, "%d", rankingFases[faseRankAtual][perfilEncontrado].melhorPontuacao);
            DrawText(texto, 870, y, 24, corTexto());

            linha++;
        }
    }

    if (linha == 0) {
        DrawText("Nenhum ranking registrado para esta missao.", 390, 300, 24, corTexto());
    }

    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, anterior) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        faseRankAtual--;

        if (faseRankAtual < 0) {
            faseRankAtual = TOTAL_MAPS - 1;
        }
    }

    if (CheckCollisionPointRec(mouse, proxima) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        faseRankAtual++;

        if (faseRankAtual >= TOTAL_MAPS) {
            faseRankAtual = 0;
        }
    }

    if (CheckCollisionPointRec(mouse, voltar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        telaAtual = TELA_MENU_INICIAL;
    }

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
    Texture2D bg = backgrounds[faseAtual][telaMapaAtual];

    if (bg.id > 0) {
        DrawTexturePro(
            bg,
            (Rectangle){0, 0, bg.width, bg.height},
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
    desenharItens(heartTexture);
    desenharInimigos();
    desenharPlayerLocal();
    desenharProjeteis((Texture2D){0}, (Texture2D){0});
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

    if (CheckCollisionPointRec(mouse, sim) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) telaAtual = TELA_MENU_FASES;
    if (CheckCollisionPointRec(mouse, nao) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) telaAtual = telaAnterior;
}

static void atualizarRankingFase(void) {
    int pontuacao = (vidasFinalizadas * 1000) + (int)(1000 - tempoFinalizado);
    if (pontuacao < 0) pontuacao = vidasFinalizadas * 1000;

    RankingPerfil *r = &rankingFases[faseAtual][perfilSelecionado];

    if (!r->existe || tempoFinalizado < r->melhorTempo) {
        r->existe = 1;
        r->melhorTempo = tempoFinalizado;
        r->melhorPontuacao = pontuacao;
        salvarRankingArquivo();
    }
}

static void desenharFaseCompleta(void) {
    char texto[100];

    DrawText("PARABENS!", 530, 160, 42, GREEN);
    DrawText("Voce completou a missao!", 455, 230, 30, corTexto());

    sprintf(texto, "Tempo: %.1f segundos", tempoFinalizado);
    DrawText(texto, 500, 310, 26, corTexto());

    sprintf(texto, "Vidas restantes: %d", vidasFinalizadas);
    DrawText(texto, 500, 350, 26, corTexto());

    DrawText("Ranking salvo e proxima missao liberada", 410, 390, 24, BLUE);
}

static void desenharGameOver(void) {
    DrawText("GAME OVER", 510, 220, 54, RED);
    DrawText("Voce perdeu as 3 vidas.", 500, 300, 28, corTexto());
    DrawText("Voltando para a tela de missoes...", 440, 360, 26, BLUE);
}

static void concluirFase(void) {
    tempoFinalizado = tempoFase;
    vidasFinalizadas = player.vidas;

    atualizarRankingFase();

    ranking = AED_InserirRanking(
        ranking,
        perfis[perfilSelecionado],
        rankingFases[faseAtual][perfilSelecionado].melhorPontuacao,
        tempoFinalizado
    );

    if (faseAtual + 1 < TOTAL_MAPS && perfilFaseDesbloqueada[perfilSelecionado] < faseAtual + 1) {
        perfilFaseDesbloqueada[perfilSelecionado] = faseAtual + 1;
        AED_InserirPerfil(perfis[perfilSelecionado], perfilFaseDesbloqueada[perfilSelecionado]);
        salvarPerfis();
    }

    AED_Enfileirar(&filaEventos, AED_EVENTO_FASE_COMPLETA, faseAtual);

    tempoConclusao = 0;
    telaAtual = TELA_FASE_COMPLETA;
}

static void avancarTunel(void) {
    if (telaMapaAtual < TELAS_POR_MAPA - 1) {
        AED_Enfileirar(&filaEventos, AED_EVENTO_TUNEL, telaMapaAtual);
    }
}

static void processarEventosAED(void) {
    EventoFila evento;

    while (AED_Desenfileirar(&filaEventos, &evento)) {
        if (evento.tipo == AED_EVENTO_DANO) {
            player.vidas--;

            if (player.vidas <= 0) {
                player.frame = FRAME_DEAD;
                tempoGameOver = 0;
                telaAtual = TELA_GAME_OVER;
            } else {
                reiniciarPosicaoPlayer();
            }
        } else if (evento.tipo == AED_EVENTO_TUNEL) {
            if (telaMapaAtual < TELAS_POR_MAPA - 1) {
                telaMapaAtual = AED_AvancarTelaCircular(&telaAtualCircular);
                carregarParteAtual();
            }
        }
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
    atualizarProjeteis(dt);
    atualizarCombate(dt);
    atualizarItens(player.rect, &player.vidas);

    if (CheckCollisionRecs(player.rect, tunel) && IsKeyPressed(KEY_E)) {
        avancarTunel();
    }

    if (CheckCollisionRecs(player.rect, porta) && IsKeyPressed(KEY_E)) {
        concluirFase();
    }

    processarEventosAED();
}

void iniciarJogo(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mangue Run: Lendas do Recife");
    SetTargetFPS(60);

    inicializarAEDJogo();
    carregarConfiguracoes();
    carregarPerfisDoArquivo();
    carregarRankingArquivo();

    mapTextures.usarTextura = 0;

    playerTexture = LoadTexture("assets/player/playerMODEL.png");
    heartTexture = LoadTexture("assets/items/heart.png");
    carregarTexturasInimigos();

    carregarBackgrounds();
    carregarFaseAtual();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(corFundo());

        if (telaAtual == TELA_MENU_INICIAL) desenharMenuInicial();
        else if (telaAtual == TELA_NOVO_PERFIL) desenharSlotsPerfil(1);
        else if (telaAtual == TELA_DIGITAR_PERFIL) desenharTelaDigitarPerfil();
        else if (telaAtual == TELA_CARREGAR_PERFIL) desenharSlotsPerfil(0);
        else if (telaAtual == TELA_CONFIGURACOES) desenharConfiguracoes();
        else if (telaAtual == TELA_MENU_FASES) desenharMenuFases();
        else if (telaAtual == TELA_RANK) desenharTelaRank();
        else if (telaAtual == TELA_CUTSCENE) {
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

            if (tempoConclusao >= 5.0f) telaAtual = TELA_MENU_FASES;
        } else if (telaAtual == TELA_GAME_OVER) {
            tempoGameOver += dt;
            desenharGameOver();

            if (tempoGameOver >= 5.0f) telaAtual = TELA_MENU_FASES;
        }

        EndDrawing();
    }

    salvarConfiguracoes();
    salvarPerfis();
    salvarRankingArquivo();

    UnloadMapTextures(&mapTextures);
    FreeEnemies(&enemyList);
    liberarItens();

    liberarBackgrounds();

    AED_LiberarFases(arvoreFases);
    AED_LiberarRanking(ranking);
    AED_LiberarListaCircular(listaTelas);
    AED_Finalizar();

    if (playerTexture.id > 0) UnloadTexture(playerTexture);
    if (heartTexture.id > 0) UnloadTexture(heartTexture);
    descarregarTexturasInimigos();

    CloseWindow();
}
