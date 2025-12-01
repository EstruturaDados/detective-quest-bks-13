#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50
#define MAX_PISTA 50

// Estrutura que representa cada sala do mapa organizado como árvore binária
typedef struct MapNo {
    char nome[MAX_NOME];      // Nome da sala
    char *pista_encontrada;   // Ponteiro para uma pista que pode existir na sala
    struct MapNo *esquerda;   // Caminho à esquerda
    struct MapNo *direita;    // Caminho à direita
} MapNo;

// Estrutura da árvore de busca binária para armazenar pistas
typedef struct PistaNo {
    char pista[MAX_PISTA];    // Conteúdo da pista
    struct PistaNo *esquerda; // Nó menor na esquerda
    struct PistaNo *direita;  // Nó maior na direita
} PistaNo;

// Funções do mapa
MapNo* criarNoMapa(const char *nome, const char *pista);
MapNo* construirMansao();
void liberarMapa(MapNo *raiz);

// Funções da Arvore binaria de busca de pistas
PistaNo* criarNoPista(const char *pista);
PistaNo* inserirPista(PistaNo *raiz, const char *pista);
void mostrarEmOrdem(PistaNo *raiz);
void liberarPistas(PistaNo *raiz);

// Função de exploração
void explorarSalas(MapNo *hall, PistaNo **pistas_raiz);


// Cria um nó sala do mapa
MapNo* criarNoMapa(const char *nome, const char *pista) {
    MapNo *no = (MapNo *)malloc(sizeof(MapNo));
    if (!no) {
        perror("Erro ao alocar memória");
        exit(1);
    }

    strcpy(no->nome, nome);
    no->esquerda = NULL;
    no->direita = NULL;

    // Se a sala possui pista, duplica a string
    if (pista != NULL)
        no->pista_encontrada = strdup(pista);
    else
        no->pista_encontrada = NULL;

    return no;
}

// Constrói a mansão completa como uma árvore binária de salas
MapNo* construirMansao() {
    MapNo *hall = criarNoMapa("Hall de Entrada", "Retrato Rasgado");

    hall->esquerda = criarNoMapa("Sala de Jantar", NULL);
    hall->direita = criarNoMapa("Biblioteca", "Carta Manuscrita");

    hall->esquerda->esquerda = criarNoMapa("Cozinha", NULL);
    hall->esquerda->direita = criarNoMapa("Escritorio", "Chave Envelhecida");

    hall->direita->esquerda = criarNoMapa("Sala de Fumos", NULL);
    hall->direita->direita = criarNoMapa("Quarto Principal", NULL);

    hall->esquerda->esquerda->esquerda = criarNoMapa("Despensa (Fim)", NULL);
    hall->direita->direita->direita = criarNoMapa("Banheiro Privativo (Fim)", "Alianca Simples");

    return hall;
}

// Cria um nó da arvore binaria
PistaNo* criarNoPista(const char *pista) {
    PistaNo *no = (PistaNo *)malloc(sizeof(PistaNo));
    if (!no) {
        perror("Erro ao alocar memória");
        exit(1);
    }

    strcpy(no->pista, pista);
    no->esquerda = NULL;
    no->direita = NULL;

    return no;
}

// Insere pista na arvore binaria em ordem alfabética
PistaNo* inserirPista(PistaNo *raiz, const char *pista) {
    if (raiz == NULL)
        return criarNoPista(pista);

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);

    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);

    // Se for igual, não insere para evitar repetição
    return raiz;
}

// Percorre e mostra as pistas em ordem alfabética
void mostrarEmOrdem(PistaNo *raiz) {
    if (raiz) {
        mostrarEmOrdem(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        mostrarEmOrdem(raiz->direita);
    }
}


// Função principal de exploração do mapa, o jogador começa no "hall" e pode se mover pela mansão, coletando e inserindo na arvore binaria de pistas
void explorarSalas(MapNo *hall, PistaNo **pistas_raiz) {
    MapNo *atual = hall;
    char escolha[10];

    printf("Inicio da exploracao\n");

    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        // Se a sala possui pista, coleta e adiciona na BST
        if (atual->pista_encontrada != NULL) {
            printf("Pista encontrada: %s\n", atual->pista_encontrada);

            // Insere na arvore binaria
            *pistas_raiz = inserirPista(*pistas_raiz, atual->pista_encontrada);

            // Libera pista do nó do mapa
            free(atual->pista_encontrada);
            atual->pista_encontrada = NULL;
        }

        // Se a sala não tem mais caminhos, é um nó-folha
        if (!atual->esquerda && !atual->direita) {
            printf("Fim do caminho.\n");
            break;
        }

        // Menu de ações
        printf("Acoes: [e] Esquerda | [d] Direita | [p] Ver pistas | [s] Sair: ");
        scanf("%s", escolha);

        if (strcmp(escolha, "s") == 0) {
            break;
        }
        else if (strcmp(escolha, "p") == 0) {
            printf("\n--- Pistas coletadas ---\n");
            mostrarEmOrdem(*pistas_raiz);
            continue;
        }
        else if (strcmp(escolha, "e") == 0 && atual->esquerda) {
            atual = atual->esquerda;
        }
        else if (strcmp(escolha, "d") == 0 && atual->direita) {
            atual = atual->direita;
        }
        else {
            printf("Caminho invalido.\n");
        }
    }
}


//Funções para liberar memoria no mapa

void liberarMapa(MapNo *raiz) {
    if (raiz) {
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        free(raiz);
    }
}

void liberarPistas(PistaNo *raiz) {
    if (raiz) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

int main() {
    printf("--- Detective Quest ---\n");

    MapNo *mansao = construirMansao();
    PistaNo *pistas = NULL;

    explorarSalas(mansao, &pistas);

    printf("\nSaindo e Limpando memoria...\n");
    liberarMapa(mansao);
    liberarPistas(pistas);

    return 0;
}
