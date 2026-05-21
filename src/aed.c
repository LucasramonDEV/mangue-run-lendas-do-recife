#include "aed.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static PerfilHash *tabelaPerfis[AED_HASH_SIZE];

static int alturaFase(FaseAVL *n) {
    return n == NULL ? 0 : n->altura;
}

static int alturaRanking(RankingAVL *n) {
    return n == NULL ? 0 : n->altura;
}

static int maior(int a, int b) {
    return a > b ? a : b;
}

void AED_Inicializar(void) {
    for (int i = 0; i < AED_HASH_SIZE; i++) {
        tabelaPerfis[i] = NULL;
    }
}

void AED_Finalizar(void) {
    for (int i = 0; i < AED_HASH_SIZE; i++) {
        PerfilHash *p = tabelaPerfis[i];

        while (p != NULL) {
            PerfilHash *tmp = p;
            p = p->prox;
            free(tmp);
        }

        tabelaPerfis[i] = NULL;
    }
}

int AED_Hash(char *nome) {
    int soma = 0;

    for (int i = 0; nome[i] != '\0'; i++) {
        soma += nome[i] * (i + 1);
    }

    return soma % AED_HASH_SIZE;
}

void AED_InserirPerfil(char *nome, int faseDesbloqueada) {
    int pos = AED_Hash(nome);
    PerfilHash *existente = AED_BuscarPerfil(nome);

    if (existente != NULL) {
        existente->faseDesbloqueada = faseDesbloqueada;
        return;
    }

    PerfilHash *novo = malloc(sizeof(PerfilHash));
    if (novo == NULL) return;

    strcpy(novo->nome, nome);
    novo->faseDesbloqueada = faseDesbloqueada;
    novo->prox = tabelaPerfis[pos];
    tabelaPerfis[pos] = novo;
}

PerfilHash *AED_BuscarPerfil(char *nome) {
    int pos = AED_Hash(nome);
    PerfilHash *p = tabelaPerfis[pos];

    while (p != NULL) {
        if (strcmp(p->nome, nome) == 0) return p;
        p = p->prox;
    }

    return NULL;
}

void AED_RemoverPerfil(char *nome) {
    int pos = AED_Hash(nome);
    PerfilHash *p = tabelaPerfis[pos];
    PerfilHash *ant = NULL;

    while (p != NULL) {
        if (strcmp(p->nome, nome) == 0) {
            if (ant == NULL) tabelaPerfis[pos] = p->prox;
            else ant->prox = p->prox;

            free(p);
            return;
        }

        ant = p;
        p = p->prox;
    }
}

/* AVL DE FASES */

static FaseAVL *novoNoFase(int id, int desbloqueada) {
    FaseAVL *n = malloc(sizeof(FaseAVL));
    if (n == NULL) return NULL;

    n->id = id;
    n->desbloqueada = desbloqueada;
    n->altura = 1;
    n->esq = NULL;
    n->dir = NULL;

    return n;
}

static FaseAVL *rotacaoDireitaFase(FaseAVL *y) {
    FaseAVL *x = y->esq;
    FaseAVL *t2 = x->dir;

    x->dir = y;
    y->esq = t2;

    y->altura = maior(alturaFase(y->esq), alturaFase(y->dir)) + 1;
    x->altura = maior(alturaFase(x->esq), alturaFase(x->dir)) + 1;

    return x;
}

static FaseAVL *rotacaoEsquerdaFase(FaseAVL *x) {
    FaseAVL *y = x->dir;
    FaseAVL *t2 = y->esq;

    y->esq = x;
    x->dir = t2;

    x->altura = maior(alturaFase(x->esq), alturaFase(x->dir)) + 1;
    y->altura = maior(alturaFase(y->esq), alturaFase(y->dir)) + 1;

    return y;
}

static int fatorFase(FaseAVL *n) {
    if (n == NULL) return 0;
    return alturaFase(n->esq) - alturaFase(n->dir);
}

FaseAVL *AED_InserirFase(FaseAVL *raiz, int id, int desbloqueada) {
    if (raiz == NULL) return novoNoFase(id, desbloqueada);

    if (id < raiz->id) raiz->esq = AED_InserirFase(raiz->esq, id, desbloqueada);
    else if (id > raiz->id) raiz->dir = AED_InserirFase(raiz->dir, id, desbloqueada);
    else {
        raiz->desbloqueada = desbloqueada;
        return raiz;
    }

    raiz->altura = 1 + maior(alturaFase(raiz->esq), alturaFase(raiz->dir));

    int fb = fatorFase(raiz);

    if (fb > 1 && id < raiz->esq->id) return rotacaoDireitaFase(raiz);
    if (fb < -1 && id > raiz->dir->id) return rotacaoEsquerdaFase(raiz);

    if (fb > 1 && id > raiz->esq->id) {
        raiz->esq = rotacaoEsquerdaFase(raiz->esq);
        return rotacaoDireitaFase(raiz);
    }

    if (fb < -1 && id < raiz->dir->id) {
        raiz->dir = rotacaoDireitaFase(raiz->dir);
        return rotacaoEsquerdaFase(raiz);
    }

    return raiz;
}

