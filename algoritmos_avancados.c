#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50  // Tamanho maximo para os nomes dos comodos
#define MAX_PISTA 50 // Tamanho maximo para os nomes dados as pistas
#define MAX_SUSPEITO 30 //Tamanho maximo para os nomes dos suspeitos
#define TAMANHO_HASH 10 // Tamanho da tabela hash

//Estrutura que representa um comodo da mansao
typedef struct MapNo {
    char nome[MAX_NOME]; //Nome do comodo
    char *pista_encontrada; //Pista que esta associada ao comodo
    struct MapNo *esquerda; //Ponteiro para comodo a esquerda
    struct MapNo *direita; //Ponteiro para comodo a direita
} MapNo;

// Estrutura de nó da árvore binária 
typedef struct PistaNo {
    char pista[MAX_PISTA];
    struct PistaNo *esquerda;
    struct PistaNo *direita;
} PistaNo;

// Estrutura para item da tabela hash, onde associa pistas aos suspeitos
typedef struct HashItem {
    char pista[MAX_PISTA];
    char suspeito[MAX_SUSPEITO];
    struct HashItem *proximo;
} HashItem;

// Tabela hash: vetor de ponteiros para listas encadeadas de HashItem
typedef HashItem* TabelaHash[TAMANHO_HASH];

// Limpar buffer do teclado
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função hash simples para mapear uma pista a um índice na tabela
int funcaoHash(const char *pista) {
    int soma = 0;
    for (int i = 0; i < 3 && pista[i]; i++)
        soma += pista[i];
    return soma % TAMANHO_HASH;
}

// Insere uma pista na Bst de pistas coletadas
PistaNo* inserirPista(PistaNo *raiz, const char *pista) {
    if (!raiz) {
        PistaNo *n = malloc(sizeof(PistaNo));
        strcpy(n->pista, pista);
        n->esquerda = n->direita = NULL;
        return n;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

// Funçao para inserir uma pista ao suspeito na tabela hash
void inserirNaHash(TabelaHash tabela, const char *pista, const char *suspeito) {
    int idx = funcaoHash(pista);
    HashItem *novo = malloc(sizeof(HashItem));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabela[idx];
    tabela[idx] = novo;
}

// Funçao para encontrar o suspeito associado a uma pista na tabela hash
const char* encontrarSuspeito(TabelaHash tabela, const char *pista) {
    int idx = funcaoHash(pista);
    HashItem *aux = tabela[idx];
    while (aux) {
        if (strcmp(aux->pista, pista) == 0)
            return aux->suspeito;
        aux = aux->proximo;
    }
    return NULL;
}

// Funçao para criar dinamicamente um comodo da mansao
MapNo* criarSala(const char *nome, const char *pista) {
    MapNo *s = malloc(sizeof(MapNo));
    strcpy(s->nome, nome);
    s->esquerda = s->direita = NULL;
    s->pista_encontrada = pista ? strdup(pista) : NULL;
    return s;
}

// Função para construir manualmente a mansão com salas e pistas
MapNo* construirMansao() {
    MapNo *hall = criarSala("Hall de Entrada", "Retrato Rasgado"); 
    hall->esquerda = criarSala("Sala de Jantar", "Bilhete da Biblioteca"); 
    hall->direita = criarSala("Biblioteca", "Carta Manuscrita"); 
    hall->esquerda->esquerda = criarSala("Cozinha", "Chave Envelhecida");
    hall->esquerda->direita = criarSala("Escritorio", NULL);
    hall->direita->esquerda = criarSala("Sala de Fumos", "Manuscrito Antigo"); 
    hall->direita->direita = criarSala("Quarto Principal", "Diário Rasgado"); 
    hall->esquerda->esquerda->esquerda = criarSala("Despensa (Fim)", NULL);
    hall->direita->direita->direita = criarSala("Banheiro Privativo (Fim)", "Alianca Simples"); 
    return hall;
}

// Função de exploração interativa da mansão
void explorarSalas(MapNo *atual, PistaNo **pistas) {
    char esc[5];
    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);
        if (atual->pista_encontrada) {
            printf("PISTA ENCONTRADA: %s\n", atual->pista_encontrada);
            *pistas = inserirPista(*pistas, atual->pista_encontrada);
            free(atual->pista_encontrada);
            atual->pista_encontrada = NULL;
        }
        if (!atual->esquerda && !atual->direita) {
            printf("Fim do caminho. Não há mais salas para explorar.\n");
            return;
        }
        printf("Ações: [e] Esquerda | [d] Direita | [s] Sair: ");
        scanf("%s", esc);
        if (!strcmp(esc, "s")) return;
        else if (!strcmp(esc, "e")) atual = atual->esquerda ? atual->esquerda : atual;
        else if (!strcmp(esc, "d")) atual = atual->direita ? atual->direita : atual;
    }
}

