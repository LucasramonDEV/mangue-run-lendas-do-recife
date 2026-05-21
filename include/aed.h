#ifndef AED_H
#define AED_H

#define AED_HASH_SIZE 31
#define AED_MAX_NOME 50
#define AED_MAX_EVENTOS 100
#define AED_MAX_PILHA 50

#define AED_EVENTO_DANO 1
#define AED_EVENTO_TUNEL 2
#define AED_EVENTO_FASE_COMPLETA 3
#define AED_EVENTO_GAME_OVER 4

typedef struct PerfilHash {
    char nome[AED_MAX_NOME];
    int faseDesbloqueada;
    struct PerfilHash *prox;
} PerfilHash;

typedef struct FaseAVL {
    int id;
    int desbloqueada;
    int altura;
    struct FaseAVL *esq;
    struct FaseAVL *dir;
} FaseAVL;

typedef struct RankingAVL {
    char nome[AED_MAX_NOME];
    int pontuacao;
    float tempo;
    int altura;
    struct RankingAVL *esq;
    struct RankingAVL *dir;
} RankingAVL;

typedef struct EventoFila {
    int tipo;
    int valor;
} EventoFila;

typedef struct {
    EventoFila dados[AED_MAX_EVENTOS];
    int inicio;
    int fim;
    int qtd;
} FilaEventos;

typedef struct {
    int dados[AED_MAX_PILHA];
    int topo;
} PilhaTelas;

typedef struct TelaCircular {
    int tela;
    struct TelaCircular *prox;
} TelaCircular;

void AED_Inicializar(void);
void AED_Finalizar(void);

int AED_Hash(char *nome);
void AED_InserirPerfil(char *nome, int faseDesbloqueada);
PerfilHash *AED_BuscarPerfil(char *nome);
void AED_RemoverPerfil(char *nome);

FaseAVL *AED_InserirFase(FaseAVL *raiz, int id, int desbloqueada);
FaseAVL *AED_BuscarFase(FaseAVL *raiz, int id);
void AED_DesbloquearFase(FaseAVL *raiz, int id);
void AED_LiberarFases(FaseAVL *raiz);

RankingAVL *AED_InserirRanking(RankingAVL *raiz, char *nome, int pontuacao, float tempo);
void AED_LiberarRanking(RankingAVL *raiz);

void AED_InicializarFila(FilaEventos *fila);
int AED_FilaVazia(FilaEventos *fila);
int AED_Enfileirar(FilaEventos *fila, int tipo, int valor);
int AED_Desenfileirar(FilaEventos *fila, EventoFila *evento);

void AED_InicializarPilha(PilhaTelas *pilha);
int AED_PilhaVazia(PilhaTelas *pilha);
int AED_Empilhar(PilhaTelas *pilha, int tela);
int AED_Desempilhar(PilhaTelas *pilha, int *tela);

TelaCircular *AED_CriarListaCircularTelas(int qtd);
int AED_ValorTelaAtual(TelaCircular *atual);
int AED_AvancarTelaCircular(TelaCircular **atual);
void AED_LiberarListaCircular(TelaCircular *lista);

int AED_BuscaLinear(int v[], int n, int alvo);
int AED_BuscaBinaria(int v[], int n, int alvo);

void AED_QuickSort(int v[], int inicio, int fim);
void AED_MergeSort(int v[], int inicio, int fim);
void AED_HeapSort(int v[], int n);

#endif