FaseAVL *AED_BuscarFase(FaseAVL *raiz, int id) {
    if (raiz == NULL) return NULL;
    if (id == raiz->id) return raiz;
    if (id < raiz->id) return AED_BuscarFase(raiz->esq, id);
    return AED_BuscarFase(raiz->dir, id);
}

void AED_DesbloquearFase(FaseAVL *raiz, int id) {
    FaseAVL *f = AED_BuscarFase(raiz, id);
    if (f != NULL) f->desbloqueada = 1;
}

void AED_LiberarFases(FaseAVL *raiz) {
    if (raiz == NULL) return;

    AED_LiberarFases(raiz->esq);
    AED_LiberarFases(raiz->dir);
    free(raiz);
}

/* AVL RANKING */

static RankingAVL *novoNoRanking(char *nome, int pontuacao, float tempo) {
    RankingAVL *n = malloc(sizeof(RankingAVL));
    if (n == NULL) return NULL;

    strcpy(n->nome, nome);
    n->pontuacao = pontuacao;
    n->tempo = tempo;
    n->altura = 1;
    n->esq = NULL;
    n->dir = NULL;

    return n;
}

static RankingAVL *rotacaoDireitaRanking(RankingAVL *y) {
    RankingAVL *x = y->esq;
    RankingAVL *t2 = x->dir;

    x->dir = y;
    y->esq = t2;

    y->altura = maior(alturaRanking(y->esq), alturaRanking(y->dir)) + 1;
    x->altura = maior(alturaRanking(x->esq), alturaRanking(x->dir)) + 1;

    return x;
}

static RankingAVL *rotacaoEsquerdaRanking(RankingAVL *x) {
    RankingAVL *y = x->dir;
    RankingAVL *t2 = y->esq;

    y->esq = x;
    x->dir = t2;

    x->altura = maior(alturaRanking(x->esq), alturaRanking(x->dir)) + 1;
    y->altura = maior(alturaRanking(y->esq), alturaRanking(y->dir)) + 1;

    return y;
}

static int fatorRanking(RankingAVL *n) {
    if (n == NULL) return 0;
    return alturaRanking(n->esq) - alturaRanking(n->dir);
}

RankingAVL *AED_InserirRanking(RankingAVL *raiz, char *nome, int pontuacao, float tempo) {
    if (raiz == NULL) return novoNoRanking(nome, pontuacao, tempo);

    if (pontuacao < raiz->pontuacao) {
        raiz->esq = AED_InserirRanking(raiz->esq, nome, pontuacao, tempo);
    } else {
        raiz->dir = AED_InserirRanking(raiz->dir, nome, pontuacao, tempo);
    }

    raiz->altura = 1 + maior(alturaRanking(raiz->esq), alturaRanking(raiz->dir));

    int fb = fatorRanking(raiz);

    if (fb > 1 && pontuacao < raiz->esq->pontuacao) return rotacaoDireitaRanking(raiz);
    if (fb < -1 && pontuacao >= raiz->dir->pontuacao) return rotacaoEsquerdaRanking(raiz);

    if (fb > 1 && pontuacao >= raiz->esq->pontuacao) {
        raiz->esq = rotacaoEsquerdaRanking(raiz->esq);
        return rotacaoDireitaRanking(raiz);
    }

    if (fb < -1 && pontuacao < raiz->dir->pontuacao) {
        raiz->dir = rotacaoDireitaRanking(raiz->dir);
        return rotacaoEsquerdaRanking(raiz);
    }

    return raiz;
}

void AED_LiberarRanking(RankingAVL *raiz) {
    if (raiz == NULL) return;

    AED_LiberarRanking(raiz->esq);
    AED_LiberarRanking(raiz->dir);
    free(raiz);
}

/* FILA */

void AED_InicializarFila(FilaEventos *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->qtd = 0;
}

int AED_FilaVazia(FilaEventos *fila) {
    return fila->qtd == 0;
}

int AED_Enfileirar(FilaEventos *fila, int tipo, int valor) {
    if (fila->qtd == AED_MAX_EVENTOS) return 0;

    fila->dados[fila->fim].tipo = tipo;
    fila->dados[fila->fim].valor = valor;
    fila->fim = (fila->fim + 1) % AED_MAX_EVENTOS;
    fila->qtd++;

    return 1;
}

int AED_Desenfileirar(FilaEventos *fila, EventoFila *evento) {
    if (AED_FilaVazia(fila)) return 0;

    *evento = fila->dados[fila->inicio];
    fila->inicio = (fila->inicio + 1) % AED_MAX_EVENTOS;
    fila->qtd--;

    return 1;
}