// Função para inicializar a tabela hash com pistas e suspeitos
void inicializarHash(TabelaHash tabela) {
    inserirNaHash(tabela, "Carta Manuscrita", "Coronel Mustard");
    inserirNaHash(tabela, "Chave Envelhecida", "Coronel Mustard");
    inserirNaHash(tabela, "Retrato Rasgado", "Miss Scarlett");
    inserirNaHash(tabela, "Bilhete da Biblioteca", "Miss Scarlett");
    inserirNaHash(tabela, "Manuscrito Antigo", "Mrs. White");
    inserirNaHash(tabela, "Diário Rasgado", "Mrs. White");
    inserirNaHash(tabela, "Alianca Simples", "Professor Plum");
}

// Listar pistas coletadas
void listarPistas(PistaNo *r) {
    if (!r) return;
    listarPistas(r->esquerda);
    printf("- %s\n", r->pista);
    listarPistas(r->direita);
}

// Julgamento final
void verificarSuspeitoFinal(TabelaHash tabela, PistaNo *pistas) {
    printf("\n--- Pistas coletadas ---\n");
    listarPistas(pistas);

    printf("\nSuspeitos possíveis:\n");
    printf("1. Coronel Mustard\n2. Miss Scarlett\n3. Mrs. White\n4. Professor Plum\n");

    char nome[MAX_SUSPEITO];
    int cont = 0;
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    limpar_buffer();
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    //Percorre a bst usando pilha
    PistaNo *pilha[50];
    int topo = -1;
    PistaNo *atual = pistas;
    while (atual || topo >= 0) {
        while (atual) {
            pilha[++topo] = atual;
            atual = atual->esquerda;
        }
        atual = pilha[topo--];
        const char *sus = encontrarSuspeito(tabela, atual->pista);
        if (sus && strcmp(sus, nome) == 0)
            cont++;
        atual = atual->direita;
    }

    // Verifica se pelo menos 2 pistas apontam para o suspeito acusado
    if (cont >= 2)
        printf("\nParabéns! A acusação está correta. %s é o culpado!\n", nome);
    else
        printf("\nInfelizmente, a acusação está incorreta. %s NÃO é o culpado.\n", nome);
}

// Liberar memória
void liberarMansao(MapNo *r) {
    if (!r) return;
    liberarMansao(r->esquerda);
    liberarMansao(r->direita);
    free(r);
}

void liberarPistas(PistaNo *r) {
    if (!r) return;
    liberarPistas(r->esquerda);
    liberarPistas(r->direita);
    free(r);
}

void liberarHash(TabelaHash t) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashItem *aux = t[i];
        while (aux) {
            HashItem *tmp = aux;
            aux = aux->proximo;
            free(tmp);
        }
        t[i] = NULL;
    }
}

// Função principal
int main() {
    MapNo *mansao = construirMansao(); //Cria mansao e sala
    PistaNo *pistas = NULL; // Inicia a bst de pistas vazia
    TabelaHash tabela = {NULL}; //Inicializa tabela hash vazia

    printf("\n--- Detective Quest: Nível Mestre ---\n");

    // Associa pistas aos suspeitos
    inicializarHash(tabela);

    explorarSalas(mansao, &pistas); // Exploração interativa das salas

    verificarSuspeitoFinal(tabela, pistas);  // Julgamento final

    liberarMansao(mansao);
    liberarPistas(pistas);
    liberarHash(tabela);

    return 0;
}