/* PILHA */

void AED_InicializarPilha(PilhaTelas *pilha) {
    pilha->topo = -1;
}

int AED_PilhaVazia(PilhaTelas *pilha) {
    return pilha->topo == -1;
}

int AED_Empilhar(PilhaTelas *pilha, int tela) {
    if (pilha->topo == AED_MAX_PILHA - 1) return 0;

    pilha->topo++;
    pilha->dados[pilha->topo] = tela;

    return 1;
}

int AED_Desempilhar(PilhaTelas *pilha, int *tela) {
    if (AED_PilhaVazia(pilha)) return 0;

    *tela = pilha->dados[pilha->topo];
    pilha->topo--;

    return 1;
}

/* LISTA CIRCULAR */

TelaCircular *AED_CriarListaCircularTelas(int qtd) {
    if (qtd <= 0) return NULL;

    TelaCircular *inicio = NULL;
    TelaCircular *anterior = NULL;

    for (int i = 0; i < qtd; i++) {
        TelaCircular *novo = malloc(sizeof(TelaCircular));
        if (novo == NULL) return inicio;

        novo->tela = i;
        novo->prox = NULL;

        if (inicio == NULL) inicio = novo;
        else anterior->prox = novo;

        anterior = novo;
    }

    anterior->prox = inicio;
    return inicio;
}

int AED_ValorTelaAtual(TelaCircular *atual) {
    if (atual == NULL) return 0;
    return atual->tela;
}

int AED_AvancarTelaCircular(TelaCircular **atual) {
    if (*atual == NULL) return 0;

    *atual = (*atual)->prox;
    return (*atual)->tela;
}

void AED_LiberarListaCircular(TelaCircular *lista) {
    if (lista == NULL) return;

    TelaCircular *atual = lista->prox;

    while (atual != lista) {
        TelaCircular *tmp = atual;
        atual = atual->prox;
        free(tmp);
    }

    free(lista);
}

/* BUSCAS E ORDENAÇÕES */

int AED_BuscaLinear(int v[], int n, int alvo) {
    for (int i = 0; i < n; i++) {
        if (v[i] == alvo) return i;
    }

    return -1;
}

int AED_BuscaBinaria(int v[], int n, int alvo) {
    int ini = 0;
    int fim = n - 1;

    while (ini <= fim) {
        int meio = (ini + fim) / 2;

        if (v[meio] == alvo) return meio;
        if (v[meio] < alvo) ini = meio + 1;
        else fim = meio - 1;
    }

    return -1;
}

static void trocar(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

static int particionar(int v[], int inicio, int fim) {
    int pivo = v[fim];
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        if (v[j] <= pivo) {
            i++;
            trocar(&v[i], &v[j]);
        }
    }

    trocar(&v[i + 1], &v[fim]);
    return i + 1;
}

void AED_QuickSort(int v[], int inicio, int fim) {
    if (inicio < fim) {
        int p = particionar(v, inicio, fim);
        AED_QuickSort(v, inicio, p - 1);
        AED_QuickSort(v, p + 1, fim);
    }
}

static void merge(int v[], int inicio, int meio, int fim) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    int *e = malloc(n1 * sizeof(int));
    int *d = malloc(n2 * sizeof(int));

    if (e == NULL || d == NULL) {
        free(e);
        free(d);
        return;
    }

    for (int i = 0; i < n1; i++) e[i] = v[inicio + i];
    for (int j = 0; j < n2; j++) d[j] = v[meio + 1 + j];

    int i = 0;
    int j = 0;
    int k = inicio;

    while (i < n1 && j < n2) {
        if (e[i] <= d[j]) v[k++] = e[i++];
        else v[k++] = d[j++];
    }

    while (i < n1) v[k++] = e[i++];
    while (j < n2) v[k++] = d[j++];

    free(e);
    free(d);
}

void AED_MergeSort(int v[], int inicio, int fim) {
    if (inicio < fim) {
        int meio = (inicio + fim) / 2;
        AED_MergeSort(v, inicio, meio);
        AED_MergeSort(v, meio + 1, fim);
        merge(v, inicio, meio, fim);
    }
}

static void heapify(int v[], int n, int i) {
    int maiorIndice = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n && v[esq] > v[maiorIndice]) maiorIndice = esq;
    if (dir < n && v[dir] > v[maiorIndice]) maiorIndice = dir;

    if (maiorIndice != i) {
        trocar(&v[i], &v[maiorIndice]);
        heapify(v, n, maiorIndice);
    }
}

void AED_HeapSort(int v[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(v, n, i);

    for (int i = n - 1; i > 0; i--) {
        trocar(&v[0], &v[i]);
        heapify(v, i, 0);
    }
}